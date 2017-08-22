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

	double DiscardTextTime;
	double EquipTextTime;
public:

	Inventory() {};
	~Inventory() {};

	void Init();
	void Update(double dt);
	Equipment** ReturnType();

	void assign_storage(Equipment* object);
	void remove_storage(int position);
	void RenderWeapon();
	void DeleteWeapon();
	void EquipWeapon();
	bool empty;
};
