/*Equipment was done by Lim Guan Sheng, Marcus (161725E)*/
/*Tested by Lai Wei Jing, Eugene, Chan Zhi Hao and Kwong Yu Ming*/

#include "Helmet.h"
#include "MeshBuilder.h"
#include "EquipmentManager.h"
#include "Equipment.h"
#include "Inventory.h"

Helmet * Create::theHelmet(const std::string & _meshName, string ID, string attack, string defense, string speed, bool isEquipped)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	Helmet* result = new Helmet(modelMesh);
	result->SetID(stoi(ID));
	result->SetAttack(stoi(attack));
	result->SetDef(stoi(defense));
	result->SetSpeed(stoi(speed));
	result->SetType(Equipment::HELMET);
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
