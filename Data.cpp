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
    // initialize Tower 
    // input: Tower address, input line
    void Initialize(Tower* t, const int &TH, const int &N, const int &TP)
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
}

namespace ENEMY
{
    // returns address of enemy initialized with input variables
    Enemy* Initialize(const int &S, const int &TY, const int &T, const int &H, const int &Pow, const int &Prd, const int &Speed, const int &R)
    {
        Enemy* e = new Enemy;

        e->ID = S;
        e->Type = static_cast<TYPE>(TY);
        e->arrive_time = T;
        e->Health = H;
        e->fire_power = Pow;
        e->reload_period = Prd;
		e->speed = Speed;
        e->Region = static_cast<REGION>(R);
        
        e->Distance = 0;
        e->fight_delay = -1;
        e->kill_delay = -1;
        e->next = NULL;

        return e;
    }



}