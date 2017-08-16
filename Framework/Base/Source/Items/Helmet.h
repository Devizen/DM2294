#pragma once
#include "Equipment.h"

class Helmet : public Equipment
{
	Mesh* modelMesh;
public:
	Helmet() {};
	Helmet(Mesh* modelMesh) { this->modelMesh = modelMesh; };
	~Helmet() {};

	virtual int GetAttack() { return attack; };
	virtual int GetDefense() { return defense; };
	virtual int GetSpeed() { return speed; };
	virtual int GetID() { return id; };
	virtual int GetType() { return type; };
	virtual string getName() { return name; };
	virtual bool getEquippedStatus() { return isEquipped; };

	void SetAttack(int newAtt) { attack = newAtt; }
	void SetDef(int newDef) { defense = newDef; }
	void SetSpeed(int newSpd) { speed = newSpd; }
	void SetID(int newID) { id = newID; }
	void SetType(int newType) { type = newType; }
	void SetName(string newName) { name = newName; }
};

namespace Create
{
	Helmet* theHelmet(const std::string& _meshName, string ID, string attack, string defense, string speed, bool isEquipped);
};