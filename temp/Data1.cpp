#include "Data.h"

namespace TOWER
{
	// damage to tower by enemy 
	// decrease its health
	void Damage(Enemy* e, Tower* t)                 
	{
		// provided constant
		int k = 1;
		if (SHIELDED::IsShielded(e))
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
				_Transfer(c.towers[region], c.towers[nextTower], SHLD_FITR);
				_Transfer(c.towers[region], c.towers[nextTower], FITR);
				return;
			}
		}
	}

	// move enemies from Tower 1 --> Tower 2
	void _Transfer(Tower* T1, Tower* T2, Type type)
	{
		Enemy* list1 = nullptr, Enemy* list2 = nullptr;
		// choose the list to transfer
		switch (type):
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

			ENEMY::_InsertBefore(temp, list2)

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
	
}

namespace ENEMY
{

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
		int k = 1;
		if (SHIELDED::IsShielded(e))
			k++;

		// update health
		e->Health -= ((1 / e->Distance) * t->fire_power * (1 / k));  

		// calc fight delay FD = Time - arrival time
		if (e->fight_delay == -1)	// firt time
			e->fight_delay = time - e->arrive_time;                             
	}

	// enemy fires at given tower
	// paver pave with their fire power 
	void Fire(Enemy* e,Tower* t)
	{
		if (IsPaver(e))
		{
			// paves and moves to the beginnig of unpaved area
			t->unpaved = t->unpaved - e->fire_power;
			e->distance = t->unpaved;
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

	// void modify(Enemy*list,Tower*t)
	
    //  {
	// 	 while (list!=nullptr)
	// 	 {
	// 		 if (list->Distance < t->unpaved)
	// 		 list->Distance=t->unpaved;

	// 		 list=list->next;
	// 	 }

	//  }

}