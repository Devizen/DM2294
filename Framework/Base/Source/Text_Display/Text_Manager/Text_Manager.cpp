/*Text Display was done by Lai Wei Jing, Eugene (160540A)*/
/*Tested by Chan Zhi Hao, Lim Guan Sheng, Marcus and Kwong Yu Ming*/

#include "Text_Manager.h"
#include "../Text/Text.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../../Application.h"
#include "KeyboardController.h"
#include <iostream>

using std::cout;
using std::endl;

Text_Manager::Text_Manager()
{
}

Text_Manager::~Text_Manager()
{
}

vector<CText*>& Text_Manager::returnTextList()
{
	return textList;
}

void Text_Manager::updateText(double dt)
{
	if (textList.size() > 0)
	{
		for (vector<CText*>::iterator it = textList.begin(); it != textList.end();)
		{
			CText* text = (CText*)*it;

			if (text->activateText)
				if (text->textType == CText::TEXT_BATTLE)
				{
					text->durationElapsed += static_cast<float>(dt);

					/*Scale the background; animation.*/
					if (text->scaleBackground <= Application::GetInstance().GetWindowWidth() * 2.f)
						text->scaleBackground += static_cast<float>(dt) * 3000.f;

					if (text->scaleText <= Application::GetInstance().GetWindowWidth() * 0.04f)
						text->scaleText += static_cast<float>(dt) * 50.f;

					if (text->durationElapsed >= text->maxDuration)
					{
						text->activateText = false;

						if (textList.size() > 1)
						{
							CText* tempText = textList.back();

							/*Move the first text to the last in vector.*/
							textList.back() = text;
							/*First text change to last text.*/
							text = tempText;
							/*Remove first text.*/
							delete text;
							text = nullptr;
							textList.pop_back();
							/*Move last text back to the default position.*/
							textList.back() = tempText;
							break;
						}
						else
						{
							delete text;
							text = nullptr;
							textList.pop_back();
							break;
						}
					}
				}
				else if (text->textType == CText::TEXT_CONVERSATION)
				{
					static string storeText = text->message;
					static int count = 0;
					static bool erase = false;
					static int lineCount = 0;
					static int characterCount = 0;
					bool preventCancel = false;

					if (!erase)
					{
						storeText = text->message;
						text->message = "";
						erase = true;
					}

					/*Display entire message.*/
					if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) && text->message.size() != storeText.size())
					{
						text->textConversation[0] = "";
						text->textConversation[1] = "";
						text->textConversation[2] = "";

						int count = 0;
						int nextVector = 0;
						for (size_t i = 0; i < storeText.size(); ++i)
						{
							text->textConversation[nextVector] += storeText[i];
							if (count >= 50)
							{
								count = 0;
								++nextVector;
								continue;
							}
							++count;
						}
						text->message = storeText;
						preventCancel = true;
					}

					text->durationElapsed += static_cast<float>(dt);

					if (text->durationElapsed >= 0.025f && text->message.size() != storeText.size())
					{
						text->durationElapsed = 0.f;
						text->message += storeText[count];
						++characterCount;

						if (characterCount <= 50 && lineCount == 0)
							text->textConversation[0] += storeText[count];
						else if (characterCount <= 50 && lineCount == 1)
							text->textConversation[1] += storeText[count];
						else if (characterCount <= 50 && lineCount == 2)
							text->textConversation[2] += storeText[count];

						if (text->message[count] == '\n')
						{
							++lineCount;
							characterCount = 0;
						}

						if (characterCount == 50)
						{
							characterCount = 0;
							++lineCount;
						}

						++count;
						cout << "Count: " << count << " and Message: " << text->textConversation[0] << endl;
						cout << "Count: " << count << " and Message: " << text->textConversation[1] << endl;
						cout << "Count: " << count << " and Message: " << text->textConversation[2] << endl;
					}
					cout << "Original Length: " << storeText.length() << " and " << "Message Length: " << text->message.size() << endl;

					if (text->message.size() >= storeText.size())
					{
						if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) && !preventCancel)
						{
							text->activateText = false;

							if (textList.size() > 0)
							{
								/*Resets variables so that it will re-calculate.*/
								erase = false;
								count = 0;
								lineCount = 0;
								characterCount = 0;
								storeText = "";

								CText* _text = textList.back();
								delete text;
								text = nullptr;

								
								textList.pop_back();
								//CText* tempText = textList.back();

								///*Remove the strings in vector.*/
								//while (tempText->textConversation.size() != 0)
								//{
								//	tempText->textConversation.pop_back();

								//}

								///*Move the first text to the last in vector.*/
								//textList.back() = text;
								///*First text change to last text.*/
								//text = tempText;

								///*Remove first text.*/
								//delete text;
								//text = nullptr;
								//textList.pop_back();
								///*Move last text back to the default position.*/
								//textList.back() = tempText;
								break;
							}
							else
							{
								delete text;
								text = nullptr;
								textList.pop_back();
								break;
							}
						}
					}
				}
			break;
		}
	}
}

void Text_Manager::renderText(void)
{
	if (textList.size() > 0)
	{
		for (vector<CText*>::iterator it = textList.begin(); it != textList.end();)
		{
			CText* text = (CText*)*it;

			if (text->activateText)
			{
				if (text->textType == CText::TEXT_BATTLE)
				{
					if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() < 1.5f)
					{
						/*Quad for the text to display onto.*/
						MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
						modelStack.PushMatrix();
						modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.48f, 0.f, 0.f);
						modelStack.Scale(text->scaleBackground, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);

						Mesh* modelMesh;
						modelMesh = MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 1.0f), 1.0f);
						RenderHelper::RenderMesh(modelMesh);
						modelStack.PopMatrix();

						/*Display text.*/
						modelStack.PushMatrix();
						modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.48f, 0.f, 0.f);
						modelStack.Scale(text->scaleText, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
						RenderHelper::RenderText(text->modelMesh, text->message, Color(1.f, 0.f, 0.f));
						modelStack.PopMatrix();
					}
					else
					{
						/*Quad for the text to display onto.*/
						MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
						modelStack.PushMatrix();
						modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.48f, 0.f, 0.f);
						modelStack.Scale(text->scaleBackground, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);

						Mesh* modelMesh;
						modelMesh = MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 1.0f), 1.0f);
						RenderHelper::RenderMesh(modelMesh);
						modelStack.PopMatrix();

						/*Display text.*/
						modelStack.PushMatrix();
						modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.48f, 0.f, 0.f);
						modelStack.Scale(text->scaleText, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
						RenderHelper::RenderText(text->modelMesh, text->message, Color(1.f, 0.f, 0.f));
						modelStack.PopMatrix();
					}
				}
				else if (text->textType == CText::TEXT_CONVERSATION)
				{

					MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

					modelStack.PushMatrix();
					modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.48f, 0.f, 0.f);
					modelStack.Scale(Application::GetInstance().GetWindowWidth() * 2.f, Application::GetInstance().GetWindowWidth() * 0.12f, 1.f);

					Mesh* modelMesh;
					modelMesh = MeshBuilder::GetInstance()->GenerateCube("cube", Color(0.f, 0.f, 0.f), 1.0f);
					RenderHelper::RenderMesh(modelMesh);
					modelStack.PopMatrix();

					modelStack.PushMatrix();
					modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.48f, Application::GetInstance().GetWindowHeight() * 0.05f, 0.f);
					modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.04f, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
					RenderHelper::RenderText(text->modelMesh, text->textConversation[0], Color(1.f, 0.f, 0.f));
					modelStack.PopMatrix();

					modelStack.PushMatrix();
					modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.48f, 0.f, 0.f);
					modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.04f, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
					RenderHelper::RenderText(text->modelMesh, text->textConversation[1], Color(1.f, 0.f, 0.f));
					modelStack.PopMatrix();

					modelStack.PushMatrix();
					modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.48f, -Application::GetInstance().GetWindowHeight() * 0.05f, 0.f);
					modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.04f, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
					RenderHelper::RenderText(text->modelMesh, text->textConversation[2], Color(1.f, 0.f, 0.f));
					modelStack.PopMatrix();
				}
			}
			break;
		}
	}
}

void Text_Manager::addText(CText * _text)
{
	textList.push_back(_text);
}
