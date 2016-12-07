#include "Data.h"

namespace TOWER
{
	void Damage(Tower*t,Enemy*E)                     //Damage to tower by enemy  
	{
		int k=1;
	if (E->Type=SHLD_FITR)
	{
		k=2;
	}
		
	t->Health=t->Health-(k/E->Distance)*E->fire_power;

	//if (c.towers[i].Health<=0)                            //if tower is destroyed

		
    }
	
	// move enemies from Tower 1 --> Tower 2
	void Transfer(Tower* T1, Tower* T2, Type type)
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

	// move enemies from destroyed 
	void Transfer(Castle &c, int region)
	{ 
		int nextTower = (region + 1) % NUM_OF_TOWERS;

		for (int i = 0; i < 3; nextTower = (nextTower + 1) % NUM_OF_TOWERS, i++)
		{
			if (c.towers[nextTower].Health > 0)
			{
				Transfer(c.towers[region], c.towers[nextTower], SHLD_FITR);
				Transfer(c.towers[region], c.towers[nextTower], FITR);
				break;
			}
		}
	}
	
}

namespace ENEMY
{

	// kill/remove enemy from the list and print its information
	void Killed(Enemy* e, Tower* t)      
	{
		Enemy* next = t->firstEnemy;
		Enemy* pre = NULL;

		if(next->Health <= 0){
			ENEMY::Print(*next);             //function to print all information about the enemy
			t->firstEnemy = next->next;
			delete next;
			return;
		}

		while(next != e)
		{
			pre = next;
			next = next->next;
		}

		ENEMY::Print(*next);
		pre->next=next->next;
		delete next;
	}

	//function to check if the enemy reloaded his weapon or not
	bool CanFire(Enemy* e, int time)                       
	{ 
		// special case
		if (e->reload_period == 0)
			return true;

		return ((time - e->arrive_time) % e->reload_period == 0);
	}

	//function to reduce enemy's health and check if the enemy is killed or not and remove it if true
	void Damage(Enemy* e,Tower* t)
	{
		// provided constant
		int k = 1;
		if (SHIELDED::IsShielded(e))
			k++;

		// update health
		e->Health -= ((1 / e->Distance) * t->fire_power * (1 / k));                               

		//function to kill/remove enemy from the list and print its information
		if(e->Health <= 0)
			Killed(e,t);
	}

	void Fire(Enemy* e,Tower* t){
		if (IsPaver(e))
			t->unpaved = t->unpaved - e->fire_power;
		else
			TOWER::Damage(t, e);
	}

	// insert temp before e2
	void _InsertBefore(Enemy* temp, Enemy* e2)
	{
		temp->next = e2;
			
		if (!e2 || !e2->prev)
			return;

		temp->prev = e2->prev;
		e2->prev->next = temp;

		e2->prev = temp;
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