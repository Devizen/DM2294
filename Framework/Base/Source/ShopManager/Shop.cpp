#include "Shop.h"
#include "ShopManager.h"

Shop * Create::ShopItems(const std::string& _meshName, string ID, string attack, string defense, string speed, int gold, int type)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	Shop* result = new Shop(modelMesh);
	result->SetID(stoi(ID));
	result->SetAttack(stoi(attack));
	result->SetDef(stoi(defense));
	result->SetSpeed(stoi(speed));
	result->SetType(type);
	result->SetName(_meshName);
	result->setGold(gold);
	ShopManager::GetInstance()->PushBackIntoList(result, type);
	return result;
}

void Shop::setGold(int gold)
{
	this->gold = gold;
}

int Shop::getGold()
{
	return gold;
}
