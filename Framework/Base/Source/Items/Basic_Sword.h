/*Equipment was done by Lim Guan Sheng, Marcus (161725E)*/
/*Tested by Lai Wei Jing, Eugene, Chan Zhi Hao and Kwong Yu Ming*/

#pragma once
#include "Weapon.h"

class Basic_Sword : public Weapon
{
	Mesh* modelMesh;
public:
	Basic_Sword() {};
	Basic_Sword(Mesh* modelMesh) { this->modelMesh = modelMesh; };
	~Basic_Sword() {};

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
	Basic_Sword* theSword(const std::string& _meshName, string ID, string attack, bool isEquipped);
};