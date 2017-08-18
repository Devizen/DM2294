#include "Inventory.h"
#include <iostream>
#include "Mesh.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "KeyboardController.h"
#include "../FileManager.h"

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

	if (KeyboardController::GetInstance()->IsKeyPressed('E'))
	{
		DeleteWeapon();
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
			count++;
			cout << count << endl;
			break;
		}
	}
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
	for (int i = position; i < 12 - 1; i++)
	{
		Equipment* temp = storage[i + 1];
		storage[i] = temp;
	}
	storage[11] = NULL;
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

		if (storage[i] != NULL)
		{
			modelStack.PushMatrix();
			// Translate the current transformation
			modelStack.Translate(-180 + j * 118, 135 - k * 135, 0);
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
}

void Inventory::DeleteWeapon()
{
	int PosToDelete = pressCountX + (pressCountY * 4);
	remove_storage(PosToDelete);
}
