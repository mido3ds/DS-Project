#pragma once
#include "Graph.h"
#include "Data.h"
#include "Const.h"
#include "Control.h"
using namespace control;

#define _TEST_

int main()
{
    Phase1();
}

// int main()
// {
//     Castle c;
// 	Read(c);
// 	SetWindow();
// 	Enemy** arr = new Enemy*[c.towers[0].num_enemies];

// 	Enemy* e = c.towers[0].firstEnemy;
// 	arr[0] = e;
// 	for (int i = 1; i < c.towers[0].num_enemies; i++)
// 		{
// 			Tower &T = c.towers[0];

// 			e = e->next;
// 			arr[i] = e;
// 		}

// 	for (int i = 0; i < 60; i++)
// 		{
// 			DrawCastle(c, i);
// 			DrawEnemies(arr, c.towers[0].num_enemies);

// 			for (int i = 0; i < c.towers[0].num_enemies; i++)
// 				{
// 					ENEMY::Move(*arr[i]);
// 				}
	
// 			Sleep(SECOND);
// 		}
// }