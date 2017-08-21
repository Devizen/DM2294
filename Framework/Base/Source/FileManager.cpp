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

			for (int i = 0; i < data.size(); ++i)
			{
				if (nextData == 0)
				{
					for (int j = i; j < data.size(); ++j)
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
					CPlayerInfo::GetInstance()->setHealthTo(stof(tempData));
				}

				if (nextData == 1)
				{
					for (int j = i; j < data.size(); ++j)
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
					CPlayerInfo::GetInstance()->setMP(stof(tempData));
				}

				if (nextData == 2)
				{
					for (int j = i; j < data.size(); ++j)
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

				if (nextData == 3)
				{
					for (int j = i; j < data.size(); ++j)
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
					CPlayerInfo::GetInstance()->setDefenseTo(stof(tempData));
				}

				if (nextData == 4)
				{
					for (int j = i; j < data.size(); ++j)
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

				if (nextData == 5)
				{
					for (int j = i; j < data.size(); ++j)
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

				if (nextData == 6)
				{
					for (int j = i; j < data.size(); ++j)
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

				if (nextData == 7)
				{
					for (int j = i; j < data.size(); ++j)
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
					CPlayerInfo::GetInstance()->setEXP(stof(tempData));
				}

				++nextData;
			}
			nextData = 0;
		}
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

			for (int i = 0; i < data.size(); ++i)
			{
				if (nextData == 0)
				{
					for (int j = i; j < data.size(); ++j)
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

				if (nextData == 1)
				{
					for (int j = i; j < data.size(); ++j)
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

				if (nextData == 2)
				{
					for (int j = i; j < data.size(); ++j)
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

				if (nextData == 3)
				{
					for (int j = i; j < data.size(); ++j)
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

				if (nextData == 4)
				{
					for (int j = i; j < data.size(); ++j)
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

				if (nextData == 5)
				{
					for (int j = i; j < data.size(); ++j)
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

				if (nextData == 6)
				{
					for (int j = i; j < data.size(); ++j)
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
					theEQInfo.isEquipped = stoi(tempData);
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
		cout << "GG" << endl;
		return false;
	}
	return true;
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

			for (int i = 0; i < data.size(); ++i)
			{
				if (nextData == 0)
				{
					for (int j = i; j < data.size(); ++j)
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

				if (nextData == 1)
				{
					for (int j = i; j < data.size(); ++j)
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

				if (nextData == 2)
				{
					for (int j = i; j < data.size(); ++j)
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

				if (nextData == 3)
				{
					for (int j = i; j < data.size(); ++j)
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

				if (nextData == 4)
				{
					for (int j = i; j < data.size(); ++j)
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

				if (nextData == 5)
				{
					for (int j = i; j < data.size(); ++j)
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

				if (nextData == 6)
				{
					for (int j = i; j < data.size(); ++j)
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

				if (nextData == 7)
				{
					for (int j = i; j < data.size(); ++j)
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

				if (nextData == 8)
				{
					for (int j = i; j < data.size(); ++j)
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

				if (nextData == 9)
				{
					for (int j = i; j < data.size(); ++j)
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

				if (nextData == 10)
				{
					for (int j = i; j < data.size(); ++j)
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

				if (nextData == 11)
				{
					for (int j = i; j < data.size(); ++j)
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

				if (nextData == 12)
				{
					for (int j = i; j < data.size(); ++j)
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

				if (nextData == 13)
				{
					for (int j = i; j < data.size(); ++j)
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
	}

	return true;
}

void FileManager::EditFile(const string myFile)
{
}

void FileManager::EditWeaponFile(const string myFile)
{
	ofstream File;
	File.open(myFile);
	File << "Name,Attack,Defense,Speed,ID,Type,Equipped\n";
	for (int i = 0; i < 6; i++)
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

			cout << "Equipped Status: " << temp->getEquippedStatus() << endl;
		}
	}

	for (int i = 0; i < 12; i++)
	{
		cout << "IN" << endl;

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
		File << temp->GetItem() << ","
			<< temp->GetPosition().x << ","
			<< temp->GetPosition().y << ","
			<< temp->GetPosition().z << ","
			<< temp->GetMinAABB().x << ","
			<< temp->GetMinAABB().y << ","
			<< temp->GetMinAABB().z << ","
			<< temp->GetMaxAABB().x << ","
			<< temp->GetMaxAABB().x << ","
			<< temp->GetMaxAABB().x << ","
			<< temp->GetScale().x << ","
			<< temp->GetScale().y << ","
			<< temp->GetScale().z << ","
			<< temp->GetRotate() << "\n";
	}
}

void FileManager::PrintWeaponFile()
{
	for (vector<FileManager::EQ_Info>::iterator it = masterList.begin(); it != masterList.end(); ++it)
	{
		FileManager::EQ_Info store = (FileManager::EQ_Info)(*it);
		cout << store.name << ", "
			<< store.eqAtk << ", "
			<< store.eqDef << ", "
			<< store.eqSpeed << ", "
			<< store.eqId << ", "
			<< store.isEquipped << ", "
			<< store.eqType << endl;
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