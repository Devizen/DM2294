/*Inventory was done by Lim Guan Sheng, Marcus (161725E)*/
/*Tested by Lai Wei Jing, Eugene, Chan Zhi Hao and Kwong Yu Ming*/

#pragma once
#include "Equipment.h"
#include "SingletonTemplate.h"
#include <list>

using std::list;

class Equipment;

class Inventory : public Singleton <Inventory>
{
	friend Singleton <Inventory>;
	Equipment* storage[12];

	int pressCountX;
	int pressCountY;

	bool showDiscardText;
	bool showEquipText;


public:

	Inventory() {};
	~Inventory() {};
	double DiscardTextTime;
	double EquipTextTime;
	void Init();
	void Update(double dt);
	Equipment** ReturnType();

	void assign_storage(Equipment* object);
	void remove_storage(int position);
	void RenderWeapon();
	void DeleteWeapon();
	void EquipWeapon();
	bool empty;
	bool currentEquipped;
};
