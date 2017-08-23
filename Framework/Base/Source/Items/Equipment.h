/*Equipment was done by Lim Guan Sheng, Marcus (161725E)*/
/*Tested by Lai Wei Jing, Eugene, Chan Zhi Hao and Kwong Yu Ming*/

#pragma once
#include <string>
#include "SingletonTemplate.h"
#include "Mesh.h"

using std::string;

class Equipment : public Singleton <Equipment>
{
public:
	Equipment() : attack(0), defense(0), speed(0), id(0), quantity(0), type(0) {} ;
	~Equipment() {};

	virtual int GetAttack() = 0;
	virtual int GetDefense() = 0;
	virtual int GetSpeed() = 0;
	virtual int GetID() = 0;
	virtual int GetType() = 0;
	virtual string getName() = 0;
	virtual int getEquippedStatus() = 0;
	virtual int getGold() = 0;

	virtual void setStatus(bool newStat) = 0;

	enum EQUIPMENT_TYPE
	{
		HELMET = 0,
		ARMOUR,
		GLOVE,
		SHOE,
		SWORD,
		GUN,
		HP_POTION,
		MP_POTION
	};

protected:
	int attack;
	int defense;
	int speed;
	int id;
	int quantity;
	int type;
	string name;
	bool isEquipped;
};