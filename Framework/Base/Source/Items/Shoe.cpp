#include "Shoe.h"
#include "MeshBuilder.h"
#include "EquipmentManager.h"
#include "Equipment.h"
#include "Inventory.h"

Shoe * Create::theShoe(const std::string & _meshName, string ID, string attack, string defense, string speed, bool isEquipped)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	Shoe* result = new Shoe(modelMesh);
	result->SetID(stoi(ID));
	result->SetAttack(stoi(attack));
	result->SetDef(stoi(defense));
	result->SetSpeed(stoi(speed));
	result->SetType(Equipment::SHOE);
	result->SetName(_meshName);
	if (isEquipped)
	{
		EquipmentManager::GetInstance()->AssignEquipment(result);
	}
	else
	{
		Inventory::GetInstance()->assign_storage(result);
	}
}
