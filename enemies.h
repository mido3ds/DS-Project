/*  Enemies header file, contains all data structure to store data of enemies
*   and functions to read, write, modifie and delete them ...etc
*   Main Responsibilty of: Mahmoud Youssry
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
#include "castle.h"
using namespace std;

//Enemy types: Paver, Fighter, and Shielded Fighter
enum Etype {
	PVR,
	FITR,
	SHLD_FITR
};

struct Enemy
{
	
	int ID;			//Each enemy has a unique ID (sequence number)
	REGION Region;	//Region of this enemy
	int Distance;	//Distance to the castle
	double Health;	//Enemy health
	Etype Type;		//PVR, FITR, SHLD_FITR
};