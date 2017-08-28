#include "Attributes.h"
#include <iostream>

using std::cout;
using std::endl;

CAttributes::CAttributes()
	: MAX_HEALTH(100)
	, HEALTH(100)
	, ATTACK(1)
	, DEFENSE(0)
	, GOLD(0)
	, EXP(0)
	, MAX_EXP(1)
	, LEVEL(1)
	,SPEED(10)
{
}

CAttributes::~CAttributes()
{
}
/*
Returns Attribute's value base on attribute type specified
Types:
-TYPE_HEALTH
-TYPE_ATTACK
-TYPE_DEFENSE
-TYPE_LEVEL
-TYPE_EXP
-TYPE_MAXEXP
-TYPE_MAXHEALTH
-TYPE_GOLD
*/
int CAttributes::GetAttribute(ATTRIBUTE_TYPES type)
{
	switch (type)
	{
	case TYPE_HEALTH:
		return this->HEALTH;
	case TYPE_ATTACK:
		return this->ATTACK;
	case TYPE_DEFENSE:
		return this->DEFENSE;
	case TYPE_LEVEL:
		return this->LEVEL;
	case TYPE_EXP:
		return this->EXP;
	case TYPE_MAXEXP:
		return this->MAX_EXP;
	case TYPE_MAXHEALTH:
		return this->MAX_HEALTH;
	case TYPE_SPEED:
		return this->SPEED;
	case TYPE_GOLD:
		return this->GOLD;
	}
	return NULL;
}
/*
Resets health to max
*/
void CAttributes::resetHealthToMax()
{
	this->HEALTH = MAX_HEALTH;
}
/*
Deduct health by amount specified in parameter
*/
void CAttributes::deductHealthBy(int amount)
{
	this->HEALTH -= amount;
}
/*
Increase health by amount specified in parameter
*/
void CAttributes::increaseHealthBy(int _amount)
{
	this->HEALTH += _amount;
}
/*
Set health to amount specified in parameter
*/
void CAttributes::setHealthTo(int _amount)
{
	HEALTH = _amount;
}
/*
Increase level by one
*/
void CAttributes::levelUp()
{
	this->LEVEL += 1;
}
/*
Increase amount of gold by amount specified in parameter
*/
void CAttributes::increaseGoldBy(int _amount)
{
	this->GOLD += _amount;
}
/*
Sets Attack damage to amount specified in parameter
*/
void CAttributes::setAttackTo(int _amount)
{
	this->ATTACK = _amount;
}
/*
Sets Defense to amount specified in parameter
*/
void CAttributes::setDefenseTo(int _amount)
{
	this->DEFENSE = _amount;
}
/*
Increase max health to amount specified in parameter
*/
void CAttributes::increaseMaxHealth(int _amount)
{
	this->MAX_HEALTH = _amount;
}
/*
Set Max Health to amount specified in parameter
*/
void CAttributes::setMaxHealthTo(int _amount)
{
	MAX_HEALTH = _amount;
}
/*
Sets attributes base on CAttribute specified in parameter
*/
void CAttributes::operator=(CAttributes newAttributes)
{
	this->ATTACK = newAttributes.GetAttribute(TYPE_ATTACK);
	this->DEFENSE = newAttributes.GetAttribute(TYPE_DEFENSE);
	this->HEALTH = newAttributes.GetAttribute(TYPE_HEALTH);
	this->MAX_EXP = newAttributes.GetAttribute(TYPE_MAXEXP);
	this->MAX_HEALTH = newAttributes.GetAttribute(TYPE_MAXHEALTH);
	this->GOLD = newAttributes.GetAttribute(TYPE_GOLD);
	this->EXP = newAttributes.GetAttribute(TYPE_EXP);
	this->LEVEL = newAttributes.GetAttribute(TYPE_LEVEL);
}

void CAttributes::setLevel(int level)
{
	LEVEL = level;
}

void CAttributes::setSpeed(int speed)
{
	SPEED = speed;
}

void CAttributes::setGold(int gold)
{
	GOLD = gold;
}

void CAttributes::setMP(int mana)
{
	MANA = mana;
}

void CAttributes::setMaxMP(int maxMana)
{
	MAX_MANA = maxMana;
}

void CAttributes::setEXP(int exp)
{
	EXP = exp;
}

void CAttributes::printAttributes()
{
	//cout << "Health: " << HEALTH << endl;
	//cout << "Attack: " << ATTACK << endl;
	//cout << "Defense: " << DEFENSE << endl;
	//cout << "Speed: " << SPEED << endl;
	//cout << endl;
}

void CAttributes::addAttack(int newAttack)
{
	ATTACK += newAttack;
}

void CAttributes::addDefence(int newDefence)
{
	DEFENSE += newDefence;
}

void CAttributes::addSpeed(int newSpeed)
{
	SPEED += newSpeed;
}
