#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include "SingletonTemplate.h"

class CAttributes : public Singleton <CAttributes>
{
public:
	CAttributes();
	~CAttributes();
	enum ATTRIBUTE_TYPES
	{
		TYPE_HEALTH = 0,
		TYPE_ATTACK,
		TYPE_DEFENSE,
		TYPE_LEVEL,
		TYPE_EXP,
		TYPE_MAXEXP,
		TYPE_MAXHEALTH,
		TYPE_SPEED,
		TYPE_GOLD,
	};

	//Getters
	/*
	Get Attribute's value base on attribute type
	Types:
		TYPE_HEALTH,
		TYPE_ATTACK,
		TYPE_DEFENSE,
		TYPE_LEVEL,
		TYPE_EXP,
		TYPE_MAXEXP,
		TYPE_MAXHEALTH,
		TYPE_GOLD,
	*/
	float getAttribute(ATTRIBUTE_TYPES type);

	//Setters
	/*
	Resets health to max
	*/
	void resetHealthToMax();
	/*
	Deduct health by amount specified in parameter
	*/
	void deductHealthBy(float _amount);
	/*
	Increase health by amount specified in parameter
	*/
	void increaseHealthBy(float _amount);
	/*
	Set health to amount specified in parameter
	*/
	void setHealthTo(float _amount);
	/*
	Increase level by one
	*/
	void levelUp();
	/*
	Increase amount of gold by amount specified in parameter
	*/
	void increaseGoldBy(int _amount);
	/*
	Sets Attack damage to amount specified in parameter
	*/
	void setAttackTo(int _amount);
	/*
	Sets Defense to amount specified in parameter
	*/
	void setDefenseTo(int _amount);
	/*
	Increase max health to amount specified in parameter
	*/
	void increaseMaxHealth(float _amount);
	/*
	Set Max Health to amount specified in parameter
	*/
	void setMaxHealthTo(float _amount);
	/*
	Sets attributes base on CAttribute specified in parameter
	*/
	void operator= (CAttributes newAttributes);

	//set the level
	void setLevel(int level);

	//set the speed
	void setSpeed(int speed);

	//set the gold
	void setGold(int gold);

	//set the MP
	void setMP(float mana);

	//set max MP
	void setMaxMP(float maxMana);

	//set current exp
	void setEXP(int exp);

	void printAttributes();

	void addAttack(int newAttack);
	void addDefence(int newDefence);
	void addSpeed(int newSpeed);

private:
	float MAX_HEALTH;			//To cap player's health / when reseting player's health to this
	float HEALTH;				//Current health
	float MAX_MANA;				//cap player's mana
	float MANA;					//current mana
	/*const float MAX_ARMOUR;
	float ARMOUR;*/
	int MAX_EXP;				//For player to level up once EXP >= MAX_EXP
	int EXP;					//Experience points
	int GOLD;					//currency
	int LEVEL;					//Level
	int ATTACK;					//Attack damage
	int DEFENSE;				//Defense points
	int SPEED;					//Speed points
};

#endif