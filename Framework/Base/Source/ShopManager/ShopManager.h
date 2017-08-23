#pragma once
#include "SingletonTemplate.h"
#include <list>
#include "Shop.h"
#include <string>
#include <vector>
#include "..//Items/Equipment.h"
#include "..//FileManager.h"

using std::vector;
using std::string;
using std::list;

class ShopManager : public Singleton<ShopManager>
{
	Equipment* helmList[3];
	Equipment* armorList[3];
	Equipment* gloveList[3];
	Equipment* shoeList[3];
	Equipment* swordList[3];
	Equipment* gunList[3];
	Equipment* temp[3];

	string name;
	int pressCountX;
	int pressCountY;
public: 
	ShopManager() {};
	~ShopManager() {};

	void init();
	void update(double dt);
	void render();

	void setList();
	void PushBackIntoList(Equipment* result, int type);
	void ArrangeVector(int type);

	Equipment** getHelmVector() { return helmList; }
	Equipment** getArmorVector() { return armorList; }
	Equipment** getGloveVector() { return gloveList; }
	Equipment** getShoeVector() { return shoeList; }
	Equipment** getSwordVector() { return swordList; }
	Equipment** getGunVector() { return gunList; }
};