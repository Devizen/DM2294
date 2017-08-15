#include "Armour.h"
#include "Inventory.h"
#include "MeshBuilder.h"
#include "EquipmentManager.h"
#include "Equipment.h"

Armour * Create::theArmour(const std::string & _meshName, string ID)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	Armour* result = new Armour(modelMesh);
	result->SetID(stoi(ID));
	result->SetType(Equipment::ARMOUR);
	result->SetName(_meshName);
	EquipmentManager::GetInstance()->AssignEquipment(result);
	Inventory::GetInstance()->assign_storage(result);
}
