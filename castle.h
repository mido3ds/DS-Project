/*  Castles header file, contains all data structure to store data of castles
*   and functions to read, write, modifie and delete them ...etc
*   Main Responsibilty of: Khaled Sabry
*   DEADLINE: Before 11/11
*   Team: Adas, Yousry and khaled sabry
*/
/*  Tips: Read Carefully!
*   -document everything you write, comment it and be aware of the readability of your code, and chose meaningfull nanes for variables 
*   -write comments before your functions to tell the reader how to use it and how it works, you are not the only working with it
*   -test every thing you develop before you move to next part, make a temporary main function in test file and test your code, keep copy of test main in its folder
*   -don't leave your work before deadline, plz we don't have much time
*   -check group for updates
*   -upload your edits to project to github and notifiy your group of them daily
*   -write pseudo-code comments in function before making it, to make it easy to develop, read and modify the function later
*/

#pragma once
#include <iostream>
#include "utility.h"
#include "enemies.h"
using namespace std;


//The four regions: A, B, C , and D
enum REGION {	
	A_REG,
	B_REG,
	C_REG,
	D_REG
};

struct Tower
{
	int TW; //Tower width
	int TL; //Tower Height
	int Health;
};

struct Castle
{
	//starting x,y
	int Xstrt;
	int Ystrt;
	int W;	//width
	int L;  //Height
	Tower towers[TowersNum];	//Castle has 4 towers
};