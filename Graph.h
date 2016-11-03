<<<<<<< HEAD:Graph.h
// DO NOT EDIT THIS!  //

#pragma once
#include <iostream>
#include <windows.h>
#include "Data.h"
using namespace std;


//constants
#define CmdWidth		150
#define CmdHeight		50
#define CastleWidth		30
#define CastleLength	20
#define CastleXStrt		(CmdWidth/2-(CastleWidth/2))
#define CastleYStrt		(CmdHeight/2-(CastleLength/2))
#define TowerWidth      7
#define TowerLength     3
#define EnemyShape		219  //ASCII code of enemy char shape 


//Functions Prototype

/* set cursor position on the screen */
void gotoxy(const int &x, const int &y);

/* set the command window lenght and height for the game specification */
void SetWindow();

/* color the cmd text */
void color(const int &thecolor);

/* partition the castle into regions (A,B,C,D) */
void DrawRegions(const Castle & C);

/* draw the castle and the towers */
void DrawCastle(const Castle & C,int SimulationTime);

/* draw a single enemy using its distance from the castle */
void DrawEnemy(const Enemy& E, int Ypos=0);

/* draw a list of enemies exist in all regions and ensure there is no overflow in the drawing */
void DrawEnemies(Enemy* enemies[], int size);

/* print message on screen */
void PrintMsg(char* msg);

=======
// DO NOT EDIT THIS!  //

#pragma once
#include <iostream>
#include <windows.h>
#include "Data.h"
using namespace std;


//constants
#define CmdWidth		150
#define CmdHeight		50
#define CastleWidth		30
#define CastleLength	20
#define CastleXStrt		(CmdWidth/2-(CastleWidth/2))
#define CastleYStrt		(CmdHeight/2-(CastleLength/2))
#define TowerWidth      7
#define TowerLength     3
#define EnemyShape		219  //ASCII code of enemy char shape 


//Functions Prototype

/* set cursor position on the screen */
void gotoxy(const int &x, const int &y);

/* set the command window lenght and height for the game specification */
void SetWindow();

/* color the cmd text */
void color(const int &thecolor);

/* partition the castle into regions (A,B,C,D) */
void DrawRegions(const Castle & C);

/* draw the castle and the towers */
void DrawCastle(const Castle & C,int SimulationTime);

/* draw a single enemy using its distance from the castle */
void DrawEnemy(const Enemy& E, int Ypos=0);

/* draw a list of enemies exist in all regions and ensure there is no overflow in the drawing */
void DrawEnemies(Enemy* enemies[], int size);

/* print message on screen */
void PrintMsg(char* msg);

>>>>>>> origin/master:Graph.h
