#include "Glove.h"
#include "MeshBuilder.h"
#include "EquipmentManager.h"
#include "Equipment.h"
#include "Inventory.h"


/*Debugging Purposes*/
#include <iostream>

using std::cout;
using std::endl;

Glove * Create::theGlove(const std::string & _meshName, string ID, string attack, string defense, string speed, bool isEquipped)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	Glove* result = new Glove(modelMesh);
	result->SetID(stoi(ID));
	result->SetAttack(stoi(attack));
	result->SetDef(stoi(defense));
	result->SetSpeed(stoi(speed)); 
	result->SetType(Equipment::GLOVE);
	result->SetName(_meshName);
	cout << "Address in Create: " << result << endl;
	if (isEquipped)
	{
		EquipmentManager::GetInstance()->AssignEquipment(result);
	}
	else
	{
		Inventory::GetInstance()->assign_storage(result);
	}
}
