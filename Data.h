/*  Data header file, contains all data structure to store data of castles and enemies
*   and functions prototypes to read, write, modifie and delete them ...etc
*   Team: Adas, Yousry and khaled sabry
*/
#ifndef __Data_h__
#define __Data_h__

#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include "Const.h"
#include "Sound.h"
using namespace std;

/* Enumerators */
//Enemy types: Paver, Fighter, and Shielded Fighter
enum TYPE {
	PVR,
	FITR,
	SHLD_FITR,
	DOC,
	TANK_ENEM
};

//The four regions: A, B, C , and D
enum REGION {	
	A_REG,
	B_REG,
	C_REG,
	D_REG
};

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
	void _Transfer(Tower* T1, Tower* T2, TYPE type, const REGION &Region);
	void _Fix_forTransfer(Tower* T, Enemy *e, const REGION &Region);
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
	void _InsertAfter(Enemy* e1, Enemy* &e2);
	bool IsShielded(const Enemy *e);
	bool IsPaver(const Enemy &e);
	bool IsFighter(const Enemy &e);
	bool IsDoctor(Enemy* e);
	bool IsTank(Enemy *e);
	char GetRegion(Enemy *e);
	char GetRegion(const int &region);
	void Destroy(Enemy* e);
	Enemy* _Generate(const int &timer, const int &region, const int &distance);
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

namespace DOCTOR
{
	void Heal(Enemy* d);
}

namespace PAVER
{
	void Pave(Enemy* e, Tower* t);
}

namespace TANK
{
	void Drop(Enemy* h, Tower* T, const int &timer);
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

#endif  /* __Data_h__ */