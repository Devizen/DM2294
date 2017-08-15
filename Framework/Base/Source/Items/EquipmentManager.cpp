#include "EquipmentManager.h"


void EquipmentManager::AssignEquipment(Equipment * newEQ)
{
	EqList.push_back(newEQ);
}

list<Equipment*>& EquipmentManager::getEqList(void)
{
	return EqList;
}

