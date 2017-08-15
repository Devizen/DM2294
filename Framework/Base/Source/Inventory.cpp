#include "Items\Inventory.h"
#include "Items\Equipment.h"
#include <iostream>

using std::cout;
using std::endl;

void Inventory::assign_storage(Equipment* object)
{
	for (int i = 0; i < 12; i++)
	{
		if (storage[i] == NULL)
		{
			storage[i] = object;
			cout << "Success" << endl;

			break;
		}
	}

}

void Inventory::Init()
{
	for (int i = 0; i < 12; i++)
	{
		storage[i] = NULL;
		continue;
	}
}

Equipment** Inventory::ReturnType()
{
	return storage;
}

void Inventory::remove_storage(int position)
{
	int count = 0;
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
	storage[position] = NULL;
	for (int i = position; i < 12 - 1; i++)
	{
		Equipment* temp = storage[i + 1];
		storage[i] = temp;
	}
}
