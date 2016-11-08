/*
*   DS Project CMP-2020 Cairo Uni., Castle Defending Simulator
*	Here is the Logic of the simulator and the main function
*   Team: Mahmoud Adas, Mahmoud Yousri, Khaled Sabry
*   Started 1-Nov-16, Delivered: -----
*/

// project header files
#include "Graph.h"
#include "Data.h"
#include "Const.h"
#include "Control.h"

using namespace control;


int main()
{
	//Phase1();

	Castle c;
	Read(c);
	SetWindow();
	Enemy** arr = new Enemy*[c.towers[0].num_enemies];

	Enemy* e = c.towers[0].firstEnemy;
	arr[0] = e;
	for (int i = 1; i < c.towers[0].num_enemies; i++)
		{
			Tower &T = c.towers[0];

			e = e->next;
			arr[i] = e;
		}

	for (int i = 0; i < 60; i++)
		{
			DrawCastle(c, i);
			DrawEnemies(arr, c.towers[0].num_enemies);

			for (int i = 0; i < c.towers[0].num_enemies; i++)
				{
					ENEMY::Move(*arr[i]);
				}
	
			Sleep(SECOND);
		}
}