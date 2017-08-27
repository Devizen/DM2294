/*Equipment was done by Lim Guan Sheng, Marcus (161725E)*/
/*Tested by Lai Wei Jing, Eugene, Chan Zhi Hao and Kwong Yu Ming*/

#include "Gun.h"
#include "MeshBuilder.h"
#include "EquipmentManager.h"
#include "Equipment.h"
#include "Inventory.h"

Gun * Create::theGun(const std::string & _meshName, string ID, string attack, bool isEquipped)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	Gun* result = new Gun(modelMesh);
	result->SetID(stoi(ID));
	result->SetAttack(stoi(attack));
	result->SetDef(0);
	result->SetSpeed(0);
	result->SetType(Equipment::GUN);
	result->SetName(_meshName);
	if (isEquipped)
	{
		EquipmentManager::GetInstance()->AssignEquipment(result);
	}
	else
	{
		Inventory::GetInstance()->assign_storage(result);
	}
	return result;

}
