#include "FileManager.h"
#include <iostream>
#include <sstream>
#include "Vector3.h"

#include "Items\Armour.h"
#include "Items\Basic_Sword.h"
#include "Items\Glove.h"
#include "Items\Gun.h"
#include "Items\Helmet.h"
#include "Items\Shoe.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::getline;

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
					setHealthTo(stof(tempData));
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
					setMP(stof(tempData));
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
					setAttackTo(stoi(tempData));
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
					setDefenseTo(stof(tempData));
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
					setSpeed(stoi(tempData));
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
					setGold(stoi(tempData));
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
					setLevel(stoi(tempData));
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
					setEXP(stof(tempData));
				}
			}
		}
	}

	return false;
}

bool FileManager::ReadWeaponFile(const string myFile)
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
					theEQInfo.eqAtk = tempData;
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
					theEQInfo.eqDef = tempData;
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
					theEQInfo.eqSpeed = tempData;
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
					theEQInfo.eqId = tempData;
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
					theEQInfo.eqType = tempData;
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
					theEQInfo.name = tempData;
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
			}
			masterList.push_back(theEQInfo);
		}
		return true;
	}
	return true;
}

void FileManager::EditFile(const string myFile)
{
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