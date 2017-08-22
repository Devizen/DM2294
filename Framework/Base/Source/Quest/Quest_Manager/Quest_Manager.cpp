#include "Quest_Manager.h"/*
#include "../Quest/Quest.h"*/
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../../Application.h"
#include "KeyboardController.h"
#include <iostream>

using std::cout;
using std::endl;

Quest_Manager::Quest_Manager()
{
}

Quest_Manager::~Quest_Manager()
{
}

vector<CQuest*>& Quest_Manager::returnQuestList()
{
	return QuestList;
}

void Quest_Manager::updateQuest(double dt)
{
	//if (QuestList.size() > 0)
	//{
	//	for (vector<CQuest*>::iterator it = QuestList.begin(); it != QuestList.end();)
	//	{
	//		CQuest* Quest = (CQuest*)*it;
	//		if (Quest->activateQuest)
	//			if (Quest->QuestType == CQuest::Quest_BATTLE)
	//			{
	//				Quest->durationElapsed += static_cast<float>(dt);
	//				/*Scale the background; animation.*/
	//				if (Quest->scaleBackground <= Application::GetInstance().GetWindowWidth() * 2.f)
	//					Quest->scaleBackground += static_cast<float>(dt) * 3000.f;
	//				if (Quest->scaleQuest <= Application::GetInstance().GetWindowWidth() * 0.04f)
	//					Quest->scaleQuest += static_cast<float>(dt) * 50.f;
	//				if (Quest->durationElapsed >= Quest->maxDuration)
	//				{
	//					Quest->activateQuest = false;
	//					if (QuestList.size() > 1)
	//					{
	//						CQuest* tempQuest = QuestList.back();
	//						/*Move the first Quest to the last in vector.*/
	//						QuestList.back() = Quest;
	//						/*First Quest change to last Quest.*/
	//						Quest = tempQuest;
	//						/*Remove first Quest.*/
	//						delete Quest;
	//						Quest = nullptr;
	//						QuestList.pop_back();
	//						/*Move last Quest back to the default position.*/
	//						QuestList.back() = tempQuest;
	//						break;
	//					}
	//					else
	//					{
	//						delete Quest;
	//						Quest = nullptr;
	//						QuestList.pop_back();
	//						break;
	//					}
	//				}
	//			}
	//			else if (Quest->QuestType == CQuest::Quest_CONVERSATION)
	//			{
	//				static string storeQuest = Quest->message;
	//				static int count = 0;
	//				static bool erase = false;
	//				static int lineCount = 0;
	//				static int characterCount = 0;
	//				bool preventCancel = false;
	//				if (!erase)
	//				{
	//					storeQuest = Quest->message;
	//					Quest->message = "";
	//					erase = true;
	//				}
	//				/*Display entire message.*/
	//				if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) && Quest->message.size() != storeQuest.size())
	//				{
	//					Quest->QuestConversation[0] = "";
	//					Quest->QuestConversation[1] = "";
	//					Quest->QuestConversation[2] = "";
	//					int count = 0;
	//					int nextVector = 0;
	//					for (size_t i = 0; i < storeQuest.size(); ++i)
	//					{
	//						Quest->QuestConversation[nextVector] += storeQuest[i];
	//						if (count >= 50)
	//						{
	//							count = 0;
	//							++nextVector;
	//							continue;
	//						}
	//						++count;
	//					}
	//					Quest->message = storeQuest;
	//					preventCancel = true;
	//				}
	//				Quest->durationElapsed += static_cast<float>(dt);
	//				if (Quest->durationElapsed >= 0.025f && Quest->message.size() != storeQuest.size())
	//				{
	//					Quest->durationElapsed = 0.f;
	//					Quest->message += storeQuest[count];
	//					++characterCount;
	//					if (characterCount <= 50 && lineCount == 0)
	//						Quest->QuestConversation[0] += storeQuest[count];
	//					else if (characterCount <= 50 && lineCount == 1)
	//						Quest->QuestConversation[1] += storeQuest[count];
	//					else if (characterCount <= 50 && lineCount == 2)
	//						Quest->QuestConversation[2] += storeQuest[count];
	//					if (Quest->message[count] == '\n')
	//					{
	//						++lineCount;
	//						characterCount = 0;
	//					}
	//					if (characterCount == 50)
	//					{
	//						characterCount = 0;
	//						++lineCount;
	//					}
	//					++count;
	//					cout << "Count: " << count << " and Message: " << Quest->QuestConversation[0] << endl;
	//					cout << "Count: " << count << " and Message: " << Quest->QuestConversation[1] << endl;
	//					cout << "Count: " << count << " and Message: " << Quest->QuestConversation[2] << endl;
	//				}
	//				cout << "Original Length: " << storeQuest.length() << " and " << "Message Length: " << Quest->message.size() << endl;
	//				if (Quest->message.size() >= storeQuest.size())
	//				{
	//					if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) && !preventCancel)
	//					{
	//						Quest->activateQuest = false;
	//						if (QuestList.size() > 0)
	//						{
	//							/*Resets variables so that it will re-calculate.*/
	//							erase = false;
	//							count = 0;
	//							lineCount = 0;
	//							characterCount = 0;
	//							storeQuest = "";
	//							CQuest* _Quest = QuestList.back();
	//							delete Quest;
	//							Quest = nullptr;
	//							
	//							QuestList.pop_back();
	//							//CQuest* tempQuest = QuestList.back();
	//							///*Remove the strings in vector.*/
	//							//while (tempQuest->QuestConversation.size() != 0)
	//							//{
	//							//	tempQuest->QuestConversation.pop_back();
	//							//}
	//							///*Move the first Quest to the last in vector.*/
	//							//QuestList.back() = Quest;
	//							///*First Quest change to last Quest.*/
	//							//Quest = tempQuest;
	//							///*Remove first Quest.*/
	//							//delete Quest;
	//							//Quest = nullptr;
	//							//QuestList.pop_back();
	//							///*Move last Quest back to the default position.*/
	//							//QuestList.back() = tempQuest;
	//							break;
	//						}
	//						else
	//						{
	//							delete Quest;
	//							Quest = nullptr;
	//							QuestList.pop_back();
	//							break;
	//						}
	//					}
	//				}
	//			}
	//		break;
	//	}
	//}
}

void Quest_Manager::renderQuest(void)
{
	//if (QuestList.size() > 0)
	//{
	//	for (vector<CQuest*>::iterator it = QuestList.begin(); it != QuestList.end();)
	//	{
	//		CQuest* Quest = (CQuest*)*it;
	//		if (Quest->activateQuest)
	//		{
	//			if (Quest->QuestType == CQuest::Quest_BATTLE)
	//			{
	//				if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() < 1.5f)
	//				{
	//					/*Quad for the Quest to display onto.*/
	//					MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	//					modelStack.PushMatrix();
	//					modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.48f, 0.f, 0.f);
	//					modelStack.Scale(Quest->scaleBackground, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
	//					Mesh* modelMesh;
	//					modelMesh = MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 1.0f), 1.0f);
	//					RenderHelper::RenderMesh(modelMesh);
	//					modelStack.PopMatrix();
	//					/*Display Quest.*/
	//					modelStack.PushMatrix();
	//					modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.48f, 0.f, 0.f);
	//					modelStack.Scale(Quest->scaleQuest, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
	//					RenderHelper::RenderQuest(Quest->modelMesh, Quest->message, Color(1.f, 0.f, 0.f));
	//					modelStack.PopMatrix();
	//				}
	//				else
	//				{
	//					/*Quad for the Quest to display onto.*/
	//					MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	//					modelStack.PushMatrix();
	//					modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.48f, 0.f, 0.f);
	//					modelStack.Scale(Quest->scaleBackground, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
	//					Mesh* modelMesh;
	//					modelMesh = MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 1.0f), 1.0f);
	//					RenderHelper::RenderMesh(modelMesh);
	//					modelStack.PopMatrix();
	//					/*Display Quest.*/
	//					modelStack.PushMatrix();
	//					modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.48f, 0.f, 0.f);
	//					modelStack.Scale(Quest->scaleQuest, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
	//					RenderHelper::RenderQuest(Quest->modelMesh, Quest->message, Color(1.f, 0.f, 0.f));
	//					modelStack.PopMatrix();
	//				}
	//			}
	//			else if (Quest->QuestType == CQuest::Quest_CONVERSATION)
	//			{
	//				MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	//				modelStack.PushMatrix();
	//				modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.48f, 0.f, 0.f);
	//				modelStack.Scale(Application::GetInstance().GetWindowWidth() * 2.f, Application::GetInstance().GetWindowWidth() * 0.12f, 1.f);
	//				Mesh* modelMesh;
	//				modelMesh = MeshBuilder::GetInstance()->GenerateCube("cube", Color(0.f, 0.f, 0.f), 1.0f);
	//				RenderHelper::RenderMesh(modelMesh);
	//				modelStack.PopMatrix();
	//				modelStack.PushMatrix();
	//				modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.48f, Application::GetInstance().GetWindowHeight() * 0.05f, 0.f);
	//				modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.04f, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
	//				RenderHelper::RenderQuest(Quest->modelMesh, Quest->QuestConversation[0], Color(1.f, 0.f, 0.f));
	//				modelStack.PopMatrix();
	//				modelStack.PushMatrix();
	//				modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.48f, 0.f, 0.f);
	//				modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.04f, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
	//				RenderHelper::RenderQuest(Quest->modelMesh, Quest->QuestConversation[1], Color(1.f, 0.f, 0.f));
	//				modelStack.PopMatrix();
	//				modelStack.PushMatrix();
	//				modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.48f, -Application::GetInstance().GetWindowHeight() * 0.05f, 0.f);
	//				modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.04f, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
	//				RenderHelper::RenderQuest(Quest->modelMesh, Quest->QuestConversation[2], Color(1.f, 0.f, 0.f));
	//				modelStack.PopMatrix();
	//			}
	//		}
	//		break;
	//	}
	//}
}

void Quest_Manager::addQuest(CQuest * _Quest)
{
	QuestList.push_back(_Quest);
}
