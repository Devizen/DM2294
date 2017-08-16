#include "EquipmentManager.h"


void EquipmentManager::AssignEquipment(Equipment * newEQ)
{
	for (int i = 0; i < 12; i++)
	{
		if (EqList[i] == NULL)
		{
			EqList[i] = newEQ;
			break;
		}
	}
}

Equipment ** EquipmentManager::ReturnList()
{
	return EqList;
}
