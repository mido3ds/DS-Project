#include "Data.h"

/*		// Tips: Read Carefully!  //
*   -document everything you write, comment it and be aware of the readability of your code, and chose meaningfull nanes for variables 
*   -write comments before your functions to tell the reader how to use it and how it works, you are not the only working with it
*   -test every thing you develop before you move to next part, make a temporary main function in test file and test your code, keep copy of test main in its folder
*   -don't leave your work before deadline, plz we don't have much time
*   -check group for updates
*   -upload your edits to project to github and notifiy your group of them daily
*   -write pseudo-code comments in function before making it, to make it easy to develop, read and modify the function later
*/


namespace CASTLE
{
	// initialize Castle
	void Initialize(Castle &C)
	{
		C.Xstrt = CastleXStrt;
		C.Ystrt = CastleYStrt;
		C.W = CastleWidth;
		C.L = CastleLength;

		C.c2 = C.c3 = C.c1 = 0;
	}
}

namespace TOWER
{
    // initialize castle towers 
    // input: castle, input line
    void Initialize(Castle &c, const int &TH, const int &N, const int &TP)
    {
		// iterate through all towers
		for (int i = 0; i < NUM_OF_TOWERS; i++)
		{
			Tower* t = &(c.towers[i]);		// point at tower

			t->Health = TH;
			t->maxN_enemies = N;
			t->fire_power = TP;

			t->unpaved = 30;
			t->firstEnemy = NULL;
			t->num_enemies = 0;
			t->TW = TowerWidth;
			t->TL = TowerLength;
		}
    } 
}

namespace ENEMY
{
    // returns address of enemy initialized with input variables
    Enemy* Initialize(const int &S, const int &TY, const int &T, const int &H, const int &Pow, const int &Prd, const int &Speed, const REGION &R)
    {
        Enemy* e = new Enemy;

        e->ID = S;
        e->Type = static_cast<TYPE>(TY);
        e->arrive_time = T;
        e->Health = H;
        e->fire_power = Pow;
        e->reload_period = Prd;
		e->speed = Speed;
        e->Region = R;
        
        e->Distance = 60 + e->arrive_time * e->speed;
        e->fight_delay = -1;
        e->kill_delay = -1;
        e->next = NULL;

        return e;
    }

	// adds enemy to the end of list 
	// if is first one, it makes tower points at this enemy
	Enemy* Add(Tower* t, Enemy* &lastOne,
		const int &S, const int &TY, const int &T, const int &H,
		const int &Pow, const int &Prd, const int &Speed, const REGION &R)
	{
		// check if tower is provided
		if (!t)
			throw -1;

		Enemy* temp;

		// if first one in list, 
		// change the pointer of tower to point at it
		if (lastOne == NULL)
		{
			lastOne = ENEMY::Initialize(S, TY, T, H, Pow, Prd, Speed, R);
			t->firstEnemy = lastOne;		// tower points at enemy added
			t->num_enemies++;		// new enemy added
			return lastOne;
		}

		// it is not the first one

		temp = ENEMY::Initialize(S, TY, T, H, Pow, Prd, Speed, R);		// new enemy at temp
		lastOne->next = temp;		// previous node points at the next one
		t->num_enemies++;		// new enemy added

		lastOne = temp;		// update lastone
		return lastOne;
	}

	// moves enemy according to its speed and unpaved area
	// takes enemy to move, tower to detect if possible to move depending on unpaved are
	void Move(Enemy &e, const Tower &T)
	{
		// chek if it can move
		if (e.Distance > T.unpaved)
		{
			// don't let enemy get iside tower
			if (e.Distance > MIN_DISTANCE_FROM_CASTLE)
				e.Distance -= e.speed;
		}
	}

	// for phase1 only
	// adds in dead list and returns pointer to first enemy in it
	Enemy* AddToDead(Enemy* e)
	{
		static Enemy* list = NULL;
		static Enemy* lastptr = list;

		if (!e)
			throw -1;

		if (!list) {	// first one
			list = e;
			lastptr = e;
			return list;
		}


		// add
		lastptr->next = e;
		lastptr = e;

		return list;
	}
}