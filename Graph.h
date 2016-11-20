// DO NOT EDIT THIS!  //

#pragma once
#include <iostream>
#include <windows.h>
#include "Data.h"
using namespace std;

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

