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
	}

	// loop for all towers in one time step
	void Loop(Castle &c, const int &timer)
	{
		// loop for regions
		for (int region = A_REG; region <= D_REG; region++)
		{
			// pseudo
				// iterate through enemies:
					// move enemy
					// fire at tower if possible
					// add it to active array
					// break if enemy is not avtive
				// begin with shielded enemies, then the normal
				// tower fires at enemies
				// draw 
				// if tower is destroyed transfer enemies

			// tower alias
			Tower &T = c.towers[region];

			// dont waste time with destroyed/empty tower
			if (TOWER::IsDestroyed(T) || TOWER::IsEmpty(T))
				continue;

			// array of active enemies 
			Enemy** active = new Enemy*[T.num_enemies];
			int act_count = 0;		// counter to store in array

			// enemies fire, move and be killed
			ENEMY::Loop(T.firstShielded, T, timer, active, act_count);
			ENEMY::Loop(T.firstEnemy, T, timer, active, act_count);

			// tower fires
			TOWER::Fire(&T, active, act_count, timer);
			
			if (TOWER::IsDestroyed(T))
				TOWER::Transfer(c, region);

			delete[] active;
		}
	}

	// check if all towers in castle with no enemies
	bool IsEmpty(const Castle &c)
	{
		return (TOWER::IsEmpty(c.towers[0]) && TOWER::IsEmpty(c.towers[1]) &&TOWER::IsEmpty(c.towers[2]) &&TOWER::IsEmpty(c.towers[3]));
	}

	bool IsDestroyed(const Castle &c)
	{
		return (TOWER::IsDestroyed(c.towers[A_REG]) 
				&& TOWER::IsDestroyed(c.towers[B_REG]) 
				&& TOWER::IsDestroyed(c.towers[C_REG]) 
				&& TOWER::IsDestroyed(c.towers[D_REG]));
	}

	int GetTotalEnemies(Castle &c)
	{
		return (c.towers[0].num_enemies + c.towers[1].num_enemies + c.towers[2].num_enemies + c.towers[3].num_enemies);
	}

	// destroy all lists
	void Destroy(Castle &c)
	{
		for (int i = A_REG; i < D_REG; i++)
			TOWER::Destroy(&c.towers[i]);
	}
}

namespace TOWER
{
    // defining extern constants 
	double c1, c2, c3;

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
			t->firstShielded = NULL;
			t->num_enemies = 0;
			t->TW = TowerWidth;
			t->TL = TowerLength;
		}
    } 

	// check if Tower is Empty of both normal and shielded enemies
	bool IsEmpty(const Tower &t)
	{
		return (t.num_enemies == 0);
	}

	// damge enemies
	// enemies are stored, sorted, killed & printed to screen/file if enemy isdead
	void Fire(Tower* t, Enemy* arr[], int size, int time)
	{
		if (size == 0 || !t || !arr)
			return;

		SHIELDED::GetPriority(arr, size, time);
		SHIELDED::Sort(arr,size);
		
		Enemy** killed = new Enemy*[t->maxN_enemies];
		int kill_count = 0;


		for (int i = 0; i < t->maxN_enemies && i < size && arr[i]; i++)
		{
			ENEMY::Damage(arr[i], t, time);
			
			// remove from list if dead
			if(arr[i]->Health <= 0)
			{
				killed[kill_count++] = arr[i];
				arr[i] = NULL;
			}
		}

		// sort depending on FD
		for (int i = 0; i < kill_count; i++)
		{   
			int min = i;
			for(int j=i+1;j<kill_count;j++)
			{
				if (killed[j]->fight_delay < killed[j]->fight_delay)
					min = j;
			}

			if (min != i)
				ENEMY::Swap(arr[min],arr[i]);
		}

		// kill & print
		for (int i = 0; i < kill_count; i++)
			ENEMY::Kill(killed[i], t, time);

		delete[] killed;

	}

	bool IsDestroyed(const Tower &t)
	{
		return (t.Health <= 0);
	}

	// damage to tower by enemy 
	// decrease its health
	void Damage(Enemy* e, Tower* t)                 
	{
		// provided constant
		double k = 1;
		if (ENEMY::IsShielded(e))
			k++;
		
		// update health
		t->Health -= (k / e->Distance) * e->fire_power;		
    }
	
	// move enemies from destroyed tower to next one 
	void Transfer(Castle &c, int region)
	{ 
		// check if tower has enemies to move
		if (c.towers[region].num_enemies == 0)
			return;

		// index of next tower
		int nextTower = (region + 1) % NUM_OF_TOWERS;

		// test all next towers to find the tower that can 
		for (int i = 0; i < 3; nextTower = (nextTower + 1) % NUM_OF_TOWERS, i++)
		{
			// found non-destroyed tower, transfer to it
			if (c.towers[nextTower].Health > 0)
			{
				_Transfer(&c.towers[region], &c.towers[nextTower], SHLD_FITR);
				_Transfer(&c.towers[region], &c.towers[nextTower], FITR);
				return;
			}
		}
	}

	// move enemies from Tower 1 --> Tower 2
	void _Transfer(Tower* T1, Tower* T2, TYPE type)
	{
		Enemy* list1 = nullptr; Enemy* list2 = nullptr;
		// choose the list to transfer
		switch (type)
		{
			case SHLD_FITR:
				list1 = T1->firstShielded;
				list2 = T2->firstShielded;
				break;
			default:
				list1 = T1->firstEnemy;
				list2 = T2->firstEnemy;
				break;
		}

		// iterate through all enemies in list1
		while (list2)
		{
			// to move it
			Enemy* temp = list1;

			// now list points at next one
			list1 = list1->next;

			// cut it
			list1->prev = temp->next = temp->prev = nullptr;

			// move list2 to the proper position
			while (list2 && temp->arrive_time > list2->arrive_time)
				list2 = list2->next;

			ENEMY::_InsertBefore(temp, list2);

			// if list2 is first one, insert at first position
			if (temp->prev == nullptr)
			{
				// insert at first
				if (type == SHLD_FITR)
					T2->firstShielded = list1;		
				else 
					T2->firstEnemy = list1;
			}
		}
	}

	// delete all lists in tower
	void Destroy(Tower* T)
	{
		if (!T)
			throw -1;

		if (T->firstEnemy)
			ENEMY::Destroy(T->firstEnemy);

		if (T->firstShielded)
			ENEMY::Destroy(T->firstShielded);
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
		e->priority = -1;
        
        e->Distance = 60 + e->arrive_time * e->speed;
        e->fight_delay = -1;
        e->kill_delay = -1;
        e->next = NULL;
		e->prev = NULL;

        return e;
    }

	// loop for one enemy
	// move enemy, make him fire, add him to active list while this enmey is active
	void Loop(Enemy* e, Tower* T, const int &timer, Enemy* active[], int &act_count)
	{
		while (e && ENEMY::IsActive(*e, timer))
		{
			ENEMY::Move(*e, T);

			ENEMY::Fire(e, &T);

			active[act_count++] = e;

			// go to next enemy
			e = e->next;
		}
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
		temp->prev = lastOne;
		t->num_enemies++;		// new enemy added

		lastOne = temp;		// update lastone
		return lastOne;
	}

	// moves enemy according to its speed and unpaved area
	// takes enemy to move, tower to detect if possible to move depending on unpaved are
	void Move(Enemy &e, const Tower &T)
	{
		// don't let enemy get iside tower
		if (e.Distance > MIN_DISTANCE_FROM_CASTLE)
			e.Distance -= e.speed;

		// if entered paved area, return him to the beginnig of it
		if (e.Distance < T.unpaved)
			e.Distance = T.unpaved;
	}

	// for phase1 only, we won't use it
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

	// print enemy data to screen
	// it prints id, type health and arrive time 
	void Print(const Enemy &e)
	{
		cout << "-Enemy ID: " << e.ID;
		cout << ", Type: ";
		switch (e.Type)
		{
			case FITR:
				cout << "Fighter";
				break;
			case PVR:
				cout << "Paver";
				break;
			default:
				cout << "Shielded";
				break;
		}

		cout << ", Health: " << e.Health;
		cout << ", Arrival Time: " << e.arrive_time << endl;
	}

	// takes two pointers at enemies objects and swaps them
	void Swap(Enemy* &a, Enemy* &b)
	{
		Enemy* temp = a;
		a = b;
		b = temp;
	}

	// takes enemy and determines if active or not
	// active: arrival time <= time
	bool IsActive(const Enemy &e, const int &time)
	{
		return (e.arrive_time <= time);
	}

	bool IsPaver(const Enemy &e)
	{
		return (e.Type == PVR);
	}

	// returns true if enemy is shielded
	bool IsShielded(const Enemy *e)
	{
		return (e->Type == SHLD_FITR);
	}

	bool IsFighter(const Enemy &e)
	{
		return (e.Type == FITR);
	}

	// remove enemy from the list 
	void Kill(Enemy* e, Tower* t, const int &time)      
	{
		if (!e || !t)
			throw -1;

		Enemy* e_plus = e->next;
		Enemy* e_minus = e->prev;

		if (e_plus)
			e_plus->prev = e_minus;

		if (e_minus)
			e_minus->next = e_plus;
		else	// remove first one, update the head
		{
			if (e->Type == SHLD_FITR)
				t->firstShielded = e_plus;
			else 	// normal
				t->firstEnemy = e_plus;
		}

		// calc Kill delay KD = time - (FD + arrival time)
		e->kill_delay = time - (e->fight_delay + e->arrive_time);
		Log::total_KD += e->kill_delay;

		Log::ToFile(e, time);
		
		delete e;
		t->num_enemies--;
	}

	//function to check if the enemy can fire his weapon or not at ginven time step
	bool CanFire(Enemy* e, int time)                       
	{ 
		// special case
		if (e->reload_period == 0)
			return true;

		return ((time - e->arrive_time) % e->reload_period == 0);
	}

	//function to reduce enemy's health 
	void Damage(Enemy* e, Tower* t, const int &time)
	{
		// provided constant
		double k = 1;
		if (ENEMY::IsShielded(e))
			k++;

		// update health
		e->Health -= ((1.0 / e->Distance) * t->fire_power * (1.0 / k));  

		// calc fight delay FD = Time - arrival time
		if (e->fight_delay == -1)	// firt time
			e->fight_delay = time - e->arrive_time;                             
	}

	// enemy fires at given tower
	// paver pave with their fire power 
	void Fire(Enemy* e,Tower* t)
	{
		if (ENEMY::IsPaver(*e))
		{
			if (e->Distance == t->unpaved)
			{
				// paves and moves to the beginnig of unpaved area
			    t->unpaved = t->unpaved - e->fire_power;
				e->Distance = t->unpaved;	
			}
		}
		else
			TOWER::Damage(e, t);
	}

	// insert e1 before e2
	void _InsertBefore(Enemy* e1, Enemy* e2)
	{
		e1->next = e2;
			
		if (!e2 || !e2->prev)
			return;

		e1->prev = e2->prev;
		e2->prev->next = e1;

		e2->prev = e1;
	}

	char GetRegion(Enemy *e)
	{
		if (!e)
			throw -1;
		switch (e->Region)
		{
			case A_REG:
				return 'A';
			case B_REG:
				return 'B';
			case D_REG:
				return 'D';
			case C_REG:
				return 'C';
			default:
				throw -1;
		}
	} 

	// kill all enemies in list, remove them instantly
	void Destroy(Enemy* e)
	{
		if (!e)
			throw -1;

		if (e->next)
			Destroy(e->next);

		delete e;
	}

}

namespace SHIELDED
{
	// adds shielded enemy to the end of list 
	// if is first one, it makes tower points at this enemy
	Enemy* Add(Tower* t, Enemy* &lastOne,
		const int &S, const int &T, const int &H,
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
			lastOne = ENEMY::Initialize(S, SHLD_FITR, T, H, Pow, Prd, Speed, R);
			t->firstShielded = lastOne;		// tower points at enemy added
			t->num_enemies++;		// new enemy added
			return lastOne;
		}

		// it is not the first one
		temp = ENEMY::Initialize(S, SHLD_FITR, T, H, Pow, Prd, Speed, R);		// new enemy at temp
		lastOne->next = temp;		// previous node points at the next one
		temp->prev = lastOne;
		t->num_enemies++;		// new enemy added

		lastOne = temp;		// update lastone
		return lastOne;
	}

	// calculates priorities of a list of shilded and store them in Enemey::priority
	int GetPriority(Enemy*arr[], int size, int time)
	{ 
		int i = 0;
      	while (i < size && ENEMY::IsShielded(arr[i]))
		{
			arr[i]->priority = (arr[i]->fire_power /arr[i]->Distance) * TOWER::c1 + TOWER::c2/((time - arr[i]->arrive_time)+1)+arr[i]->Health *TOWER::c3;
			i++;
		}

		return (i);
	}

	// sort an array of shielded enemies ,from max to min, depending on its priority
	// use selection sorting
	void Sort(Enemy* arr[], const int &size)
	{
		int maxIndex = 0;

		for (int i = 0; i < size && ENEMY::IsShielded(arr[i]); i++)
		{
			// assume 
			maxIndex = i;

			for (int j = i + 1; j < size && ENEMY::IsShielded(arr[j]); j++)
			{
				if (arr[i]->priority < arr[j]->priority)
				{
					// should be swaped
					maxIndex = j;
				}
			}

			// swap
			if (maxIndex != i)
				ENEMY::Swap(arr[i], arr[maxIndex]);
		}
	}
}

namespace Log
{
	// bunch of variables to store information to be print at end
	int total_FD = 0;
	int total_KD = 0;
	int total_enemies_beg = 0;		// at beginning
	int tower_health_beg = 0;		// at beginning


	// to init the file
	void Initialize(Castle &c)
	{
		ofstream outFile("output.txt", ios::out);
		if (!outFile)
		{
			cerr << "FATAL ERROR: Couldn't open file output.txt\n";
			exit(1);
		}
		
		// write first line
		outFile << "KTS S FD KD FT\n";

		total_enemies_beg = CASTLE::GetTotalEnemies(c);
		tower_health_beg = c.towers[0].Health;
	}

	// add enemy to file 
	void ToFile(Enemy* e, const int &time)
	{
		if (!e)
			throw -1;

		// to file
		ofstream outFile("output.txt", ios::app);
		if (!outFile)
		{
			cerr << "FATAL ERROR: Could not open file output.txt\n";
			exit(1);
		}

		int &KTS = time, &S = e->ID, &FD = e->fight_delay, &KD = kill_delay, &FT = KD + FD;

		outFile << KTS << ' ' << S << ' ' << FD << ' ' << KD<< ' ' << FT << '\n';
	}

	// add towers data to file
	void ToFile(Castle &c)
	{
		ofstream outFile("output.txt", ios::app);
		if (!outFile)
		{
			cerr << "FATAL ERROR: Could not open file output.txt\n";
			exit(1);
		}

		// calc damage
		int T1_D = tower_health_beg - c.towers[0].Health, 
			T2_D = tower_health_beg - c.towers[1].Health, 
			T3_D = tower_health_beg - c.towers[2].Health,
			T4_D = tower_health_beg - c.towers[3].Health;

		outFile << "T1_Total_Damage T2_Total_Damage T3_Total_Damage T4_Total_Damage\n";
		outFile << T1_D << ' '
				<< T2_D << ' '
				<< T3_D << ' '
				<< T4_D << '\n';

		// print unpaved
		outFile << "R1_Distance R2_Distance R3_Distance R4_Distance\n";
		outFile << c.towers[0].unpaved << ' '	
				<< c.towers[1].unpaved << ' '
				<< c.towers[2].unpaved << ' '
				<< c.towers[3].unpaved << '\n'
	}

	// end of file
	void End(Castle &c)
	{
		ofstream outFile("output.txt", ios::app);
		if (!outFile)
		{
			cerr << "FATAL ERROR: Could not open file output.txt\n";
			exit(1);
		}

		int total_killed = (total_enemies_beg - CASTLE::GetTotalEnemies(c));

		// WIN
		if (CASTLE::IsEmpty(c))
		{
			/*a. Total number of enemies
			b. Average “Fight Delay” and Average “Kill Delay”*/
			outFile << "Game is WIN\n";

			outFile << "Total Enemies = " << total_enemies_beg << '\n';

			outFile << "Average Fight Delay = " << (total_FD + total_KD) / total_enemies_beg << '\n';
			outFile << "Average Kill Delay = " << total_KD / total_enemies_beg << '\n';
		}
		else if (CASTLE::IsDestroyed(c)) // LOOSE
		{
			/*a. Numberofkilledenemies
			b. Numberofaliveenemies(activeandinactive)
			c. Average “Fight Delay” and Average “Kill Delay” for killed enemies only*/
			outFile << "Game is LOST\n";

			outFile << "Number of killed enemies = " << total_killed << '\n';
			outFile << "Number of alive enemies = " << CASTLE::GetTotalEnemies(c) << '\n';

			outFile << "Average Fight Delay = " << (total_FD + total_KD) / total_killed << '\n';
			outFile << "Average Kill Delay = " << total_KD / total_killed << '\n';
		}
			

		outFile.close();
	}
}