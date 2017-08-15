#include "Shoe.h"
#include "MeshBuilder.h"
#include "EquipmentManager.h"
#include "Equipment.h"
#include "Inventory.h"

Shoe * Create::theShoe(const std::string & _meshName, string ID)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	Shoe* result = new Shoe(modelMesh);
	result->SetID(stoi(ID));
	result->SetType(Equipment::SHOE);
	result->SetName(_meshName);
	EquipmentManager::GetInstance()->AssignEquipment(result);
	Inventory::GetInstance()->assign_storage(result);
}
