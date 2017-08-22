/*Equipment was done by Lim Guan Sheng, Marcus (161725E)*/
/*Tested by Lai Wei Jing, Eugene, Chan Zhi Hao and Kwong Yu Ming*/

#pragma once
#include "Equipment.h"

class Glove : public Equipment
{
	Mesh* modelMesh;
public:
	Glove() {};
	Glove(Mesh* modelMesh) { this->modelMesh = modelMesh; };
	~Glove() {};

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
	Glove* theGlove(const std::string& _meshName, string ID, string attack, string defense, string speed, bool isEquipped);
};