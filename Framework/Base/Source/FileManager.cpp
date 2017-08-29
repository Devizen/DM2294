/*File Manager was done by Lim Guan Sheng, Marcus (161725E)*/
/*Tested by Lai Wei Jing, Eugene, Chan Zhi Hao and Kwong Yu Ming*/

#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Vector3.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "Items\Inventory.h"
#include "RenderHelper.h"
#include "KeyboardController.h"

#include "Items\Armour.h"
#include "Items\Basic_Sword.h"
#include "Items\Glove.h"
#include "Items\Gun.h"
#include "Items\Helmet.h"
#include "Items\Shoe.h"
#include "Attributes.h"
#include "Items\EquipmentManager.h"

#include "PlayerInfo\PlayerInfo.h"
#include "EntityManager.h"
#include "Object\Furniture.h"
#include "Enemy\AnimatedEnemy\AnimatedEnemy.h"
#include "Enemy\Enemy3D.h"
#include "Enemy\Horde\Horde.h"
#include "Enemy\Tower\Tower.h"
#include "Map_Editor\Map_Editor.h"
#include "ShopManager\ShopManager.h"
#include "ShopManager\Shop.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::getline;

void FileManager::init()
{

}

bool FileManager::ReadPlayerFile(const string myFile)
{
	ifstream file(myFile);
	static int count = 0;
	string data = "";
	string tempData = "";
	int nextData = 0;
	bool skipFirstLine = false;

	if (file.is_open())
	{
		while (getline(file, data))
		{
			if (!skipFirstLine)
			{
				skipFirstLine = true;
				continue;
			}

			for (unsigned int i = 0; i < data.size(); ++i)
			{
				if (nextData == 0)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					//add to player hp
					CPlayerInfo::GetInstance()->setHealthTo(stoi(tempData));
				}

				else if (nextData == 1)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					//add to player MP
					CPlayerInfo::GetInstance()->setMP(stoi(tempData));
				}

				else if (nextData == 2)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					//add to player Attack
					CPlayerInfo::GetInstance()->setAttackTo(stoi(tempData));
				}

				else if (nextData == 3)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					//add to player Def
					CPlayerInfo::GetInstance()->setDefenseTo(stoi(tempData));
				}

				else if (nextData == 4)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					//add to player speed
					CPlayerInfo::GetInstance()->setSpeed(stoi(tempData));
				}

				else if (nextData == 5)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					//add to player Gold
					CPlayerInfo::GetInstance()->setGold(stoi(tempData));
				}

				else if (nextData == 6)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					//add to player Level
					CPlayerInfo::GetInstance()->setLevel(stoi(tempData));
				}

				else if (nextData == 7)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					//add to player exp
					CPlayerInfo::GetInstance()->setEXP(stoi(tempData));
				}

				++nextData;
			}
			nextData = 0;
		}
		return true;
	}

	return false;
}

bool FileManager::ReadWeaponFile( string myFile)
{
	ifstream file;
	static int count = 0;
	string data = "";
	string tempData = "";
	int nextData = 0;
	bool skipFirstLine = false;
	file.open(myFile);

	if (file.is_open())
	{
		while (getline(file, data))
		{
			if (!skipFirstLine)
			{
				skipFirstLine = true;
				continue;
			}

			for (unsigned int i = 0; i < data.size(); ++i)
			{
				if (nextData == 0)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theEQInfo.name = tempData;
				}

				else if (nextData == 1)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}

					theEQInfo.eqAtk = tempData;
				}

				else if (nextData == 2)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}

					theEQInfo.eqDef = tempData;
				}

				else if (nextData == 3)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}

					theEQInfo.eqSpeed = tempData;
				}

				else if (nextData == 4)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}

					theEQInfo.eqId = tempData;
				}

				else if (nextData == 5)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theEQInfo.eqType = tempData;
				}

				else if (nextData == 6)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theEQInfo.isEquipped = static_cast<bool>(stoi(tempData));
				}

				tempData = "";
				++nextData;
			}
			masterList.push_back(theEQInfo);
			nextData = 0;
			tempData = "";
		}
		return true;
	}
	else
	{
		return false;
	}
	return true;

	if (masterList.size() > 0)
	{
		CreateWeapon();
		CPlayerInfo::GetInstance()->resetAttribute();
		EquipmentManager::GetInstance()->AddAttributes();
	}
}

bool FileManager::ReadMapFile(const string myFile)
{
	ifstream file;
	static int count = 0;
	string data = "";
	string tempData = "";
	int nextData = 0;
	bool skipFirstLine = false;
	file.open(myFile);

	if (file.is_open())
	{
		while (getline(file, data))
		{
			if (!skipFirstLine)
			{
				skipFirstLine = true;
				continue;
			}

			for (unsigned int i = 0; i < data.size(); ++i)
			{
				if (nextData == 0)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.type = tempData;
				}

				else if (nextData == 1)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.posX = stof(tempData);
				}

				else if (nextData == 2)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.posY = stof(tempData);
				}

				else if (nextData == 3)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.posZ = stof(tempData);
				}

				else if (nextData == 4)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.minAABBx = stof(tempData);
				}

				else if (nextData == 5)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.minAABBy = stof(tempData);
				}

				else if (nextData == 6)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.minAABBz = stof(tempData);
				}

				else if (nextData == 7)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.maxAABBx = stof(tempData);
				}

				else if (nextData == 8)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.maxAABBy = stof(tempData);
				}

				else if (nextData == 9)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.maxAABBz = stof(tempData);
				}

				else if (nextData == 10)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.scalex = stof(tempData);
				}

				else if (nextData == 11)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.scaley = stof(tempData);
				}

				else if (nextData == 12)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.scalez = stof(tempData);
				}

				else if (nextData == 13)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.rotateAngle = stof(tempData);
				}

				nextData += 1;
				tempData = "";
			}
			CFurniture* temp = Create::Furniture(theOBJinfo.type, Vector3(theOBJinfo.posX, theOBJinfo.posY, theOBJinfo.posZ), Vector3(theOBJinfo.scalex, theOBJinfo.scaley, theOBJinfo.scalez));
			temp->SetAABB(Vector3(theOBJinfo.maxAABBx, theOBJinfo.maxAABBy, theOBJinfo.maxAABBz), Vector3(theOBJinfo.minAABBx, theOBJinfo.minAABBy, theOBJinfo.minAABBz));
			temp->SetRotate(theOBJinfo.rotateAngle);
			nextData = 0;
		}
		return true;
	}

	return false;
}

bool FileManager::ReadEnemyFile(const string myFile)
{
	ifstream file;
	static int count = 0;
	string data = "";
	string tempData = "";
	int nextData = 0;
	bool skipFirstLine = false;
	file.open(myFile);

	if (file.is_open())
	{
		while (getline(file, data))
		{
			if (!skipFirstLine)
			{
				skipFirstLine = true;
				continue;
			}

			for (unsigned int i = 0; i < data.size(); ++i)
			{
				if (nextData == 0)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theEnemyInfo.type = stoi(tempData);
				}

				else if (nextData == 1)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theEnemyInfo.displacementX = stof(tempData);
				}

				else if (nextData == 2)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theEnemyInfo.displacementY = stof(tempData);
				}

				else if (nextData == 3)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theEnemyInfo.displacementZ = stof(tempData);
				}

				else if (nextData == 4)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theEnemyInfo.scaleX= stof(tempData);
				}

				else if (nextData == 5)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theEnemyInfo.scaleY = stof(tempData);
				}

				else if (nextData == 6)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theEnemyInfo.scaleZ = stof(tempData);
				}

				else if (nextData == 7)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theEnemyInfo.scaleZ = stof(tempData);
				}

				tempData = "";
				++nextData;
			}

			if (theEnemyInfo.type != 0)
			{
				if (theEnemyInfo.type == 1)
				{
					Create::AnimatedEnemy("ROBOT_CORE", "ROBOT_LeftArm", "ROBOT_RightArm", "ROBOT_LeftLeg", "ROBOT_RightLeg", "ROBOT_Head",
						Vector3(theEnemyInfo.displacementX, theEnemyInfo.displacementY, theEnemyInfo.displacementZ),
						Vector3(theEnemyInfo.scaleX, theEnemyInfo.scaleY, theEnemyInfo.scaleZ));
				}

				else if (theEnemyInfo.type == 2)
				{
					Create::Horde("ROBOT",
						Vector3(theEnemyInfo.displacementX, theEnemyInfo.displacementY, theEnemyInfo.displacementZ),
						Vector3(theEnemyInfo.scaleX, theEnemyInfo.scaleY, theEnemyInfo.scaleZ));
				}

				else if (theEnemyInfo.type == 3)
				{
					Create::Enemy3D("turret", Vector3(theEnemyInfo.displacementX, theEnemyInfo.displacementY, theEnemyInfo.displacementZ),
						Vector3(theEnemyInfo.scaleX, theEnemyInfo.scaleY, theEnemyInfo.scaleZ));
				}
				else if (theEnemyInfo.type == 4)
				{
					Create::Tower("TOWER", Vector3(theEnemyInfo.displacementX, theEnemyInfo.displacementY, theEnemyInfo.displacementZ), 0.f,
						Vector3(theEnemyInfo.scaleX, theEnemyInfo.scaleY, theEnemyInfo.scaleZ));
				}
			}
			nextData = 0;
			tempData = "";
		}
		return true;
	}
	else
	{
		return false;
	}
	return true;
}

bool FileManager::ReadShopFile(const string myFile)
{
	ifstream file;
	static int count = 0;
	string data = "";
	string tempData = "";
	int nextData = 0;
	bool skipFirstLine = false;
	file.open(myFile);

	if (file.is_open())
	{
		while (getline(file, data))
		{
			if (!skipFirstLine)
			{
				skipFirstLine = true;
				continue;
			}

			for (unsigned int i = 0; i < data.size(); ++i)
			{
				if (nextData == 0)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theShopInfo.name = tempData;
				}

				else if (nextData == 1)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}

					theShopInfo.eqAtk = tempData;
				}

				else if (nextData == 2)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}

					theShopInfo.eqDef = tempData;
				}

				else if (nextData == 3)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}

					theShopInfo.eqSpeed = tempData;
				}

				else if (nextData == 4)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}

					theShopInfo.eqId = tempData;
				}

				else if (nextData == 5)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theShopInfo.eqType = tempData;
				}

				else if (nextData == 6)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theShopInfo.isEquipped = static_cast<bool>(stoi(tempData));
				}

				else if (nextData == 7)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theShopInfo.gold = stoi(tempData);
				}

				tempData = "";
				++nextData;
			}
			Create::ShopItems(theShopInfo.name, theShopInfo.eqId, theShopInfo.eqAtk, theShopInfo.eqDef, theShopInfo.eqSpeed, theShopInfo.gold, stoi(theShopInfo.eqType));
			nextData = 0;
			tempData = "";
		}
		return true;
	}
	else
	{
		return false;
	}
	return true;
}

void FileManager::EditShopFile(const string myFile)
{
	ofstream File;
	File.open(myFile);
	File << "Name,Attack,Defense,Speed,ID,Type,Equipped,Gold\n";
	for (unsigned int i = 0; i < 3; i ++)
	{
		Equipment* temp = ShopManager::GetInstance()->getHelm()[i];
		if (temp != NULL)
		{
			File << temp->getName() << ","
				<< temp->GetAttack() << ","
				<< temp->GetDefense() << ","
				<< temp->GetSpeed() << ","
				<< temp->GetID() << ","
				<< temp->GetType() << ","
				<< temp->getEquippedStatus() << ","
				<<temp->getGold() << "\n";
		}
	}

	for (unsigned int i = 0; i < 3; i++)
	{
		Equipment* temp = ShopManager::GetInstance()->getArmor()[i];
		if (temp != NULL)
		{
			File << temp->getName() << ","
				<< temp->GetAttack() << ","
				<< temp->GetDefense() << ","
				<< temp->GetSpeed() << ","
				<< temp->GetID() << ","
				<< temp->GetType() << ","
				<< temp->getEquippedStatus() << ","
				<< temp->getGold() << "\n";
		}
	}

	for (unsigned int i = 0; i < 3; i++)
	{
		Equipment* temp = ShopManager::GetInstance()->getGlove()[i];
		if (temp != NULL)
		{
			File << temp->getName() << ","
				<< temp->GetAttack() << ","
				<< temp->GetDefense() << ","
				<< temp->GetSpeed() << ","
				<< temp->GetID() << ","
				<< temp->GetType() << ","
				<< temp->getEquippedStatus() << ","
				<< temp->getGold() << "\n";
		}
	}

	for (unsigned int i = 0; i < 3; i++)
	{
		Equipment* temp = ShopManager::GetInstance()->getShoe()[i];
		if (temp != NULL)
		{
			File << temp->getName() << ","
				<< temp->GetAttack() << ","
				<< temp->GetDefense() << ","
				<< temp->GetSpeed() << ","
				<< temp->GetID() << ","
				<< temp->GetType() << ","
				<< temp->getEquippedStatus() << ","
				<< temp->getGold() << "\n";
		}
	}

	for (unsigned int i = 0; i < 3; i++)
	{
		Equipment* temp = ShopManager::GetInstance()->getSword()[i];
		if (temp != NULL)
		{
			File << temp->getName() << ","
				<< temp->GetAttack() << ","
				<< temp->GetDefense() << ","
				<< temp->GetSpeed() << ","
				<< temp->GetID() << ","
				<< temp->GetType() << ","
				<< temp->getEquippedStatus() << ","
				<< temp->getGold() << "\n";
		}
	}

	for (unsigned int i = 0; i < 3; i++)
	{
		Equipment* temp = ShopManager::GetInstance()->getGun()[i];
		if (temp != NULL)
		{
			File << temp->getName() << ","
				<< temp->GetAttack() << ","
				<< temp->GetDefense() << ","
				<< temp->GetSpeed() << ","
				<< temp->GetID() << ","
				<< temp->GetType() << ","
				<< temp->getEquippedStatus() << ","
				<< temp->getGold() << "\n";
		}
	}
}

void FileManager::EditWeaponFile(const string myFile)
{
	ofstream File;
	File.open(myFile);
	File << "Name,Attack,Defense,Speed,ID,Type,Equipped\n";
	for (unsigned int i = 0; i < 6; i++)
	{
		if (EquipmentManager::GetInstance()->ReturnList()[i] != NULL)
		{
			Equipment* temp = EquipmentManager::GetInstance()->ReturnList()[i];
			File << temp->getName() << ","
				<< temp->GetAttack() << ","
				<< temp->GetDefense() << ","
				<< temp->GetSpeed() << ","
				<< temp->GetID() << ","
				<< temp->GetType() << ","
				<< temp->getEquippedStatus() << "\n";
		}
	}

	for (unsigned int i = 0; i < 12; i++)
	{
		if (Inventory::GetInstance()->ReturnType()[i] != NULL)
		{
			Equipment* temp = Inventory::GetInstance()->ReturnType()[i];
			File << temp->getName() << ","
				<< temp->GetAttack() << ","
				<< temp->GetDefense() << ","
				<< temp->GetSpeed() << ","
				<< temp->GetID() << ","
				<< temp->GetType() << ","
				<< temp->getEquippedStatus() << "\n";
		}
	}
	File.close();
}

void FileManager::EditMapFile(const string myFile)
{
	ofstream File;
	File.open(myFile);
	File << "Name,posX,posY,posZ,minAABBx,minAABBy,minAABBz,maxAABBx,maxAABBy,maxAABBz,scaleX,scaleY,scaleZ,rotateAngle\n";
	for (list<CFurniture*>::iterator it = EntityManager::GetInstance()->returnFixed().begin(); it != EntityManager::GetInstance()->returnFixed().end(); ++it)
	{
		CFurniture* temp = (CFurniture*)*it;
		File << temp->GetName() << ","
			<< temp->GetPosition().x << ","
			<< temp->GetPosition().y << ","
			<< temp->GetPosition().z << ","
			<< temp->GetMinAABB().x << ","
			<< temp->GetMinAABB().y << ","
			<< temp->GetMinAABB().z << ","
			<< temp->GetMaxAABB().x << ","
			<< temp->GetMaxAABB().y << ","
			<< temp->GetMaxAABB().z << ","
			<< temp->GetScale().x << ","
			<< temp->GetScale().y << ","
			<< temp->GetScale().z << ","
			<< temp->GetRotate() << "\n";
	}
}

void FileManager::EditEnemyFile(const string myFile)
{
	ofstream File;
	File.open(myFile);
	File << "type,displacementx,displacementy,displacementz,scalex,scaley,scalez\n";

	for (list<CEnemy3D*>::iterator it = EntityManager::GetInstance()->returnEnemy().begin(); it != EntityManager::GetInstance()->returnEnemy().end(); ++it)
	{
		CEnemy3D* temp = (CEnemy3D*)*it;
		File << temp->GetType() << ","
			<< temp->GetPosition().x << "," 
			<< temp->GetPosition().y << "," 
			<< temp->GetPosition().z << "," 
			<< temp->GetScale().x << "," 
			<< temp->GetScale().y << "," 
			<< temp->GetScale().z << "\n";
	}
}

void FileManager::PrintWeaponFile()
{
	for (vector<FileManager::EQ_Info>::iterator it = masterList.begin(); it != masterList.end(); ++it)
	{
		FileManager::EQ_Info store = (FileManager::EQ_Info)(*it);
		
	}
}

void FileManager::CreateWeapon()
{
	for (vector<FileManager::EQ_Info>::iterator it = masterList.begin(); it != masterList.end(); ++it)
	{
		FileManager::EQ_Info store = (FileManager::EQ_Info)(*it);

		if (stoi(store.eqType) == 0)
		{
			Create::theHelmet(store.name, store.eqId, store.eqAtk, store.eqDef, store.eqSpeed, store.isEquipped);
		}

		if (stoi(store.eqType) == 1)
		{
			Create::theArmour(store.name, store.eqId, store.eqAtk, store.eqDef, store.eqSpeed, store.isEquipped);
		}

		if (stoi(store.eqType) == 2)
		{
			Create::theGlove(store.name, store.eqId, store.eqAtk, store.eqDef, store.eqSpeed, store.isEquipped);
		}

		if (stoi(store.eqType) == 3)
		{
			Create::theShoe(store.name, store.eqId, store.eqAtk, store.eqDef, store.eqSpeed, store.isEquipped);
		}

		if (stoi(store.eqType) == 4)
		{
			Create::theSword(store.name, store.eqId, store.eqAtk, store.isEquipped);
		}

		if (stoi(store.eqType) == 5)
		{
			Create::theGun(store.name, store.eqId, store.eqAtk, store.isEquipped);
		}
	}
}

vector<FileManager::EQ_Info> FileManager::returnMasterList()
{
	return masterList;
}

void FileManager::clearVector()
{
	masterList.clear();
	eqlist.clear();
	objlist.clear();
	shopList.clear();
}

//Vector3 FileManager::Token2Vector(string token)
//{
//	Vector3 tempVector(0.0f, 0.0f, 0.0f);
//
//	std::istringstream ss(token);
//	std::string aToken = "";
//	std::getline(ss, aToken, ',');
//	tempVector.x = Token2Double(aToken);
//	std::getline(ss, aToken, ',');
//	tempVector.y = Token2Double(aToken);
//	std::getline(ss, aToken, ',');
//	tempVector.z = Token2Double(aToken);
//
//	return tempVector;
//}
//
//double FileManager::Token2Double(string token)
//{
//	return atof(token.c_str());
//}
//
//bool FileManager::Token2Bool(string token)
//{
//	return token.at(0) == '1';
//}