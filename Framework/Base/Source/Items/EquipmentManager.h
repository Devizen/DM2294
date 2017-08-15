#pragma once
#include "SingletonTemplate.h"
#include "Equipment.h"
#include <list>

using std::list;
class Equipment;

class EquipmentManager : public Singleton <EquipmentManager>
{
	friend Singleton<EquipmentManager>;
	list<Equipment*> EqList;
public:
	void AssignEquipment(Equipment* newEQ);
	list<Equipment*>&getEqList(void);
};
