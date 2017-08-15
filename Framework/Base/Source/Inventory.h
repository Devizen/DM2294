#pragma once
#include "Items\Equipment.h"
#include "SingletonTemplate.h"
#include <list>

using std::list;

class Equipment;

class Inventory : public Singleton <Inventory>
{
	Equipment* storage[12];
public:
	Inventory() {};
	~Inventory() {};

	void Init();
	Equipment** ReturnType();

	void assign_storage(Equipment* object);
	void remove_storage(int position);

	bool empty;
};