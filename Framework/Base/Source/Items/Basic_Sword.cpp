#include "Basic_Sword.h"
#include "MeshBuilder.h"
#include "EquipmentManager.h"
#include "Equipment.h"
#include "Inventory.h"

Basic_Sword * Create::theSword(const std::string & _meshName, string ID)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	Basic_Sword* result = new Basic_Sword(modelMesh);
	result->SetID(stoi(ID));
	result->SetType(Equipment::SWORD);
	result->SetName(_meshName);
	EquipmentManager::GetInstance()->AssignEquipment(result);
	Inventory::GetInstance()->assign_storage(result);
}