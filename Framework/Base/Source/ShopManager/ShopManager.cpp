#include "ShopManager.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "KeyboardController.h"
#include "../FileManager.h"
#include "..//Items/Inventory.h"
#include "../PlayerInfo/PlayerInfo.h"

void ShopManager::init()
{
	for (int i = 0; i < 3; i++)
	{
		helmList[i] = NULL;
		armorList[i] = NULL;
		gloveList[i] = NULL;
		shoeList[i] = NULL;
		swordList[i] = NULL;
		gunList[i] = NULL;
		temp[i] = NULL;
	}
	pressCountX = 0;
	pressCountY = 0;
}

void ShopManager::update(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyPressed('D'))
	{
		pressCountX += 1;
		pressCountY = 0;
		setList();
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('A'))
	{
		pressCountX -= 1;
		pressCountY = 0;

		setList();
	}

	if (pressCountX < 0)
	{
		pressCountX = 5;
		setList();

	}

	if (pressCountX > 5)
	{
		pressCountX = 0;
		setList();

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

	if (KeyboardController::GetInstance()->IsKeyPressed('B'))
	{
		Inventory::GetInstance()->assign_storage(temp[pressCountY]);
	}
}

void ShopManager::render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	// Push the current transformation into the modelStack
	modelStack.PushMatrix();
	modelStack.Translate(-5, 0, 0);
	modelStack.Scale(500, 500, 1);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("SHOP"));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-5, 50 - pressCountY * 110, 0);
	modelStack.Scale(110, 110, 1);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("SELECTION"));
	modelStack.PopMatrix();

	for (int i = 0; i < 3; i++)
	{
		if (temp[i] != NULL)
		{
			modelStack.PushMatrix();
			modelStack.Translate(-5, 50 - i * 110, 0);
			modelStack.Scale(70, 70, 1);
			RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh(temp[i]->getName()));
			modelStack.PopMatrix();
		}
	}

	modelStack.PushMatrix();
	modelStack.Translate(-90, 180.f, 0.f);
	modelStack.Scale(100, 100, 35);
	RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), name, Color(0.f, 0.f, 0.f));
	modelStack.PopMatrix();
}

void ShopManager::setList()
{
	if (pressCountX == 0)
	{
		for (int i = 0; i < 3; i++)
		{
			temp[i] = helmList[i];
		}
		name = "HELMET";
	}

	if (pressCountX == 1)
	{
		for (int i = 0; i < 3; i++)
		{
			temp[i] = armorList[i];
		}
		name = "ARMOR";
	}

	if (pressCountX == 2)
	{
		for (int i = 0; i < 3; i++)
		{
			temp[i] = gloveList[i];
		}
		name = "GLOVE";
	}

	if (pressCountX == 3)
	{
		for (int i = 0; i < 3; i++)
		{
			temp[i] = shoeList[i];
		}
		name = "SHOE";
	}

	if (pressCountX == 4)
	{
		for (int i = 0; i < 3; i++)
		{
			temp[i] = swordList[i];
		}
		name = "SWORD";
	}

	if (pressCountX == 5)
	{
		for (int i = 0; i < 3; i++)
		{
			temp[i] = gunList[i];
		}
		name = "GUN";
	}

}

void ShopManager::PushBackIntoList(Equipment * result, int type)
{
	static int count = 0;

	for (int i = 0; i < 6; i++)
	{
		if (type == 0)
		{
			if (helmList[i] == NULL)
			{
				helmList[i] = result;
				count++;
				break;
			}
		}

		if (type == 1)
		{
			if (armorList[i] == NULL)
			{
				armorList[i] = result;
				count++;
				break;
			}
		}

		if (type == 2)
		{
			if (gloveList[i] == NULL)
			{
				gloveList[i] = result;
				count++;
				break;
			}
		}

		if (type == 3)
		{
			if (shoeList[i] == NULL)
			{
				shoeList[i] = result;
				count++;
				break;
			}
		}

		if (type == 4)
		{
			if (swordList[i] == NULL)
			{
				swordList[i] = result;
				count++;
				break;
			}
		}

		if (type == 5)
		{
			if (gunList[i] == NULL)
			{
				gunList[i] = result;
				count++;
				break;
			}
		}
	}
}

void ShopManager::ArrangeVector(int type)
{
	Inventory::GetInstance()->assign_storage(temp[pressCountY]);

	for (int i = pressCountY; i < 2; i++)
	{
		if (type == 0)
		{
			Equipment* temp = helmList[i + 1];
			helmList[i] = temp;
		}

		if (type == 1)
		{
			Equipment* temp = armorList[i + 1];
			armorList[i] = temp;
		}

		if (type == 2)
		{
			Equipment* temp = gloveList[i + 1];
			gloveList[i] = temp;
		}

		if (type == 3)
		{
			Equipment* temp = shoeList[i + 1];
			shoeList[i] = temp;
		}

		if (type == 4)
		{
			Equipment* temp = swordList[i + 1];
			swordList[i] = temp;
		}

		if (type == 5)
		{
			Equipment* temp = gunList[i + 1];
			gunList[i] = temp;
		}
	}

	if (type == 0)
	{
		helmList[2] = NULL;
	}

	if (type == 1)
	{
		armorList[2] = NULL;
	}

	if (type == 2)
	{
		gloveList[2] = NULL;
	}

	if (type == 3)
	{
		shoeList[2] = NULL;
	}

	if (type == 4)
	{
		swordList[2] = NULL;
	}

	if (type == 5)
	{
		gunList[2] = NULL;
	}

	setList();
}
