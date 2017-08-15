#include "Helmet.h"
#include "MeshBuilder.h"
#include "EquipmentManager.h"
#include "Equipment.h"
#include "Inventory.h"

Helmet * Create::theHelmet(const std::string & _meshName, string ID)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	Helmet* result = new Helmet(modelMesh);
	result->SetID(stoi(ID));
	result->SetType(Equipment::HELMET);
	result->SetName(_meshName);
	EquipmentManager::GetInstance()->AssignEquipment(result);
	Inventory::GetInstance()->assign_storage(result);
}
