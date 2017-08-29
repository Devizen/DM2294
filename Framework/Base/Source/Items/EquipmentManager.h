/*Equipment was done by Lim Guan Sheng, Marcus (161725E)*/
/*Tested by Lai Wei Jing, Eugene, Chan Zhi Hao and Kwong Yu Ming*/

#pragma once
#include "SingletonTemplate.h"
#include "Equipment.h"
#include <list>

using std::list;
class Equipment;

class EquipmentManager : public Singleton <EquipmentManager>
{
	friend Singleton<EquipmentManager>;
	Equipment* EqList[6];

	int pressCountX;
	int pressCountY;
	bool showText;
public:
	void AssignEquipment(Equipment* newEQ);
	void UnEquip(int countX, int countY);
	void AddAttributes();
	Equipment** ReturnList();

	void Init();
	void Update(double dt);
	void Render();
	double textTime;

	void merge_sort(Equipment* data_set[], int start_index, int end_index);
	void merge(Equipment* data_set[], int start_index, int middle_index, int end_index);
};
