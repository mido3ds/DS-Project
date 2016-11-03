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

// The state of the enemy: active, inactive, dead
enum STATE
{
	ACTIVE, INACTIVE, DEAD
};

//The four regions: A, B, C , and D
enum REGION {	
	A_REG,
	B_REG,
	C_REG,
	D_REG
};


/* Structures Prototypes */
struct Tower;
struct Castle;
struct Enemy;

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
	const int fire_power;		
	const int maxN_enemies; 		// max num of enemies it can fight

	// Pointers to enemies list
	Enemy* next;		// initailized to first one in list after reading
	int num_enemies;

	// Constructor: initialize variables
	Tower(int TH, int N, int TP): 
		Health(TH), 
		maxN_enemies(N),
		fire_power(TP),

		unpaved(30),
		next(NULL),
		num_enemies(0)
	{}
};

struct Castle
{
	//starting x,y
	int Xstrt;
	int Ystrt;
	int W;	//width
	int L;  //Height
	const Tower* towers[NUM_OF_TOWERS];	//Castle has 4 towers

	// Constructor: initialize variables
	Castle(Tower* T0, Tower* T1, Tower* T2, Tower* T3):
		Xstrt(CastleXStrt),
		Ystrt(CastleYStrt),
		W(CastleWidth),
		L(CastleLength)
	{
		towers[0] = T0;
		towers[1] = T1;
		towers[2] = T2;
		towers[3] = T3;
 	}
	
};

struct Enemy
{
	// Given Properities :-
	const int ID;			//Each enemy has a unique ID (sequence number)
	REGION Region;	//Region of this enemy
	int Distance;	//Distance to the castle, initialized to 60 (the begining)
	double Health;	//Enemy health
	const TYPE Type;		//PVR, FITR, SHLD_FITR

	// Modified Properities :-

	// from input:
	const int arrive_time;
	const int fire_power;		// if paver, it is num of metres it can pave
	const int reload_period;

	STATE state;		// initialized to ACTIVE

	// to be calculated for output
	int fight_delay;	// time of begin fighting - time of arrival
	int kill_delay;		// time of kill - time of arrival
	// initialized to -1, to catch the bug if it is still -1 at end of programm


	// Pointers
	Enemy* next;		// initialize to NULL

	// constructor: initialize variables
	Enemy(int S, int TY, int T, int H, int Pow, int Prd, int R): 
		ID(S),
		Type(static_cast<TYPE>(TY)),
		arrive_time(T),
		Health(H),
		fire_power(Pow),
		reload_period(Prd),
		Region(static_cast<REGION>(R)),

		Distance(60),
		state(ACTIVE),
		fight_delay(-1),
		kill_delay(-1),
		next(NULL)
	{}

	
};

