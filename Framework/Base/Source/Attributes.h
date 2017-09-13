#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

class CAttributes
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
		TYPE_NONE
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
	int GetAttribute(ATTRIBUTE_TYPES type);

	//Setters
	/*
	Resets health to max
	*/
	void resetHealthToMax();
	/*
	Deduct health by amount specified in parameter
	*/
	void deductHealthBy(int _amount);
	/*
	Increase health by amount specified in parameter
	*/
	void increaseHealthBy(int _amount);
	/*
	Set health to amount specified in parameter
	*/
	void setHealthTo(int _amount);
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
	void increaseMaxHealth(int _amount);
	/*
	Set Max Health to amount specified in parameter
	*/
	void setMaxHealthTo(int _amount);
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
	void setMP(int mana);

	//set max MP
	void setMaxMP(int maxMana);

	//set current exp
	void setEXP(int exp);

	/*Set max experience points.*/
	void SetMaxEXP(int _maxEXP);

	void printAttributes();

	void addAttack(int newAttack);
	void addDefence(int newDefence);
	void addSpeed(int newSpeed);

private:
	int MAX_HEALTH;			//To cap player's health / when reseting player's health to this
	int HEALTH;				//Current health
	int MAX_MANA;				//cap player's mana
	int MANA;					//current mana
	/*const int MAX_ARMOUR;
	int ARMOUR;*/
	int MAX_EXP;				//For player to level up once EXP >= MAX_EXP
	int EXP;					//Experience points
	int GOLD;					//currency
	int LEVEL;					//Level
	int ATTACK;					//Attack damage
	int DEFENSE;				//Defense points
	int SPEED;					//Speed points
};

#endif