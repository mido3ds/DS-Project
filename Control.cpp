﻿#include "Control.h"

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
		temp = ea = eb = ec = ed = NULL;

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
			
			// add to its list (Tower)
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

	/*
		1- File loading function. The function that reads input file to:
			a.  Load Towers data
			b.  Load constants values
			c.  Create and populate inactive enemies list.

		2- Simple Simulator function. This function should
			a.  Perform any needed initializations
			b.  Call file loading function
			﻿c.  At each time step do the following
				i.  Move active enemies from inactive list to active list
				ii.  Pick at most 4 random active enemies to kill: Randomly pick two from high
				priority active enemies and randomly pick two from normal active enemies.
				iii.  Remove killed enemies from the list(s).
				iv.  For each region, print
					1.  Total number of active enemies and information of each one of them.
					2.  Total number of killed enemies and information of each one of them.
					The killed enemies have to be printed ordered by enemy health.
		*/
	void Phase1()
	{
		SetWindow();
		Castle c; 
		Read(c);

		// list for dead enemies
		Enemy* dead_List = NULL;

		// iterate one time unit, test
		for (int time = 1; time < 2;time++)
		{
			
			// do the same for all towers
			for (int i = 0; i < NUM_OF_TOWERS; i++)
			{
				Tower &T = c.towers[i];		// rename the tower
				
				// iterate through all enemies in this tower
				Enemy* prev = NULL;
				Enemy* e = T.firstEnemy;
				while (e) {
					using namespace ENEMY;

					if (e->Distance > T.unpaved)
						Move(*e);

					if (e->Health == 0)		// dead, kill him, add to dead list 
						dead_List = AddToDead(e);
					
					//if (e->Type != PVR && (time - e->arrive_time + 1) % e->reload_period == 1)
						//Fire(e, &T) 


				}
			}

			DrawCastle(c, time);
		}
	}


}
