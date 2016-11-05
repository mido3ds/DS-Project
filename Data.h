#pragma once
#include <iostream>
using namespace std;
/*  Data header file, contains all data structure to store data of castles and enemies
*   and functions prototypes to read, write, modifie and delete them ...etc
*   Team: Adas, Yousry and khaled sabry
*/

/*		// Tips: Read Carefully!  //
*   -document everything you write, comment it and be aware of the readability of your code, and chose meaningfull nanes for variables 
*   -write comments before your functions to tell the reader how to use it and how it works, you are not the only working with it
*   -test every thing you develop before you move to next part, make a temporary main function in test file and test your code, keep copy of test main in its folder
*   -don't leave your work before deadline, plz we don't have much time
*   -check group for updates
*   -upload your edits to project to github and notifiy your group of them daily
*   -write pseudo-code comments in function before making it, to make it easy to develop, read and modify the function later
*/

/* constatnts  */
#ifndef NULL
#define NULL				nullptr
#endif
#define NUM_OF_TOWERS	4
#define CmdWidth		150
#define CmdHeight		50
#define CastleWidth		30
#define CastleLength	20
#define CastleXStrt		(CmdWidth/2-(CastleWidth/2))
#define CastleYStrt		(CmdHeight/2-(CastleLength/2))
#define TowerWidth      7
#define TowerLength     3
#define EnemyShape		219  //ASCII code of enemy char shape 

/* Enumerators */
//Enemy types: Paver, Fighter, and Shielded Fighter
enum TYPE {
	PVR,
	FITR,
	SHLD_FITR
};

//The four regions: A, B, C , and D
enum REGION {	
	A_REG,
	B_REG,
	C_REG,
	D_REG
};


/* Prototypes */
struct Tower;
struct Castle;
struct Enemy;
Castle* newCastle();
void ReadTower(const Tower* t, const int &TH, const int &N, const int &TP);
Enemy* newEnemy(const int &S, const int &TY, const int &T, const int &H, const int &Pow, const int &Prd, const int &R);


/* Structures Implementation */
struct Tower
{
	// Given Properities :-
	int TW; //Tower width
	int TL; //Tower Height
	int Health;

	// Modified Properities :-

	// from input:
	int unpaved;		// the pos of first unpaved block, initialized to 30
	int fire_power;		
	int maxN_enemies; 		// max num of enemies it can fight

	// Pointers to enemies list
	Enemy* firstEnemy;		// initailized to NULL
	int num_enemies;
};

struct Castle
{
	// Given Properities :-
	//starting x,y
	int Xstrt;
	int Ystrt;
	int W;	//width
	int L;  //Height
	Tower towers[NUM_OF_TOWERS];	//Castle has 4 towers	

	// Modified Properities :-
	double c1, c2, c3;		// constants to calculate priority enemies
};

struct Enemy
{
	// Given Properities :-
	int ID;			//Each enemy has a unique ID (sequence number)
	REGION Region;	//Region of this enemy
	int Distance;	//Distance to the castle, initialized to 0 (Unactive)
	double Health;	//Enemy health
	TYPE Type;		//PVR, FITR, SHLD_FITR

	// Modified Properities :-

	// from input:
	int arrive_time;
	int fire_power;		// if paver, it is num of metres it can pave
	int reload_period;

	// to be calculated for output
	int fight_delay;	// time of begin fighting - time of arrival
	int kill_delay;		// time of kill - time of arrival
	// initialized to -1, to catch the bug if it is still -1 at end of programm


	// Pointers
	Enemy* next;		// initialize to NULL
};


/* functions implementation  */

// returns new castle with its constants initialized
Castle* newCastle()
{
	Castle* C = new Castle;
	
	C->Xstrt = CastleXStrt;
	C->Ystrt = CastleYStrt;
	C->W = CastleWidth;
	C->L = CastleLength;

	return C;
}

// initialize Tower 
// input: Tower address, input line
void ReadTower(Tower* t, const int &TH, const int &N, const int &TP)
{
	// check if NULL
	if (!t)
		throw -1;

	t->Health = TH;
	t->maxN_enemies = N;
	t->fire_power = TP;

	t->unpaved = 30;
	t->firstEnemy = NULL;
	t->num_enemies = 0;
} 

// returns address of enemy initialized with input variables
Enemy* newEnemy(const int &S, const int &TY, const int &T, const int &H, const int &Pow, const int &Prd, const int &R)
{
	Enemy* e = new Enemy;

	e->ID = S;
	e->Type = static_cast<TYPE>(TY);
	e->arrive_time = T;
	e->Health = H;
	e->fire_power = Pow;
	e->reload_period = Prd;
	e->Region = static_cast<REGION>(R);
	
	e->Distance = 0;
	e->fight_delay = -1;
	e->kill_delay = -1;
	e->next = NULL;

	return e;
}