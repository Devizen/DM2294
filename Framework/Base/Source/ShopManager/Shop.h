#pragma once
#include "SingletonTemplate.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include <string>
#include "..//Items/Equipment.h"

using std::string;


class Shop : public Singleton<Shop>, public Equipment
{
	Mesh* mesh;
	int gold;

public:
	Shop() {};
	Shop(Mesh* meshName) { this->mesh = meshName; }
	~Shop() {};
	void setGold(int gold);
	virtual int getGold();
	virtual int GetAttack() { return attack; };
	virtual int GetDefense() { return defense; };
	virtual int GetSpeed() { return speed; };
	virtual int GetID() { return id; };
	virtual int GetType() { return type; };
	virtual string getName() { return name; };
	virtual int getEquippedStatus()
	{
		if (isEquipped == true)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	};

	void SetAttack(int newAtt) { attack = newAtt; }
	void SetDef(int newDef) { defense = newDef; }
	void SetSpeed(int newSpd) { speed = newSpd; }
	void SetID(int newID) { id = newID; }
	void SetType(int newType) { type = newType; }
	void SetName(string newName) { name = newName; }
	void setStatus(bool newStat) { isEquipped = newStat; };
};

namespace Create
{
	Shop* ShopItems(const std::string& _meshName, string ID, string attack, string defense, string speed, int gold, int type);
}