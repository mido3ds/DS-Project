#include "Control.h"
#include <cstdlib>
#include <limits>

/*		// Tips: Read Carefully!  //
*   -document everything you write, comment it and be aware of the readability of your code, and chose meaningfull nanes for variables 
*   -write comments before your functions to tell the reader how to use it and how it works, you are not the only working with it
*   -test every thing you develop before you move to next part, make a temporary main function in test file and test your code, keep copy of test main in its folder
*   -don't leave your work before deadline, plz we don't have much time
*   -check group for updates
*   -upload your edits to project to github and notifiy your group of them daily
*   -write pseudo-code comments in function before making it, to make it easy to develop, read and modify the function later
*/

namespace Control
{	
	void Start()
	{
		Castle c; 

		// initialize	
		Read(c);

		// play
		Loop(c);

		// end
		End(c);
	}

	// destroy castle and end log file
	void End(Castle &c)
	{
		Log::End(c);
		CASTLE::Destroy(c);
	}

	// main loop
	void Loop(Castle &c)
	{
		for (int timer = 0; !HasFinished(c); timer++)
		{
			// fight in one time step
			CASTLE::Loop(c, timer);

			// refresh frame
			Refresh(c, timer);
		}
	}

	// ask the user to enter a particular mode from 3 
	// returns chosen mode
	Mode GetMode()
	{
		printf("Choose the mode by entering its number:\n");
		printf("1- Interactive Mode\n");
		printf("2- Step-by-step Mode\n");
		printf("3- Silent Mode\n");

		Mode mode;

		char q = cin.get();
		switch (q)
		{
		case '1':
			// interact
			mode = INTERACTIVE;
			break;

		case '2':
			// step by step
			mode = STEP;
			break;

		default:
			// silent mode
			mode = SILENT;
			break;
		}
		return mode;
	}

	// read all input from input.txt 
	// initializes all towers, enemies and castle
	// adds enemies nodes to enemies list of tower 
	// and reads towers' data
	void Read(Castle &C)
	{
		CASTLE::Initialize(C);

		ifstream inFile ("input.txt");
		if (!inFile.is_open())
			throw -1;

		// other names for towers 
		Tower &TA = C.towers[0];
		Tower &TB = C.towers[1];
		Tower &TC = C.towers[2];
		Tower &TD = C.towers[3];

		// read tower data
		int TH, N, TP;
		inFile >> TH >> N >> TP;
		TOWER::Initialize(C ,TH, N, TP);

		// read constants
		inFile >> TOWER::c1 >> TOWER::c2 >> TOWER::c3;

		// read enemies
		int S, TY, T, H, Pow, Prd, Speed; char R;		// variables to read at one line as specified
		Enemy *ea, *eb, *ec, *ed;	// pointers at end of normal enemies list
		Enemy *sa, *sb, *sc, *sd;	// pointers at end of shielded enemies list
		// initialize them to NULL
		ea = eb = ec = ed = NULL;
		sa = sb = sc = sd = NULL;

		// iterate through all lines untill end of file (-1)
		while (true)
		{
			if (inFile.eof())
				throw -1;		// no -1 at end of file 

			
			// read first term and check 
			inFile >> S;
			if (S == -1)
				break;		// reached end of file

			inFile >> TY >> T >> H >> Pow >> Prd >> Speed >> R;
			
			// add shielded enemies to its list, and normal enemies to its list
			if (TY == SHLD_FITR)
			{
				// add to shielded list 
				switch (R)
				{
					case 'A':
						SHIELDED::Add(&TA, sa, S, T, H, Pow, Prd, Speed, A_REG);
						break;
					case 'B':
						SHIELDED::Add(&TB, sb, S, T, H, Pow, Prd, Speed, B_REG);
						break;
					case 'C':
						SHIELDED::Add(&TC, sc, S, T, H, Pow, Prd, Speed, C_REG);
						break;
					case 'D':
						SHIELDED::Add(&TD, sd, S, T, H, Pow, Prd, Speed, D_REG);
						break;
				}
			}
			else // normal enemy
			{
				// add to normal list 
				switch (R)
				{
					case 'A':
						ENEMY::Add(&TA, ea, S, TY, T, H, Pow, Prd, Speed, A_REG);
						break;
					case 'B':
						ENEMY::Add(&TB, eb, S, TY, T, H, Pow, Prd, Speed, B_REG);
						break;
					case 'C':
						ENEMY::Add(&TC, ec, S, TY, T, H, Pow, Prd, Speed, C_REG);
						break;
					case 'D':
						ENEMY::Add(&TD, ed, S, TY, T, H, Pow, Prd, Speed, D_REG);
						break;
				}
			}

		}

		// initialize the log file
		Log::Initialize(C);
	}

	bool HasFinished(const Castle &c)
	{
		return (CASTLE::IsEmpty(c) || CASTLE::IsDestroyed(c));
	}
#undef max(a, b)
	// interacts with user depending on the choosen mode
	void Interact(const Mode &mode)
	{
		switch (mode)
		{
			case SILENT:
			{
				return;
			}
			case INTERACTIVE:
			{
				// take input from user to continue
				cin.ignore(numeric_limits<streamsize>::max(), '\n');		// ignore last input not to be fooled :D
				cin.get();
				return;
			}
			case STEP:
			{
				// wait for a second
				Sleep(SECOND);
				return;
			}
		}
	}

	// refresh the frame
	void Refresh(const Castle &c, const int &timer)
	{
		/* ask user for the mode of simulation one time */
		static Mode mode = GetMode();

		// draw only if not silent
		if (mode == SILENT)
			return;
		
		/* draw on screen */
		SetWindow();

		DrawCastle(c, timer);
		
		DrawEnemies(c, timer);

		/* print data on screen */
		Log::ToScreen(c);

		/* interact with user depending on the choosen mode */
		Interact(mode);
	}

	void DrawEnemies(const Castle &c, const int &timer)
	{
		for (int i = A_REG; i <= D_REG; i++)
			DrawEnemies(&c.towers[i], i, timer);
	}

	// draw a tower, overloaded
	void DrawEnemies(const Tower* T, const int &region, const int &timer)
	{
		int CountEnemies = 0;	//count Enemies at the same distance to draw them vertically if they are <= 15 enemy else print number of enemy in the region
		bool draw = true;

		if (!T)
			throw -1;
		if (!T->firstEnemy && !T->firstShielded)	// if no enemies return
			return;

		Enemy* e = nullptr;
		for(int distance = ((CmdWidth/2) - (CastleWidth/2)); distance > 1; distance--)
		{
			CountEnemies = 0;
			
			// traverse through normal enemies
			e = T->firstEnemy;
			while (e && ENEMY::IsActive(*e, timer))
			{	
				if(e->Distance == distance && e->Region == region)
					CountEnemies++;
				e = e->next;
			}

			// traverse through shielded
			e = T->firstShielded;
			while (e && ENEMY::IsActive(*e, timer))
			{
				if(e->Distance == distance && e->Region == region)
					CountEnemies++;
				e = e->next;
			}

			if(CountEnemies>15)
			{
				draw = false;
				break;
			}

		}

		if(draw)
		{
			for(int distance = ((CmdWidth/2)-(CastleWidth/2)); distance > 1; distance--)
			{
				CountEnemies = 0;

				// traverse through normal
				e = T->firstEnemy;
				while (e && ENEMY::IsActive(*e, timer))
				{	
					if(e->Distance == distance)
					{
						DrawEnemy(*e, CountEnemies);
						CountEnemies++;
					}
					e = e->next;
				}

				// traverse through shielded
				e = T->firstShielded;
				while (e && ENEMY::IsActive(*e, timer))
				{	
					if(e->Distance == distance)
					{
						DrawEnemy(*e, CountEnemies);
						CountEnemies++;
					}
					e = e->next;
				}

			}


		}
		else // print message maximum reached in this region
		{
			int x;int y;
			if(region==A_REG)
			{
				x= CastleXStrt-30;
				y= (CmdHeight/2)-(CastleLength/4);
			}
			else if(region==B_REG)
			{
				x= CastleXStrt+CastleWidth+30;
				y= (CmdHeight/2)-(CastleLength/4);


			}
			else if(region==C_REG)
			{
				x= CastleXStrt+CastleWidth+30;
				y= (CmdHeight/2)+(CastleLength/4);

			}
			else
			{
				x= CastleXStrt-30;
				y= (CmdHeight/2)+(CastleLength/4);

			}
			gotoxy(x, y);
			cout<<"Maximum limit";
		}


		gotoxy(0, CmdHeight-1);
	}


	// draw by a list , overloaded
	void DrawEnemies(Enemy* head, const int &region, const int &timer)
	{
		int CountEnemies = 0;	//count Enemies at the same distance to draw them vertically if they are <= 15 enemy else print number of enemy in the region
		bool draw = true;
		Enemy* e = nullptr;

		// if no active or no enemy, return 
		if (!head || !ENEMY::IsActive(*head, timer))
			return;

		for(int distance = ((CmdWidth/2) - (CastleWidth/2)); distance > 1; distance--)
		{
			CountEnemies = 0;
			
			e = head;
			while (e && ENEMY::IsActive(*e, timer))
			{	
				if(e->Distance == distance && e->Region == region)
					CountEnemies++;
				e = e->next;
			}

			if(CountEnemies>15)
			{
				draw = false;
				break;
			}

		}

		if(draw)
		{
			for(int distance = ((CmdWidth/2)-(CastleWidth/2)); distance > 1; distance--)
			{
				CountEnemies = 0;

				e = head;
				while (e && ENEMY::IsActive(*e, timer))
				{	
					if(e->Distance == distance)
					{
						DrawEnemy(*e, CountEnemies);
						CountEnemies++;
					}
					e = e->next;
				}

			}


		}
		else // print message maximum reached in this region
		{
			int x;int y;
			if(region==A_REG)
			{
				x= CastleXStrt-30;
				y= (CmdHeight/2)-(CastleLength/4);
			}
			else if(region==B_REG)
			{
				x= CastleXStrt+CastleWidth+30;
				y= (CmdHeight/2)-(CastleLength/4);


			}
			else if(region==C_REG)
			{
				x= CastleXStrt+CastleWidth+30;
				y= (CmdHeight/2)+(CastleLength/4);

			}
			else
			{
				x= CastleXStrt-30;
				y= (CmdHeight/2)+(CastleLength/4);

			}
			gotoxy(x, y);
			cout<<"Maximum limit";
		}


		gotoxy(0, CmdHeight-1);
	}

	// overloading original function to be more efficient (array)
	void DrawEnemies(Enemy* enemies[], const int &size, const int &region)
	{
		int CountEnemies =0;	//count Enemies at the same distance to draw them vertically if they are <= 15 enemy else print number of enemy in the region
		bool draw = true;
		for(int distance = ((CmdWidth/2) - (CastleWidth/2)); distance>1; distance--)
		{
			CountEnemies=0;

			for(int i=0;i<size;i++)
			{	
				// modification to ignore nullptrs 
				if (enemies[i] == NULL)	
					continue;

				if(enemies[i]->Distance==distance && enemies[i]->Region == region)
				{
					CountEnemies++;
				}
			}
			if(CountEnemies>15)
			{
				draw=false;
				break;
			}

		}
		if(draw)
		{
			for(int distance=((CmdWidth/2)-(CastleWidth/2));distance>1;distance--)
			{
				CountEnemies=0;

				for(int i=0;i<size;i++)
				{	
					// modification to ignore nullptrs 
					if (enemies[i] == NULL)	
						continue;

					if(enemies[i]->Distance==distance && enemies[i]->Region == region)
					{
						DrawEnemy(*(enemies[i]),CountEnemies);
						CountEnemies++;
					}
				}

			}


		}
		else // print message maximum reached in this region
		{
			int x;int y;
			if(region==A_REG)
			{
				x= CastleXStrt-30;
				y= (CmdHeight/2)-(CastleLength/4);
			}
			else if(region==B_REG)
			{
				x= CastleXStrt+CastleWidth+30;
				y= (CmdHeight/2)-(CastleLength/4);


			}
			else if(region==C_REG)
			{
				x= CastleXStrt+CastleWidth+30;
				y= (CmdHeight/2)+(CastleLength/4);

			}
			else
			{
				x= CastleXStrt-30;
				y= (CmdHeight/2)+(CastleLength/4);

			}
			gotoxy(x, y);
			cout<<"Maximum limit";
		}


		gotoxy(0, CmdHeight-1);
	}
}
