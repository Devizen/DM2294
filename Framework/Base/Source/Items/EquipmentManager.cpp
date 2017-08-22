/*Equipment was done by Lim Guan Sheng, Marcus (161725E)*/
/*Tested by Lai Wei Jing, Eugene, Chan Zhi Hao and Kwong Yu Ming*/

#include "EquipmentManager.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "KeyboardController.h"
#include "../FileManager.h"
#include "Inventory.h"
#include "../PlayerInfo/PlayerInfo.h"

using std::cout;
using std::endl;

void EquipmentManager::Init()
{
	for (int i = 0; i < 6; i++)
	{
		EqList[i] = NULL;
		continue;
	}

	pressCountX = 0;
	pressCountY = 0;
	textTime = 0;
	showText = false;
}

void EquipmentManager::AssignEquipment(Equipment * newEQ)
{
	static int count = 0;
	for (int i = 0; i < 6; i++)
	{
		if (EqList[i] == NULL)
		{
			EqList[i] = newEQ;
			count++;
			std::cout << "Count: " << count << std::endl;
			break;
		}
	}
}

void EquipmentManager::UnEquip(int countX, int countY)
{
	int TypeToUnEquip; 
	int PosToUnequip;
	int count = 0;
	bool swap = false;
	if (countY <= 1)
	{
		TypeToUnEquip = countX + countY;
	}
	if (countY > 1)
	{
		TypeToUnEquip = countX + countY * 2;
	}

	if (TypeToUnEquip == 4 && !swap)
	{
		TypeToUnEquip = 3;
		swap = true;
	}
	if (TypeToUnEquip == 3 && !swap)
	{
		TypeToUnEquip = 4;
		swap = true;
	}

	cout << "Type: " <<  TypeToUnEquip << endl;
	for (int i = 0; i < 6; i++)
	{
		if (EqList[i] != NULL)
		{
			if (EqList[i]->GetType() == TypeToUnEquip)
			{
				cout << "EQ TYPE: " << EqList[i]->GetType() << endl;
				PosToUnequip = i;
				Inventory::GetInstance()->assign_storage(EqList[PosToUnequip]);
				EqList[PosToUnequip] = NULL;
				break;
			}
		}
	}
	CPlayerInfo::GetInstance()->resetAttribute();
	AddAttributes();
	//cout << "Unequip: " << PosToUnEquip << endl;
	//for (int i = 0; i < 6; i++)
	//{
	//	if (EqList[i] != NULL)
	//	{
	//		count++;
	//	}
	//}

	//merge_sort(EqList, 0, count-1);
	//EqList[PosToUnEquip] = NULL;
	//for (int i = PosToUnequip; i < 6 - 1; i++)
	//{
	//	Equipment* temp = EqList[i + 1];
	//	EqList[i] = temp;
	//}
	
}

void EquipmentManager::AddAttributes()
{
	for (int i = 0; i < 6; i++)
	{
		if (EqList[i] != NULL)
		{
			if (EqList[i]->GetType() >= 0 && EqList[i]->GetType() <= 3)
			{
				CPlayerInfo::GetInstance()->addAttack(EqList[i]->GetAttack());
				CPlayerInfo::GetInstance()->addDefence(EqList[i]->GetDefense());
				CPlayerInfo::GetInstance()->addSpeed(EqList[i]->GetSpeed());
			}
			else
			{
				CPlayerInfo::GetInstance()->addAttack(EqList[i]->GetAttack());
			}
		}
	}
}

Equipment ** EquipmentManager::ReturnList()
{
	return EqList;
}

void EquipmentManager::Update(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyPressed('D'))
	{
		pressCountX += 1;
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('A'))
	{
		pressCountX -= 1;
	}

	if (pressCountX < 0 && pressCountY == 0)
	{
		pressCountX = 0;
	}

	if (pressCountX > 0 && pressCountY == 0)
	{
		pressCountX = 0;
	}

	if (pressCountX > 1 && pressCountY == 1)
	{
		pressCountX = 0;
	}

	if (pressCountX < 0 && pressCountY == 1)
	{
		pressCountX = 1;
	}	

	if (pressCountX > 1 && pressCountY == 2)
	{
		pressCountX = -1;
	}

	if (pressCountX < -1 && pressCountY == 2)
	{
		pressCountX = 1;
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

	if (KeyboardController::GetInstance()->IsKeyPressed('Q'))
	{
		UnEquip(pressCountX, pressCountY);
		showText = true;
		textTime = 0;
	}

	if (showText == true)
	{
		textTime += dt;
	}

	if (textTime > 1)
	{
		textTime = 0;
		showText = false;
	}
}

void EquipmentManager::Render()
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
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("EQWINDOW"));
	modelStack.PopMatrix();

	modelStack.PopMatrix();

	modelStack.PushMatrix();
	// Display the Avatar

	modelStack.Translate(pressCountX * 140, 160 - pressCountY * 145, 0);
	modelStack.Scale(110, 110, 1);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("SELECTION"));
	modelStack.PopMatrix();
	for (int i = 0; i < 6; i++)
	{
		if (EqList[i] != NULL)
		{
			if (EqList[i]->GetType() == 0)
			{
				modelStack.PushMatrix();
				modelStack.Translate(0, 160, 0);
				modelStack.Scale(110, 110, 1);
				RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh(EqList[i]->getName()));
				modelStack.PopMatrix();
			}

			if (EqList[i]->GetType() == 1)
			{
				modelStack.PushMatrix();
				modelStack.Translate(0, 15, 0);
				modelStack.Scale(110, 110, 1);
				RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh(EqList[i]->getName()));
				modelStack.PopMatrix();
			}

			if (EqList[i]->GetType() == 2)
			{
				modelStack.PushMatrix();
				modelStack.Translate(140, 15, 0);
				modelStack.Scale(80, 80, 1);
				RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh(EqList[i]->getName()));
				modelStack.PopMatrix();
			}

			if (EqList[i]->GetType() == 3)
			{
				modelStack.PushMatrix();
				modelStack.Translate(0, -130, 0);
				modelStack.Scale(110, 110, 1);
				RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh(EqList[i]->getName()));
				modelStack.PopMatrix();
			}

			if (EqList[i]->GetType() == 4)
			{
				modelStack.PushMatrix();
				modelStack.Translate(-140, -130, 0);
				modelStack.Scale(110, 110, 1);
				RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh(EqList[i]->getName()));
				modelStack.PopMatrix();
			}

			if (EqList[i]->GetType() == 5)
			{
				modelStack.PushMatrix();
				modelStack.Translate(140, -130, 0);
				modelStack.Scale(110, 110, 1);
				RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh(EqList[i]->getName()));
				modelStack.PopMatrix();
			}
		}
	}
	

	if (showText)
	{
		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		string message = "Unequipped!";
		modelStack.PushMatrix();
		modelStack.Translate(100, 180.f, 0.f);
		modelStack.Scale(35, 35, 35);
		RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), message, Color(1.f, 0.f, 0.f));
		modelStack.PopMatrix();
	}
}

void EquipmentManager::merge_sort(Equipment * data_set[], int start_index, int end_index)
{
	int middle_index;
	if (1 > (end_index - start_index)) return;
	middle_index = (start_index + end_index) / 2;
	merge_sort(data_set, start_index, middle_index);
	merge_sort(data_set, middle_index + 1, end_index);
	merge(data_set, start_index, middle_index, end_index);
}

void EquipmentManager::merge(Equipment * data_set[], int start_index, int middle_index, int end_index)
{
	int left_index = start_index;
	int right_index = middle_index + 1;
	int result_index = 0;
	Equipment *result_set[7];
	
	while ((left_index <= middle_index) && (right_index <= end_index)) {
		if (data_set[left_index]->GetType() < data_set[right_index]->GetType()) 
		{
			result_set[result_index] = data_set[left_index];
			++left_index;
		}
		else {
			result_set[result_index] = data_set[right_index];
			++right_index;
		}
		++result_index;
	}
	while (left_index <= middle_index) result_set[result_index++] = data_set[left_index++];
	while (right_index <= end_index) result_set[result_index++] = data_set[right_index++];
	result_index = 0;
	for (int i = start_index; i <= end_index; ++i) {
		data_set[i] = result_set[result_index++];
	}
}