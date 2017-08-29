/*Inventory was done by Lim Guan Sheng, Marcus (161725E)*/
/*Tested by Lai Wei Jing, Eugene, Chan Zhi Hao and Kwong Yu Ming*/

#include "Inventory.h"
#include <iostream>
#include "Mesh.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "KeyboardController.h"
#include "../FileManager.h"
#include "EquipmentManager.h"
#include "..//PlayerInfo/PlayerInfo.h"

using std::cout;
using std::endl;

void Inventory::Init()
{
	for (int i = 0; i < 12; i++)
	{
		storage[i] = NULL;
		continue;
	}

	pressCountX = 0;
	pressCountY = 0;

	showDiscardText = false;
	showEquipText = false;

	EquipTextTime = 0;
	DiscardTextTime = 0;
	currentEquipped = false;
}

void Inventory::Update(double dt)
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

	if (KeyboardController::GetInstance()->IsKeyPressed('F'))
	{
		if (storage[pressCountX + (pressCountY * 4)] != NULL)
		{
			DeleteWeapon();
			showDiscardText = true;
			DiscardTextTime = 0;
		}
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('E'))
	{
		for (int i = 0; i < 6; i++)
		{
			if (EquipmentManager::GetInstance()->ReturnList()[i] != NULL && storage[pressCountX + (pressCountY * 4)] != NULL)
			{
				if (storage[pressCountX + (pressCountY * 4)]->GetType() == EquipmentManager::GetInstance()->ReturnList()[i]->GetType())
				{
					currentEquipped = true; // dont equip if the item of same type is there alr
				}
			}
		}
		if (currentEquipped == false)
		{
			if (storage[pressCountX + (pressCountY * 4)] != NULL)
			{
				EquipWeapon();
				showEquipText = true;
				EquipTextTime = 0;
				currentEquipped = false;
			}
		}
	}

	if (showDiscardText == true)
	{
		DiscardTextTime += dt;
	}

	if (showEquipText == true)
	{
		EquipTextTime += dt;
	}

	if (DiscardTextTime > 1)
	{
		showDiscardText = false;
	}

	if (EquipTextTime > 1)
	{
		showEquipText = false;
	}
}

void Inventory::assign_storage(Equipment* object)
{
	static int count = 0;
	for (int i = 0; i < 12; i++)
	{
		if (storage[i] == NULL)
		{
			storage[i] = object;

			storage[i]->setStatus(false);
			count++;
			cout << count << endl;
			break;
		}
	}
	CPlayerInfo::GetInstance()->resetAttribute();
	EquipmentManager::GetInstance()->AddAttributes();
}

Equipment** Inventory::ReturnType()
{
	return storage;
}

void Inventory::remove_storage(int position)
{
	//for (auto it = Inventory::GetInstance()->storage.begin(); it != Inventory::GetInstance()->storage.end();++it)
	//{
	//	if (position != count)
	//	{
	//		++count;
	//		continue;
	//	}
	//	Equipment* find = (Equipment*)*it;
	//	cout << "Address in Inventory: " << find << endl;
	//	storage.remove(find);
	//	delete find;
	//	find = nullptr;
	//	//Inventory::GetInstance()->storage.erase(it);
	//	//storage.pop_back();
	//	break;
	//}
	for (int i = position; i < 11; i++)
	{
		Equipment* temp = storage[i + 1];
		if (temp != NULL)
		{
			storage[i] = temp;
		}
		else
		{
			storage[i] = NULL;
		}
	}
	storage[11] = NULL;

	CPlayerInfo::GetInstance()->resetAttribute();
	EquipmentManager::GetInstance()->AddAttributes();
}

void Inventory::RenderWeapon()
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

	modelStack.Translate(-180.f + (float)pressCountX * 118.f, 135.f - (float)pressCountY * 135.f, 0.f);
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

		if (storage[i] != NULL)
		{
			modelStack.PushMatrix();
			// Translate the current transformation
			modelStack.Translate(-180.f + (float)j * 118.f, 135.f - (float)k * 135.f, 0.f);
			// Scale the current transformation
			modelStack.Scale(80, 80, 1);

			// Push the current transformation into the modelStack
			modelStack.PushMatrix();
			// Display the Avatar
			RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh(storage[i]->getName()));
			modelStack.PopMatrix();
			modelStack.PopMatrix();
		}
	}

	if (showEquipText)
	{
		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		string message = "Equipped!";
		modelStack.PushMatrix();
		modelStack.Translate(100, 180.f, 0.f);
		modelStack.Scale(35, 35, 35);
		RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), message, Color(1.f, 0.f, 0.f));
		modelStack.PopMatrix();
	}

	if (showDiscardText)
	{
		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		string message = "Discarded!";
		modelStack.PushMatrix();
		modelStack.Translate(100, 150.f, 0.f);
		modelStack.Scale(35, 35, 35);
		RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), message, Color(1.f, 0.f, 0.f));
		modelStack.PopMatrix();
	}
}

void Inventory::DeleteWeapon()
{
	int PosToDelete = pressCountX + (pressCountY * 4);
	remove_storage(PosToDelete);
}

void Inventory::EquipWeapon()
{
	int PosToEquip = pressCountX + (pressCountY * 4);

	storage[PosToEquip]->setStatus(true);
	EquipmentManager::GetInstance()->AssignEquipment(storage[PosToEquip]);
	storage[PosToEquip] = NULL;
	remove_storage(PosToEquip);
}
