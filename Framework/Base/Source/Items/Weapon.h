#pragma once
#include "Equipment.h"

class Weapon : public Equipment
{
public:
	Weapon() {};
	~Weapon() {};

	virtual int GetAttack() = 0;
	virtual int GetDefense() = 0;
	virtual int GetSpeed() = 0;
	virtual int GetID() = 0;
	virtual int GetType() = 0;
	virtual string getName() = 0;
	virtual bool getEquippedStatus() = 0;

};