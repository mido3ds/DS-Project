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
			ENEMY::Loop(T.firstShielded, &T, timer, active, act_count);
			ENEMY::Loop(T.firstEnemy, &T, timer, active, act_count);

			// tower fires
			TOWER::Fire(&T, active, act_count, timer);
			
			if (TOWER::IsDestroyed(T))
			{
				T.Health = 0;
				TOWER::Transfer(c, region);
			}

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

	int GetTotalEnemies(const Castle &c)
	{
		return (c.towers[0].num_enemies + c.towers[1].num_enemies + c.towers[2].num_enemies + c.towers[3].num_enemies);
	}

	// destroy all lists
	void Destroy(Castle &c)
	{
		for (int i = A_REG; i < D_REG; i++)
			TOWER::Destroy(&c.towers[i]);
	}

	// has the war ended for this castle?
	bool HasFinished(Castle &c)
	{
		return (TOWER::HasFinished(c.towers[0]) && TOWER::HasFinished(c.towers[1]) && TOWER::HasFinished(c.towers[2]) && TOWER::HasFinished(c.towers[3]));
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
		assert(killed && "couldnt allocate memory for killed array");
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
				if (killed[j]->fight_delay < killed[i]->fight_delay)
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
			if (!TOWER::IsDestroyed(c.towers[nextTower]))
			{
				_Transfer(&c.towers[region], &c.towers[nextTower], SHLD_FITR, (REGION)nextTower);
				_Transfer(&c.towers[region], &c.towers[nextTower], FITR, (REGION)nextTower);
				return;
			}
		}
	}

	// move enemies from Tower 1 --> Tower 2
	void _Transfer(Tower* T1, Tower* T2, TYPE type, const REGION &Region)
	{
		//assert(!TOWER::IsEmpty(*T1) && "Attempt to move an empty tower");
		if (TOWER::IsEmpty(*T1))
			return;

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

		// special case
		// when tower2 is empty, make it point at tower1 list
		if (T2->firstShielded == NULL && type == SHLD_FITR)
		{
			int num = TOWER::GetNumOfShielded(T1);

			// fix all enemies before inserting
			for (Enemy* temp = T1->firstShielded; temp;temp = temp->next)
				_Fix_forTransfer(T2, temp, Region);

			T2->firstShielded = T1->firstShielded;
			T1->firstShielded = NULL;

			// update number of enemeies
			T1->num_enemies -= num;
			T2->num_enemies += num;

			return;
		}
		else if (T2->firstEnemy == NULL && type != SHLD_FITR)
		{
			int num = TOWER::GetNumOfNormal(T1);

			// fix all enemies before inserting
			for (Enemy* temp = T1->firstEnemy; temp;temp = temp->next)
				_Fix_forTransfer(T2, temp, Region);

			T2->firstEnemy = T1->firstEnemy;
			T1->firstEnemy = NULL;

			// update number of enemeies
			T1->num_enemies -= num;
			T2->num_enemies += num;

			return;
		}

		// iterate through all enemies in list1 till it is empty
		while (list1)
		{
			// to move it
			Enemy* temp = list1;

			// now list points at next one
			list1 = list1->next;

			// cut it from list1, if list1 became null you dont need to do that
			if (list1)
				list1->prev = nullptr;
			
			temp->next = temp->prev = nullptr;

			bool reached_the_end = false;
			// move list2 to the proper position
			// proper position: where this arrive_time <= the next one 
			while (temp->arrive_time > list2->arrive_time)	// while this is greater than the next
			{
				assert(list2 && "how this can be NULL?");
				// if this is the last one
				if (list2->next == nullptr)
				{
					// make a flag that we are trying to append at the end of the list
					// and break to prevent making list2 nullptr
					reached_the_end = true;
					break;
				}

				// move pointer to next
				list2 = list2->next;
			}


			// insert at the END
			if (reached_the_end)
				ENEMY::_InsertAfter(temp, list2);
			// insert before list2
			else
				ENEMY::_InsertBefore(temp, list2);

			// isert at BEGINNING
			if (temp->prev == nullptr)	// when temp is inserted at beginning, prev becomes NULL as it is the first enemy
			{
				if (type == SHLD_FITR)
					T2->firstShielded = temp;		
				else 
					T2->firstEnemy = temp;
			}
			
			//update enemy region and position if needed
			_Fix_forTransfer(T2, temp, Region);

			// update number of enemies in both towers
			T1->num_enemies--;
			T2->num_enemies++;
		}

		// now tower1 points at nothing
		if (type == SHLD_FITR)
			T1->firstShielded = NULL;
		else
			T1->firstEnemy = NULL;
	}

	// fixes enemy variables like distance and region after transfering
	// T is the tower transfered to
	void _Fix_forTransfer(Tower* T, Enemy *e, const REGION &Region)
	{
		e->Region = Region;

		if (e->Distance < T->unpaved)
			e->Distance = T->unpaved;
	}

	// delete all lists in tower
	void Destroy(Tower* T)
	{
		assert(T && "Attempt to destroy null tower");

		if (T->firstEnemy)
			ENEMY::Destroy(T->firstEnemy);

		if (T->firstShielded)
			ENEMY::Destroy(T->firstShielded);

		T->firstEnemy = T->firstShielded = nullptr;
	}

	// one tower will finish fighting when its destroyed or empty
	bool HasFinished(Tower &T)
	{
		return (IsEmpty(T) || IsDestroyed(T));
	}

	int GetNumOfShielded(Tower *T)
	{
		assert(T && "tower ptr is NULL");

		Enemy* temp = T->firstShielded;
		int num = 0;

		while (temp)
		{
			num++;
			temp = temp->next;
		}

		return num;
	}

	int GetNumOfNormal(Tower *T)
	{
		assert(T && "tower ptr is NULL");

		Enemy* temp = T->firstEnemy;
		int num = 0;

		while (temp)
		{
			num++;
			temp = temp->next;
		}

		return num;
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
        
        e->Distance = 60;
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
			ENEMY::Move(*e, *T);

			if (ENEMY::CanFire(e, timer))
				ENEMY::Fire(e, T);

			if (ENEMY::IsTank(e))
				TANK::Drop(e, T, timer);

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
		e.Distance -= e.speed;

		// don't let enemy get iside tower
		if (e.Distance < MIN_DISTANCE_FROM_CASTLE)
			e.Distance = MIN_DISTANCE_FROM_CASTLE;

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

	bool IsTank(Enemy *e)
	{
		return (e->Type == TANK_ENEM);
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
		assert(e->Distance && "enemy distance cant be zero, a try to divide on zero");

		e->Health -= ((1.0 / e->Distance) * t->fire_power * (1.0 / k));  

		// calc fight delay FD = Time - arrival time
		if (e->fight_delay == -1)	// firt time
		{
			e->fight_delay = time - e->arrive_time;  
			Log::total_FD += e->fight_delay;
		}
	}

	// enemy fires at given tower
	// paver pave with their fire power 
	void Fire(Enemy* e,Tower* t)
	{
		if (TOWER::IsDestroyed(*t))
			return;		// عشان الضرب ف الميت حرام

		if (ENEMY::IsPaver(*e))
			PAVER::Pave(e, t);
		else if (ENEMY::IsDoctor(e))
			DOCTOR::Heal(e);
		else
			TOWER::Damage(e, t);
	}

	// insert e1 before e2
	void _InsertBefore(Enemy* e1, Enemy* e2)
	{
		assert((e1 || e2) && "_insertBefore tried to insert a null enemy");

		// assume the enemy before e2 is e0
		Enemy* e0 = e2->prev;		

		// before inserting
		// e0 <-> e2
		//	   e1

		// change e1 pointers
		e1->next = e2;
		e1->prev = e0;
		//  e0 <-> e2
		//	 < e1 >

		// change e2 pointers
		e2->prev = e1;
			
		// change e0 pointers, if e0 exists
		if (e0)
			e0->next = e1;

		// after inserting
		// e0 <-> e1 <-> e2

		// if e0 is null, this will be the result
		// NULL <- e1 <-> e2
	}

	// insert e1 after e2 in list
	void _InsertAfter(Enemy* e1, Enemy* &e2)
	{
		// append at end
		e2->next = e1;

		e1->prev = e2;
		e1->next = nullptr;

		// update list2
		e2 = e1;
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

	char GetRegion(const int &region)
	{
		switch (region)
		{
			case 0:
				return 'A';
			case 1:
				return 'B';
			case 3:
				return 'D';
			case 2:
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

	bool IsDoctor(Enemy* e)
	{
		return (e->Type == DOC);	
	}

	Enemy* _Generate(const int &timer, const int &region, const int &distance)
	{
		srand (time(NULL));

		int id = ++Log::total_enemies_beg;
		int health = rand() % 300 + 1; 
		int type = rand() % 4;
		if (type == SHLD_FITR)
			type++;		// do not create shielded in this list
		if (type == TANK_ENEM)
			type--;
		int firePow = rand() % 100;
		int speed = rand() % 5 + 1;
		int reload = rand() % 8 + 1;
		
		Enemy* temp = ENEMY::Initialize(id, type, timer + 1, health, firePow, reload, speed, (REGION)region);
		temp->Distance = distance;

		return temp;
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
			assert(arr[i]->Distance && "distance is zero, a try to divide on zero");

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

namespace DOCTOR
{
	void Heal(Enemy* d)
	{
		// increase health of next and prev
		
		if (d->next && !ENEMY::IsTank(d->next))
			d->next->Health += d->fire_power / 2.0;

		if (d->prev && !ENEMY::IsTank(d->prev))
			d->prev->Health += d->fire_power / 2.0;
	}
}

namespace PAVER
{
	void Pave(Enemy* e, Tower* t)
	{
		if (e->Distance == t->unpaved && t->unpaved > 0)
		{
			// paves and moves to the beginnig of unpaved area
			t->unpaved = t->unpaved - e->fire_power;

			// correct it if it became smaller than zero
			if (t->unpaved < 0)
				t->unpaved = 0;

			e->Distance = t->unpaved;
			if (e->Distance == 0)
				e->Distance = MIN_DISTANCE_FROM_CASTLE;		// correct position to not let him enter tower
		}
	}
}

namespace TANK
{
	// genarates random enemies and add them to next of heli
	void Drop(Enemy* h, Tower* T, const int &timer)
	{
		assert(h && T && "tank or tower given to drop is NULL");

		if (!ENEMY::CanFire(h, timer))
			return;

		for (int i = 0; i < h->fire_power / (static_cast<double>(h->Distance)); i++, h->fire_power--)
		{
			Enemy* temp = ENEMY::_Generate(timer, h->Region, h->Distance);

			temp->prev = h;
			temp->next = h->next;

			if (h->next)
				h->next->prev = temp;

			h->next = temp;

			T->num_enemies++;
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
	int last_killed[NUM_OF_TOWERS] = {0, 0, 0, 0};
	int all_killed[NUM_OF_TOWERS] = {0, 0, 0, 0};


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
		outFile << "KTS" << space(DEFAULT - 2)
				<< "S"	 << space(DEFAULT)
				<< "FD"	 << space(DEFAULT)
				<< "KD"	 << space(DEFAULT)
				<< "FT\n";

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

		const int &KTS = time, &S = e->ID, &FD = e->fight_delay, &KD = e->kill_delay, &FT = KD + FD;

		outFile << KTS << space(DEFAULT) 
				<< S << space(DEFAULT) 
				<< FD << space(DEFAULT) 
				<< KD << space(DEFAULT) 
				<< FT << '\n';

		last_killed[e->Region] += 1;
		all_killed[e->Region] += 1;
	}

	// add towers data to file
	void ToFile(const Castle &c)
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

		outFile << "T1_Total_Damage"<< "  "
				<< "T2_Total_Damage" << "  "
				<< "T3_Total_Damage" << "  "
				<< "T4_Total_Damage\n";
		outFile << space(8) << T1_D << space(18)
				<< T2_D << space(18)
				<< T3_D << space(18)
				<< T4_D << '\n';

		// print unpaved
		outFile << "R1_Distance" << "  "
				<< "R2_Distance" << "  "
				<< "R3_Distance" << "  "
				<< "R4_Distance\n";

		outFile << space(8) << c.towers[0].unpaved << space(11)	
				<< c.towers[1].unpaved << space(11)
				<< c.towers[2].unpaved << space(16)
				<< c.towers[3].unpaved << '\n';
	}

	// end of file
	void End(const Castle &c)
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

			if (total_enemies_beg == 0)
			{
				outFile << "Average Fight Delay = " << "N/A" << '\n';
				outFile << "Average Kill Delay = " << "N/A" << '\n';
			}
			else
			{
				outFile << "Average Fight Delay = " << total_FD / static_cast<double>(total_enemies_beg) << '\n';
				outFile << "Average Kill Delay = " << total_KD / static_cast<double>(total_enemies_beg) << '\n';
			}
		}
		else if (CASTLE::IsDestroyed(c)) // LOOSE
		{
			/*a. Numberofkilledenemies
			b. Numberofaliveenemies(activeandinactive)
			c. Average “Fight Delay” and Average “Kill Delay” for killed enemies only*/
			outFile << "Game is LOST\n";

			outFile << "Number of killed enemies = " << total_killed << '\n';
			outFile << "Number of alive enemies = " << CASTLE::GetTotalEnemies(c) << '\n';

			if (total_killed == 0)	// no one was killed
			{
				outFile << "Average Fight Delay = " << "N/A" << '\n';
				outFile << "Average Kill Delay = " << "N/A" << '\n';
			}
			else
			{
				outFile << "Average Fight Delay = " << total_FD / static_cast<double>(total_killed) << '\n';
				outFile << "Average Kill Delay = " << total_KD / static_cast<double>(total_killed) << '\n';
			}
			
		}
			

		outFile.close();
	}

	// print tower information to screen
	void ToScreen(const Castle &c)
	{
		// print a line to user like this "Region  #Current enemies  #Last killed enemies  #All killed enemies  #Unpaved distance"
		// loop through towrs and print their data 
		
		cout << TO_CENTRE << "Region  #Current enemies  #Last killed enemies  #All killed enemies  #Unpaved distance\n";
		int align = 3;
		for (int region = A_REG; region <= D_REG; region++)
		{
			cout << TO_CENTRE << space(3) 
				 << ENEMY::GetRegion(region) << space(DEFAULT * align)
				 << c.towers[region].num_enemies << space(DEFAULT * align)
				 <<	last_killed[region] << space(DEFAULT * align + 6)
				 << all_killed[region] << space(DEFAULT * align + 6)
				 << c.towers[region].unpaved << '\n';

			last_killed[region] = 0;		// reset it for second time step
		}
	}
}