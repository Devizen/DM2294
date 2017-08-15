#pragma once
#include <string>
#include "SingletonTemplate.h"
#include "Mesh.h"

using std::string;

class Equipment : public Singleton <Equipment>
{
public:
	Equipment() : attack(0), defense(0), speed(0), id(0), quality(0), type(0) {} ;
	~Equipment() {};

	virtual int GetAttack() = 0;
	virtual int GetDefense() = 0;
	virtual int GetSpeed() = 0;
	virtual int GetID() = 0;
	virtual int GetType() = 0;
	virtual string getName() = 0;

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
	int quality;
	int type;
	string name;
};