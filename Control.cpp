#include "Control.h"
#include <cstdlib>

/*		// Tips: Read Carefully!  //
*   -document everything you write, comment it and be aware of the readability of your code, and chose meaningfull nanes for variables 
*   -write comments before your functions to tell the reader how to use it and how it works, you are not the only working with it
*   -test every thing you develop before you move to next part, make a temporary main function in test file and test your code, keep copy of test main in its folder
*   -don't leave your work before deadline, plz we don't have much time
*   -check group for updates
*   -upload your edits to project to github and notifiy your group of them daily
*   -write pseudo-code comments in function before making it, to make it easy to develop, read and modify the function later
*/

namespace control
{
	void Start()
	{
		// initiatlze
		Castle c;
		Read(c);

		// play
		_Loop(c);
		// detect win or lose
	}

	// main loop
	void _Loop(Castle &c)
	{
		SetWindow();

		for (int timer = 0;; timer++)
		{
			// refresh frame
			DrawCastle(c, timer);

			// loop for regions
			for (int region = A_REG; region <= D_REG; region++)
			{
				// iterate through enemies:
					// move enemy
					// fire at tower if possible
					// add it to active array
					// break if enemy is not avtive
				// begin with shielded enemies, then the normal
				// tower fires at enemies
				// draw 
				// if tower is destroyed transfer enemies

				// tower alias
				Tower& T = c.towers[region];

				// dont waste time with destroyed/empty tower
				if (TOWER::IsDestroyed(T) || TOWER::IsEmpty(T))
					continue;

				// active array	at most of all enemies
				Enemy** active = new Enemy*[T.num_enemies];
				int act_count = 0;		// counter to store in array

				// shielded enemies
				Enemy* e = T.firstShielded;
				while (e && ENEMY::IsActive(*e, timer))
				{
					// move enemy
					// fire at tower if possible
					// add it to active array
					// break if enemy is not avtive

					ENEMY::Move(*e, T);

					ENEMY::Fire(e, &T);

					active[act_count++] = e;

					// go to next enemy
					e = e->next;
				}

				// normal enemies
				e = T.firstEnemy;
				while (e && ENEMY::IsActive(*e, timer))
				{
					// move enemy
					// fire at tower if possible
					// add it to active array
					// break if enemy is not avtive

					ENEMY::Move(*e, T);

					ENEMY::Fire(e, &T);

					active[act_count++] = e;

					// go to next enemy
					e = e->next;
				}

				// tower fires
				TOWER::Fire(&T, active, act_count, timer);
				
				if (TOWER::IsDestroyed(T))
					 TOWER::Transfer(c, region);

				DrawEnemies(active, act_count);

				delete[] active;
			}

			// did game end?
			if (CASTLE::IsEmpty(c) || CASTLE::IsDestroyed(c))
				break;
			//Sleep(SECOND);
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

		char q = cin.get();
		switch (q)
		{
		case '1':
			// interact
			return INTERACTIVE;
			break;

		case '2':
			// step by step
			return STEP;
			break;

		default:
			// silent mode
			return SILENT;
			break;
		}
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
	}
}
