// DO NOT EDIT THIS!  //

#pragma once
#include <iostream>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include "Data.h"
using namespace std;

//Functions Prototype

/* public */

/* set the command window lenght and height for the game specification */
void SetWindow();

/* draw the castle and the towers */
void DrawCastle(const Castle & C,int SimulationTime);

/* draw a list of enemies exist in all regions and ensure there is no overflow in the drawing */
void DrawEnemies(Enemy* enemies[], int size);

/* print message on screen */
void PrintMsg(char* msg);


/* private */

/* set cursor position on the screen */
void gotoxy(const int &x, const int &y);

/* color the cmd text */
void color(const int &thecolor);

/* partition the castle into regions (A,B,C,D) */
void DrawRegions(const Castle & C);

/* draw a single enemy using its distance from the castle */
void DrawEnemy(const Enemy& E, int Ypos=0);

void PutAt(char q, int x, int y);

namespace TANK
{
	void Draw(const int &x, const int &y);
}
namespace DOCTOR
{
	void Draw(const int &x, const int &y);
}