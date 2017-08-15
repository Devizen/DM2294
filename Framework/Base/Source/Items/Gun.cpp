#include "Gun.h"
#include "MeshBuilder.h"
#include "EquipmentManager.h"
#include "Equipment.h"
#include "Inventory.h"

Gun * Create::theGun(const std::string & _meshName, string ID)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	Gun* result = new Gun(modelMesh);
	result->SetID(stoi(ID));
	result->SetType(Equipment::GUN);
	result->SetName(_meshName);
	EquipmentManager::GetInstance()->AssignEquipment(result);
	Inventory::GetInstance()->assign_storage(result);
}
