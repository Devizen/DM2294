#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Vector3.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "Inventory.h"
#include "RenderHelper.h"
#include "KeyboardController.h"

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

void FileManager::init()
{
	pressCountX = 0;
	pressCountY = 0;
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

void FileManager::EditFile(const string myFile)
{
}

void FileManager::RenderWeapon()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	// Push the current transformation into the modelStack
	modelStack.PushMatrix();
	// Translate the current transformation
	//modelStack.Translate(Application::GetInstance().GetWindowWidth() * 0.5, Application::GetInstance().GetWindowHeight() * 0.5, 0);
	// Scale the current transformation
	modelStack.Scale(500, 500, 1);

	// Push the current transformation into the modelStack
	modelStack.PushMatrix();
	// Display the Avatar
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("INVENTORY"));
	modelStack.PopMatrix();

	modelStack.PopMatrix();

	modelStack.PushMatrix();
	// Display the Avatar

	modelStack.Translate(-180 + pressCountX * 118, 135 - pressCountY * 135, 0);
	modelStack.Scale(100, 100, 1);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("SELECTION"));
	modelStack.PopMatrix();

	for (int i = 0; i < 12; i++)
	{
		int j = i;
		int k = 0;
		if (i >= 4 && i < 8)
		{
			j -= 4;
			k = 1;
		}
		else if (i >= 8 && i < 12)
		{
			j -= 8;
			k = 2;
		}

		if (Inventory::GetInstance()->ReturnType()[i] != NULL)
		{
			modelStack.PushMatrix();
			// Translate the current transformation
			modelStack.Translate(-180 + j * 118, 135 - k * 135, 0);
			// Scale the current transformation
			modelStack.Scale(80, 80, 1);

			// Push the current transformation into the modelStack
			modelStack.PushMatrix();
			// Display the Avatar
			RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh(Inventory::GetInstance()->ReturnType()[i]->getName()));
			modelStack.PopMatrix();
			modelStack.PopMatrix();
		}
	}
}

void FileManager::DeleteWeapon()
{
	int PosToDelete = pressCountX + (pressCountY * 4);
	Inventory::GetInstance()->remove_storage(PosToDelete);
}

void FileManager::update(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyPressed('D'))
	{
		pressCountX += 1;
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('A'))
	{
		pressCountX -= 1;
	}

	if (pressCountX < 0)
	{
		pressCountX = 3;
	}

	if (pressCountX > 3)
	{
		pressCountX = 0;
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('S'))
	{
		pressCountY += 1;
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('W'))
	{
		pressCountY -= 1;
	}

	if (pressCountY < 0)
	{
		pressCountY = 2;
	}

	if (pressCountY > 2)
	{
		pressCountY = 0;
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('E'))
	{
		DeleteWeapon();
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