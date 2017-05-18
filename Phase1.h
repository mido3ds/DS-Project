#ifndef __Phase1_h__
#define __Phase1_h__

#include "Data.h"

namespace Phase1
{
	void Start();
	void _SortKilled(Enemy** arr, const int &size);
	void _PrintTime(const int &time);
	void _PrintRegion(const int &i);
	void _Timer(Castle &c);
}

namespace Phase1
{
/*		! phase1 is done !
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
			priority active enemies and randomly pick two from normal active enemies. 	(done)
			iii.  Remove killed enemies from the list(s).	(done)
			iv.  For each region, print
				1.  Total number of active enemies and information of each one of them.		(done)
				2.  Total number of killed enemies and information of each one of them.		(done)
				The killed enemies have to be printed ordered by enemy health.
*/

using namespace control;

/* the interface */
void Start()
{
	Castle c;
	Read (c);
	_Timer(c);
}

/* private functions: */

// sort the array of killed enemies before they are printed
// sorte depending on their health from minimum to maximum health
// using Selection Sort, complexity of O(n^2)
void _SortKilled(Enemy** arr, const int &size)
{
	if (!arr || size < 0)
		throw -1;

	for (int i = 0; i < size; i++)
		for (int j = i + 1; j < size; j++)
			if (arr[i]->Health > arr[j]->Health)
				ENEMY::Swap(arr[i], arr[j]);
			
}

	
void _PrintTime(const int &time)
{
	cout << "-----------------------";
	cout << "Time = " << time << 's';
	cout << "------------------------" << endl;
}

void _PrintRegion(const int &i)
{
	cout << "========= ";
	cout << "Region " << (char)('A' + i) << ":\n";
}

// all what is done in every second
void _Timer(Castle &c)
{
	int time;
	// do the same each time step
	// stops when all enemies in all regions are killed 
	for (time = 1; !CASTLE::IsEmpty(c); time++)
	{
		_PrintTime(time);

		// do the same for all the Regions
		for (int i = A_REG; i <= D_REG; i++)
		{
			_PrintRegion(i);

			// to store killed enemies
			int killcount = 0;
			Enemy** killed = new Enemy*[4];	// array of pointers to killed enemies

			// store num of active enemies
			int activecount = 0;

			// alias for the current tower
			Tower &T = c.towers[i];

			cout << "Active Enemies: \n";
			
			// normal enemies:
			// iterate through all enemies
			Enemy* e = T.firstEnemy;
			Enemy* temp = NULL;
			while(e)
			{
				ENEMY::Move(*e, T);

				if (ENEMY::IsActive(*e))	// active
				{
					// kill him, print in Killed
					if (killcount < 2)
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
						T.num_enemies--;
						continue;
					}
					// cannot kill him, print in active
					else 
					{
						// print him as active but not killed 
						ENEMY::Print(*e);
						activecount++;
					}
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

				if (ENEMY::IsActive(*e))	// active
				{
					// kill him, print in Killed
					if (killcount < 4)
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
						T.num_enemies--;
						continue;
					}

					// cannot kill him, print in active
					if (killcount > 3)
					{
						// print him as active but not killed 
						ENEMY::Print(*e);
						activecount++;
					}
				}

				// go to the next enemy
				temp = e;
				e = e->next;
			}

			// Printing the active:-
			// if there is no active enemies to print , print none
			if (!activecount)
				cout << "NONE\n";
			else	// print the num
				cout << "Total Num = " << activecount << '\n';
			
			
			// printing the dead:-
			// sort the array of killed enemies in ascending order of their health
			_SortKilled(killed, killcount);

			// print killed 
			cout << "Killed Enemies: \n";
			for (int i = 0; i < killcount; i++)
				ENEMY::Print(*killed[i]);

			// if no one is killed, print none
			if (killcount == 0)
				cout << "NONE\n";
			else // print the num of them
				cout << "Total Num = " << killcount << '\n'; 

			cout << endl;

		}

		cout << endl;
	}

	cout << "Finished Killing all at Time: " << --time << '\n';
}
}

#endif  /* __Phase1_h__ */