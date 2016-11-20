#include "Control.h"

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
		inFile >> C.c1 >> C.c2 >> C.c3;

		// read enemies
		int S, TY, T, H, Pow, Prd, Speed; char R;		// variables to read at one line as specified
		Enemy *ea, *eb, *ec, *ed, *temp;	// pointers at end of enemies list, temp for the current node being read
		Enemy *sa, *sb, *sc, *sd;
		temp = ea = eb = ec = ed = NULL;
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
				// add to its list 
				switch (R)
				{
					case 'A':
						SHIELDED::Add(&TA, sa, S, TY, T, H, Pow, Prd, Speed, A_REG);
						break;
					case 'B':
						SHIELDED::Add(&TB, sb, S, TY, T, H, Pow, Prd, Speed, B_REG);
						break;
					case 'C':
						SHIELDED::Add(&TC, sc, S, TY, T, H, Pow, Prd, Speed, C_REG);
						break;
					case 'D':
						SHIELDED::Add(&TD, sd, S, TY, T, H, Pow, Prd, Speed, D_REG);
						break;
				}
			}
			else // fighter or paver enemy
			{
				// add to its list 
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

	/*
		1- File loading function. The function that reads input file to: (done)
			a.  Load Towers data
			b.  Load constants values
			c.  Create and populate inactive enemies list.

		2- Simple Simulator function. This function should
			a.  Perform any needed initializations	(done)
			b.  Call file loading function	(done)	
			﻿c.  At each time step do the following
				i.  Move active enemies from inactive list to active list	(done)
				ii.  Pick at most 4 random active enemies to kill: Randomly pick two from high
				priority active enemies and randomly pick two from normal active enemies.
				iii.  Remove killed enemies from the list(s).	(done)
				iv.  For each region, print
					1.  Total number of active enemies and information of each one of them.
					2.  Total number of killed enemies and information of each one of them.
					The killed enemies have to be printed ordered by enemy health.
		*/

	void Phase1()
	{
		Castle c;
		Read (c);

		for (int time = 1; time < 2; time++)
		{
			cout << "At Time: " << time << endl;

			// do the same for all the Towers
			for (int i = 0; i < 4; i++)
			{
				cout << "At Region " << i + 1 << ":\n";


				// to store killed enemies
				int killcount = 0;
				Enemy** killed = new Enemy*[4];	// array of pointers to killed enemies

				// alias for the current tower
				Tower &T = c.towers[i];

				// normal enemies:
				// iterate through all enemies
				Enemy* e = T.firstEnemy;
				Enemy* temp = NULL;
				while(e)
				{
					ENEMY::Move(*e, T);

					// check if enemy is active to kill him later
					if (e->Distance < 60 && killcount < 2)
					{
						killed[killcount++] = e;

						// special case: when enemy is first one
						if (temp == NULL)
						{
							T.firstEnemy = e->next;
							e->next = NULL;

							// traverse to next enemy
							e = T.firstEnemy;
						}
						else 
						{
							// isolate it from the list
							temp->next = e->next;
							e->next = NULL;

							// traverse to next enemy
							e = temp->next;
						}
						continue;
					}

					// active but cannot kill him
					if (e->Distance < 60 && killcount >= 2)
					{
						// print him as active but not killed 
						ENEMY::Print(*e);
					}


					// go to the next enemy
					temp = e;
					e = e->next;
				}


				// shielded enemies:
				// iterate through all enemies
				e = T.firstShielded;
				temp = NULL;
				while(e)
				{
					ENEMY::Move(*e, T);

					// check if enemy is active to kill him later
					if (e->Distance < 60 && killcount < 4)
					{
						killed[killcount++] = e;

						// special case: when enemy is first one
						if (temp == NULL)
						{
							// isolate it from the list
							T.firstShielded = e->next;
							e->next = NULL;

							// traverse to next enemy
							e = T.firstShielded;
						}
						else 
						{
							// isolate it from the list
							temp->next = e->next;
							e->next = NULL;

							// traverse to next enemy
							e = temp->next;
						}
						continue;
					}

					// active but cannot kill him
					if (e->Distance < 60 && killcount > 3)
					{
						// print him as active but not killed 
						ENEMY::Print(*e);
					}

					// go to the next enemy
					temp = e;
					e = e->next;
				}
				

				// sort the array of killed enemies in descend order of their health
				SortKilled(killed, killcount);

				// print killed 
				cout << "Killed Enemies: ";
				for (int i = 0; i < killcount; i++)
					ENEMY::Print(*killed[i]);

				// if no one is killed, print none
				if (killcount == 0)
					cout << "NONE";
				
				cout << endl;

			}

			cout << endl;
		}

		cout << endl;
	}

	// used only Phase1 to sort the array of killed enemies before they are printed
	// sorte depending on their health from minimum to maximum health
	// using Selection Sort of O(n^2)
	void SortKilled(Enemy** arr, const int &size)
	{
		if (!arr || size < 0)
			throw -1;

		for (int i = 0; i < size; i++)
		{
			Enemy &e = *arr[i];
			
			for (int j = i + 1; j < size; j++)
			{
				Enemy &e2 = *arr[j];

				if (e.Health > e2.Health)
				{
					// swap
					Enemy *temp = &e;
					e = e2;
					e2 = *temp;
				}
			}
				
		}
	}
}
