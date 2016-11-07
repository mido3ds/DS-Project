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

namespace Control
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
	// adds enemies nodes to enemies list of tower 
	// and reads towers' data
	void Read(Castle* C)
	{
		ifstream inFile ("input.txt");
		if (!inFile.is_open())
			throw -1;

		// other names for towers 
		Tower &TA = C->towers[0];
		Tower &TB = C->towers[1];
		Tower &TC = C->towers[2];
		Tower &TD = C->towers[3];

		// read tower data
		int TH, N, TP;
		inFile >> TH >> N >> TP;

		TOWER::Initialize(&TA ,TH, N, TP);
		TOWER::Initialize(&TB ,TH, N, TP);
		TOWER::Initialize(&TC ,TH, N, TP);
		TOWER::Initialize(&TD ,TH, N, TP);

		// read constants
		inFile >> C->c1 >> C->c2 >> C->c3;

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

			// detect tower and add enemy to its list
			switch (R)
			{
				case 'A':
				{
					// first enemy in list, pointer e points at NULL
					if (ea == NULL)
					{
						ea = ENEMY::Initialize(S, TY, T, H, Pow, Prd, Speed, static_cast<int>(A_REG));
						TA.firstEnemy = ea;
						TA.num_enemies++;
						continue;
					}

					// make new node and link the last one with it
					temp = ENEMY::Initialize(S, TY, T, H, Pow, Prd, Speed, static_cast<int>(A_REG));
					ea->next = temp;
					TA.num_enemies++;
					break;
				}
				case 'B':
				{
					// first enemy in list, pointer e points at NULL
					if (eb == NULL)
					{
						eb = ENEMY::Initialize(S, TY, T, H, Pow, Prd, Speed, static_cast<int>(B_REG));
						TB.firstEnemy = eb;
						TB.num_enemies++;
						continue;
					}

					// make new node and link the last one with it
					temp = ENEMY::Initialize(S, TY, T, H, Pow, Prd, Speed, static_cast<int>(B_REG));
					eb->next = temp;
					TB.num_enemies++;
					break;
				}
				case 'C':
				{
					// first enemy in list, pointer e points at NULL
					if (ec == NULL)
					{
						ec = ENEMY::Initialize(S, TY, T, H, Pow, Prd, Speed, static_cast<int>(C_REG));
						TC.firstEnemy = ec;
						TC.num_enemies++;
						continue;
					}

					// make new node and link the last one with it
					temp = ENEMY::Initialize(S, TY, T, H, Pow, Prd, Speed, static_cast<int>(C_REG));
					ec->next = temp;
					TC.num_enemies++;
					break;
				}
				case 'D':
				{
					// first enemy in list, pointer e points at NULL
					if (ed == NULL)
					{
						ed = ENEMY::Initialize(S, TY, T, H, Pow, Prd, Speed, static_cast<int>(D_REG));
						TD.firstEnemy = ed;
						TD.num_enemies++;
						continue;
					}

					// make new node and link the last one with it
					temp = ENEMY::Initialize(S, TY, T, H, Pow, Prd, Speed, static_cast<int>(D_REG));
					ed->next = temp;
					TD.num_enemies++;
					break;
				}
				default:
					throw -1;
					break;
			}
		}

	}
}
