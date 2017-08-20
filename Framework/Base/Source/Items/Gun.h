#pragma once
#include "Equipment.h"

class Gun : public Equipment
{
public: 
	Mesh* modelMesh;
public:
	Gun() {};
	Gun(Mesh* modelMesh) { this->modelMesh = modelMesh; };
	~Gun() {};

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
	Gun* theGun(const std::string& _meshName, string ID, string attack, bool isEquipped);
};