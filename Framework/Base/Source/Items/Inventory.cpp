#include "Inventory.h"
#include "Equipment.h"
#include <iostream>

using std::cout;
using std::endl;

void Inventory::Init()
{
	for (int i = 0; i < 12; i++)
	{
		storage[i] = NULL;
		continue;
	}
}

void Inventory::assign_storage(Equipment* object)
{
	static int count = 0;
	for (int i = 0; i < 12; i++)
	{
		if (storage[i] == NULL)
		{
			storage[i] = object;
			count++;
			cout << count << endl;
			break;
		}
	}
}

Equipment** Inventory::ReturnType()
{
	return storage;
}

void Inventory::remove_storage(int position)
{
	//for (auto it = Inventory::GetInstance()->storage.begin(); it != Inventory::GetInstance()->storage.end();++it)
	//{
	//	if (position != count)
	//	{
	//		++count;
	//		continue;
	//	}
	//	Equipment* find = (Equipment*)*it;
	//	cout << "Address in Inventory: " << find << endl;
	//	storage.remove(find);
	//	delete find;
	//	find = nullptr;
	//	//Inventory::GetInstance()->storage.erase(it);
	//	//storage.pop_back();
	//	break;
	//}
	for (int i = position; i < 12 - 1; i++)
	{
		Equipment* temp = storage[i + 1];
		storage[i] = temp;
	}
	storage[11] = NULL;
}
