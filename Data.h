#pragma once
#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip>
#include "Const.h"
#include "Sound.h"
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

namespace CASTLE
{
	void Initialize(Castle &C);
	void Loop(Castle &c, const int &timer);
	bool IsEmpty(const Castle &c);
	bool IsDestroyed(const Castle &c);
	int GetTotalEnemies(const Castle &c);
	void Destroy(Castle &c);
	bool HasFinished(Castle &c);
}

namespace TOWER
{
	extern double c1, c2, c3;		// constants read in run-time to calculate priority enemies

	void Initialize(Castle &c, const int &TH, const int &N, const int &TP);
	bool IsEmpty(const Tower &t);
	void Fire(Tower* t, Enemy* arr[], int size, int time);
	bool IsDestroyed(const Tower &t);
	void Damage(Enemy* e, Tower* t);
	void Transfer(Castle &c, int region);
	void _Transfer(Tower* T1, Tower* T2, TYPE type);
	void Destroy(Tower* T);
	bool HasFinished(Tower &T);
	int GetNumOfShielded(Tower *T);
	int GetNumOfNormal(Tower *T);
}

namespace ENEMY
{
	Enemy* Initialize(const int &S, const int &TY, const int &T, const int &H, const int &Pow, const int &Prd, const int &Speed, const REGION &R);

	void Loop(Enemy* e, Tower* T, const int &timer, Enemy* active[], int &act_count);

	Enemy* Add(Tower* t, Enemy* &lastOne,
		const int &S, const int &TY, const int &T, const int &H,
		const int &Pow, const int &Prd, const int &Speed, const REGION &R);

	void Move(Enemy &e, const Tower &T);

	Enemy* AddToDead(Enemy* e);

	void Print(const Enemy &e);

	void Swap(Enemy* &a, Enemy* &b);

	bool IsActive(const Enemy &e, const int &time);

	void Kill(Enemy* e, Tower* t, const int &time);

	bool CanFire(Enemy* e, int time);

	void Damage(Enemy* e, Tower* t, const int &time);

	void Fire(Enemy* e,Tower* t);

	void _InsertBefore(Enemy* e1, Enemy* e2);

	bool IsShielded(const Enemy *e);

	bool IsPaver(const Enemy &e);

	bool IsFighter(const Enemy &e);

	char GetRegion(Enemy *e);

	char GetRegion(const int &region);

	void Destroy(Enemy* e);
}

namespace SHIELDED
{
	Enemy* Add(Tower* t, Enemy* &lastOne,
		const int &S, const int &T, const int &H,
		const int &Pow, const int &Prd, const int &Speed, const REGION &R);
   
   int GetPriority(Enemy*arr[],int size,int Time);

   void Sort(Enemy* arr[], const int &size);

}

namespace Log
{
	// bunch of variables to store information to be print at end
	extern int total_FD;
	extern int total_KD;
	extern int total_enemies_beg;		// at beginning
	extern int tower_health_beg;		// at beginning
	extern int last_killed[NUM_OF_TOWERS];
	extern int all_killed[NUM_OF_TOWERS];

	// to init the file
	void Initialize(Castle &c);

	// add enemy to file 
	void ToFile(Enemy* e, const int &time);

	// add towers data to file
	void ToFile(const Castle &c);

	// end of file, state is the state of the game 
	void End(const Castle &c);

	void ToScreen(const Castle &c);
}


/* Structures */
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
	Enemy* firstShielded;	// initialized to NULL
	int num_enemies;	// increamented by one when adding enemy, referes to all enemies
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
};

struct Enemy
{
	// Given Properities :-
	int ID;			//Each enemy has a unique ID (sequence number)
	REGION Region;	//Region of this enemy
	int Distance;	//Distance to the castle, initialized to 0 (Unactive)
	int Health;		//Enemy health
	TYPE Type;		//PVR, FITR, SHLD_FITR

	// Modified Properities :-

	// from input:
	int arrive_time;
	int fire_power;		// if paver, it is num of metres it can pave
	int reload_period;
	int speed;		// speed of object  (bonus) 

	// to be calculated for output
	int fight_delay;	// time of begin fighting - time of arrival
	int kill_delay;		// time of kill - time of arrival
	// initialized to -1, to catch the bug if it is still -1 at end of programm

     double priority;		// initialized to -1
	// Pointers
	Enemy* next;		// initialize to NULL
	Enemy* prev;		// initialize to NULL

};