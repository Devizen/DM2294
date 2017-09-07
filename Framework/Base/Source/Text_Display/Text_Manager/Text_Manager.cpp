/*Text Display was done by Lai Wei Jing, Eugene (160540A)*/
/*Tested by Chan Zhi Hao, Lim Guan Sheng, Marcus and Kwong Yu Ming*/

#include "Text_Manager.h"
#include "../Text/Text.h"
/*For rending of text.*/
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../../Application.h"
/*For keypress to skip the text.*/
#include "KeyboardController.h"
#include "MouseController.h"
#include <iostream>
/*Sound Engine to play sound for text.*/
#include "../../SoundEngine.h"

/*Adjuster for positioning of mesh.*/
#include "../../Adjuster/Adjuster.h"

using std::cout;
using std::endl;

Text_Manager::Text_Manager() :
	displayingText(false)
	, messagePrompt(0)
	, cooldown(0.f)
	, characterCount(0)
	, initialise(false)
	, textState(0)
	, playOnce(false)
	, count(0)
	, erase(false)
	, lineCount(0)
	, wordCount(0)
	, storeText("")
	, pass(true)
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
				if (text->textType == CText::TEXT_POPUP)
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

						if (textList.size() > 0)
						{
							delete text;
							text = nullptr;
							textList.pop_back();
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
				else if (text->textType == CText::TEXT_BATTLE)
				{
					bool preventCancel = false;

					if (!erase)
					{
						storeText = text->message;
						text->message = "";
						erase = true;
					}

					/*Display entire message.*/
					if ((KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || MouseController::GetInstance()->IsButtonPressed(MouseController::BUTTON_TYPE::LMB)) && text->message.size() != storeText.size())
					{
						text->textConversation[0] = "";
						text->textConversation[1] = "";
						text->textConversation[2] = "";
						text->textConversation[3] = "";
						text->textConversation[4] = "";
						text->textConversation[5] = "";

						int count = 0;
						int nextVector = 0;
						wordCount = 0;
						for (size_t i = 0; i < storeText.size(); ++i)
						{
							if (storeText[i] == '\n')
							{
								count = 0;
								++nextVector;
								continue;
							}

							if (storeText[i] == ' ')
							{
								if (count == 0)
									continue;
								++wordCount;
								pass = false;
							}


							text->textConversation[nextVector] += storeText[i];
							++count;

							if (checkList[wordCount].characterCount + count > 26 && !pass)
							{
								count = 0;
								++nextVector;
								continue;
							}
							else
								pass = true;

							if (count > 26 && nextVector < 3)
							{
								count = 0;
								++nextVector;
								continue;
							}
						}
						text->message = storeText;
						wordCount = 0;
						preventCancel = true;
					}

					//if (checkList[wordCount].characterCount + characterCount > 26)
					//{
					//	characterCount = 0;
					//	++lineCount;
					//}

					text->durationElapsed += static_cast<float>(dt);

					if (text->durationElapsed >= 0.025f && text->message.size() != storeText.size())
					{
						if (storeText[(lineCount * 26) + characterCount] == ' ')
						{
	/*						if (characterCount == 0)
							{
								++characterCount;
								return;
							}*/
							++wordCount;
							pass = false;
						}

						text->durationElapsed = 0.f;
						text->message += storeText[(lineCount * 26) + characterCount];

						text->textConversation[lineCount] += storeText[(lineCount * 26) + characterCount];
						++characterCount;

						if (checkList[wordCount].characterCount + characterCount > 26 && !pass)
						{
							characterCount -= 26;
							++lineCount;
						}
						else
							pass = true;

						//if (characterCount > 26 && lineCount < 6)
						//{
						//	characterCount = 0;
						//	return;
						//}
					}
					if (text->message.size() >= storeText.size())
					{
						if ((KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || MouseController::GetInstance()->IsButtonPressed(MouseController::BUTTON_TYPE::LMB)) && !preventCancel)
						{
							text->activateText = false;
							resetAll();
							//if (textList.size() > 0)
							//{
							//	/*Resets variables so that it will re-calculate.*/
							//	erase = false;
							//	count = 0;
							//	lineCount = 0;
							//	characterCount = 0;
							//	storeText = "";

							//	CText* _text = textList.back();
							//	delete text;
							//	text = nullptr;
							//	textList.pop_back();
							//	pass = true;
							//	displayingText = false;
							//	break;
							//}
							//else
							//{
							//	delete text;
							//	text = nullptr;
							//	textList.pop_back();
							//	pass = true;
							//	displayingText = false;
							//	break;
							//}
						}
					}
				}

				else if (text->textType == CText::TEXT_CONVERSATION)
				{
					bool preventCancel = false;

					if (!erase)
					{
						storeText = text->message;
						text->message = "";
						erase = true;
					}

					/*Display entire message.*/
					if ((KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || MouseController::GetInstance()->IsButtonPressed(MouseController::BUTTON_TYPE::LMB)) && text->message.size() != storeText.size())
					{
						text->textConversation[0] = "";
						text->textConversation[1] = "";
						text->textConversation[2] = "";

						int count = 0;
						int nextVector = 0;
						for (size_t i = 0; i < storeText.size(); ++i)
						{
							if (storeText[i] == '\n')
							{
								count = 0;
								++nextVector;
								continue;
							}
							text->textConversation[nextVector] += storeText[i];
							++count;
							if (count > 49 && nextVector < 3)
							{
								count = 0;
								++nextVector;
								continue;
							}
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
					}
					if (text->message.size() >= storeText.size())
					{
						if ((KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || MouseController::GetInstance()->IsButtonPressed(MouseController::BUTTON_TYPE::LMB)) && !preventCancel)
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
								displayingText = false;
								break;
							}
							else
							{
								delete text;
								text = nullptr;
								textList.pop_back();
								displayingText = false;
								break;
							}
						}
					}
				}

				else if (text->textType == CText::TEXT_IMPACT)
				{
					const float textSize = 0.1f;
					bool preventCancel = false;
		/*			static int characterCount = 0;
					static bool initialise = false;
					static int textState = 0;
					static bool playOnce = false;*/

					/*Push back a character to use.*/
					if (!initialise)
					{
						if (text->message.size() != text->textImpact.size())
						{
							text->textImpact.push_back(Create::Text("text", string(1, text->message[characterCount]), 0.f, 0.5f, CText::TEXT_NONE));
							text->textImpact.back()->scaleText = Application::GetInstance().GetWindowWidth();
						}
						initialise = true;
					}

					/*When the desired scale of character is met, add another character.*/
					if (text->textImpact.back()->scaleText <= (Application::GetInstance().GetWindowWidth() * textSize) && (text->message.size() != text->textImpact.size()))
					{
						if (text->textImpact.back()->message != " ")
							CSoundEngine::GetInstance()->GetSoundEngine()->play2D("Sound\\SFX\\TEXT_IMPACT.ogg");
						++characterCount;
						text->textImpact.push_back(Create::Text("text", string(1, text->message[characterCount]), 0.f, 0.5f, CText::TEXT_NONE));
						text->textImpact.back()->scaleText = Application::GetInstance().GetWindowWidth();
					}

					if (text->textImpact.size() == text->message.size() && !text->textImpact.back()->activateText)
					{
						if (!playOnce)
							CSoundEngine::GetInstance()->GetSoundEngine()->play2D("Sound\\SFX\\TEXT_IMPACT.ogg");

						playOnce = true;
					}


					if (textState == 0 /*!= text->message.size()*/)
					{
						for (vector<CText*>::iterator it = text->textImpact.begin(); it != text->textImpact.end(); ++it)
						{
							CText* _text = (CText*)*it;

							_text->durationElapsed += static_cast<float>(dt);

							if (_text->scaleText >= (Application::GetInstance().GetWindowWidth() * textSize) && _text->activateText)
								_text->scaleText -= static_cast<float>(dt) * Application::GetInstance().GetWindowWidth() * text->printSpeed;
							else
								_text->activateText = false;
						}

						int tempCount = 0;
						for (vector<CText*>::iterator it = text->textImpact.begin(); it != text->textImpact.end(); ++it)
						{
							CText* _text = (CText*)*it;

							_text->durationElapsed += static_cast<float>(dt);

							if (_text->scaleText <= (Application::GetInstance().GetWindowWidth() * textSize) && !_text->activateText)
								++tempCount;

							if (tempCount == text->message.size())
								textState = 1;
						}
					}

					if (textState == 1)
					{
						int tempCount = 0;

						for (vector<CText*>::iterator it = text->textImpact.begin(); it != text->textImpact.end(); ++it)
						{
							CText* _text = (CText*)*it;

							_text->durationElapsed += static_cast<float>(dt);

							if (_text->scaleText <= (Application::GetInstance().GetWindowWidth() * textSize * 10.f) && !_text->activateText)
								_text->scaleText += static_cast<float>(dt) * Application::GetInstance().GetWindowWidth() * text->printSpeed;
							else
								_text->activateText = true;
						}

						for (vector<CText*>::iterator it = text->textImpact.begin(); it != text->textImpact.end(); ++it)
						{
							CText* _text = (CText*)*it;

							_text->durationElapsed += static_cast<float>(dt);

							if (_text->scaleText >= (Application::GetInstance().GetWindowWidth() * textSize * 2.f) && _text->activateText)
								++tempCount;
								
							if (tempCount == text->message.size())
								textState = 2;
						}

					}

					if (textState == 2)
					{
						int tempCount = 0;
	
						for (vector<CText*>::iterator it = text->textImpact.begin(); it != text->textImpact.end(); ++it)
						{
							CText* _text = (CText*)*it;

							_text->durationElapsed += static_cast<float>(dt);

							if (_text->scaleText >= (Application::GetInstance().GetWindowWidth() * textSize) && _text->activateText)
								_text->scaleText -= static_cast<float>(dt) * Application::GetInstance().GetWindowWidth() * text->printSpeed;
							else
								_text->activateText = false;
						}

						for (vector<CText*>::iterator it = text->textImpact.begin(); it != text->textImpact.end(); ++it)
						{
							CText* _text = (CText*)*it;

							_text->durationElapsed += static_cast<float>(dt);

							if (_text->scaleText <= (Application::GetInstance().GetWindowWidth() * textSize) && !_text->activateText)
								++tempCount;
	
							if (tempCount == text->message.size())
							{
								CSoundEngine::GetInstance()->GetSoundEngine()->play2D("Sound\\SFX\\TEXT_IMPACT_END.ogg");
								textState = 3;
							}
						}
					}

					if (textState == 3)
					{
						text->durationElapsed += static_cast<float>(dt);

						if (text->durationElapsed >= 1.5f)
						{
							text->activateText = false;

							while (text->textImpact.size() > 0)
							{
								CText* _text = text->textImpact.back();
								delete _text;
								_text = nullptr;
								text->textImpact.pop_back();
							}

							if (textList.size() > 0)
							{
								delete text;
								text = nullptr;
								textList.pop_back();
								characterCount = 0;
								Text_Manager::GetInstance()->displayingText = false;
								initialise = false;
								playOnce = false;
								preventCancel = false;
								textState = 0;
								break;
							}
						}
					}

				}
				else if (text->textType == CText::TEXT_STAY)
				{
					/*if (CPlayerInfo::GetInstance()->inBoundary)
					{
						text->scaleBackground = Application::GetInstance().GetWindowWidth() * 2.f;
						text->scaleText = Application::GetInstance().GetWindowWidth() * 0.04f;
					}
					else
					{
						if (textList.size() > 0)
						{
							delete text;
							text = nullptr;
							textList.pop_back();
							Text_Manager::GetInstance()->displayingText = false;
							break;
						}
					}*/
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
				if (text->textType == CText::TEXT_POPUP)
				{
					if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() < 1.5f)
					{
						/*Quad for the text to display onto.*/
						MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
						modelStack.PushMatrix();
						modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.48f, 0.f, 0.f);
						modelStack.Scale(text->scaleBackground, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);

						Mesh* modelMesh;
						modelMesh = MeshBuilder::GetInstance()->GetMesh("TRANS_QUAD");
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
						modelMesh = MeshBuilder::GetInstance()->GetMesh("TRANS_QUAD");
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
				else if (text->textType == CText::TEXT_BATTLE)
				{

					MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

					modelStack.PushMatrix();
					modelStack.Translate(-Application::GetInstance().GetWindowWidth() * -0.1725f, -Application::GetInstance().GetWindowWidth() * 0.2535f, 0.f);
					modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.655503f, Application::GetInstance().GetWindowWidth()  * 0.255f, 1.f);

					Mesh* modelMesh;
					modelMesh = MeshBuilder::GetInstance()->GenerateCube("cube", Color(0.f, 0.f, 0.f), 1.0f);
					RenderHelper::RenderMesh(modelMesh);
					modelStack.PopMatrix();

					/*Line 1*/
					modelStack.PushMatrix();
					modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.125f,
						Application::GetInstance().GetWindowHeight() * -0.195f,
						0.f);
					modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.04f, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
					RenderHelper::RenderText(text->modelMesh, text->textConversation[0], Color(1.f, 0.f, 0.f));
					modelStack.PopMatrix();

					/*Line 2*/
					modelStack.PushMatrix();
					modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.125f,
						Application::GetInstance().GetWindowHeight() * -0.250f,
						0.f);
					modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.04f, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
					RenderHelper::RenderText(text->modelMesh, text->textConversation[1], Color(1.f, 0.f, 0.f));
					modelStack.PopMatrix();

					/*Line 3*/
					modelStack.PushMatrix();
					modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.125f,
						Application::GetInstance().GetWindowHeight() * -0.305f,
						0.f);
					modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.04f, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
					RenderHelper::RenderText(text->modelMesh, text->textConversation[2], Color(1.f, 0.f, 0.f));
					modelStack.PopMatrix();

					/*Line 4*/
					modelStack.PushMatrix();
					modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.125f,
						Application::GetInstance().GetWindowHeight() * -0.355f,
						0.f);
					modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.04f, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
					RenderHelper::RenderText(text->modelMesh, text->textConversation[3], Color(1.f, 0.f, 0.f));
					modelStack.PopMatrix();

					/*Line 5*/
					modelStack.PushMatrix();
					modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.125f,
						Application::GetInstance().GetWindowHeight() * -0.405f,
						0.f);
					modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.04f, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
					RenderHelper::RenderText(text->modelMesh, text->textConversation[4], Color(1.f, 0.f, 0.f));
					modelStack.PopMatrix();

					/*Line 6*/
					modelStack.PushMatrix();
					modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.125f,
						Application::GetInstance().GetWindowHeight() * -0.455f,
						0.f);
					modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.04f, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
					RenderHelper::RenderText(text->modelMesh, text->textConversation[5], Color(1.f, 0.f, 0.f));
					modelStack.PopMatrix();
				}
				else if (text->textType == CText::TEXT_CONVERSATION)
				{

					MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

					modelStack.PushMatrix();
					modelStack.Translate(-Application::GetInstance().GetWindowWidth() * -0.1725f, -Application::GetInstance().GetWindowWidth() * 0.2535f, 0.f);
					modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.655503f, Application::GetInstance().GetWindowWidth()  * 0.255f, 1.f);

					Mesh* modelMesh;
					modelMesh = MeshBuilder::GetInstance()->GenerateCube("cube", Color(0.f, 0.f, 0.f), 1.0f);
					RenderHelper::RenderMesh(modelMesh);
					modelStack.PopMatrix();

					modelStack.PushMatrix();
					modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.125f, 
						Application::GetInstance().GetWindowHeight() * -0.195f,
						0.f);
					modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.04f, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
					RenderHelper::RenderText(text->modelMesh, text->textConversation[0], Color(1.f, 0.f, 0.f));
					modelStack.PopMatrix();

					modelStack.PushMatrix();
					modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.125f,
						Application::GetInstance().GetWindowHeight() * -0.250f,
						0.f);
					modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.04f, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
					RenderHelper::RenderText(text->modelMesh, text->textConversation[1], Color(1.f, 0.f, 0.f));
					modelStack.PopMatrix();

					modelStack.PushMatrix();
					modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.125f,
						Application::GetInstance().GetWindowHeight() * -0.305f,
						0.f);
					modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.04f, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
					RenderHelper::RenderText(text->modelMesh, text->textConversation[2], Color(1.f, 0.f, 0.f));
					modelStack.PopMatrix();
				}

				else if (text->textType == CText::TEXT_IMPACT)
				{
					if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() < 1.5f)
					{
						float characterOffset = 0.f;
						for (vector<CText*>::iterator it = text->textImpact.begin(); it != text->textImpact.end(); ++it)
						{
							CText* character = (CText*)*it;

							/*Quad for the text to display onto.*/
							MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

							/*Display text.*/
							modelStack.PushMatrix();
							modelStack.Translate(-Application::GetInstance().GetWindowWidth() * (0.3f - characterOffset), -Application::GetInstance().GetWindowHeight() * 0.2f, 0.f);
							modelStack.Scale(character->scaleText, character->scaleText, 1.f);
							RenderHelper::RenderText(character->modelMesh, character->message, Color(1.f, 0.f, 0.f));
							modelStack.PopMatrix();

							characterOffset += 0.05f;
						}
					}
					else
					{
						float characterOffset = 0.f;
						for (vector<CText*>::iterator it = text->textImpact.begin(); it != text->textImpact.end(); ++it)
						{
							CText* character = (CText*)*it;

							/*Quad for the text to display onto.*/
							MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

							/*Display text.*/
							modelStack.PushMatrix();
							modelStack.Translate(-Application::GetInstance().GetWindowWidth() * (0.3f - characterOffset), -Application::GetInstance().GetWindowHeight() * 0.2f, 0.f);
							modelStack.Scale(character->scaleText, character->scaleText, 1.f);
							RenderHelper::RenderText(character->modelMesh, character->message, Color(1.f, 0.f, 0.f));
							modelStack.PopMatrix();

							characterOffset += 0.05f;
						}
					}
				}
				else if (text->textType == CText::TEXT_STAY)
				{
					if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() < 1.5f)
					{
						/*Quad for the text to display onto.*/
						MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
						modelStack.PushMatrix();
						modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.48f, 0.f, 0.f);
						modelStack.Scale(text->scaleBackground, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);

						Mesh* modelMesh;
						modelMesh = MeshBuilder::GetInstance()->GetMesh("TRANS_QUAD");
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
						modelMesh = MeshBuilder::GetInstance()->GetMesh("TRANS_QUAD");
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

			}
			break;
		}
	}
}

void Text_Manager::addText(CText * _text)
{
	if (textList.size() == 0)
		textList.push_back(_text);
}

void Text_Manager::CheckText(string _message)
{
	int _characterCount = 0;
	string _word = "";

	for (int i = 0; i < _message.size(); ++i)
	{
		if (_message[i] != ' ')
		{
			++_characterCount;
			_word += _message[i];
		}
		else
		{
			Text_Check check;
			check.characterCount = _characterCount;
			check.word = _word;
			checkList.push_back(check);
			_characterCount = 0;
			_word = "";
		}
	}

	Text_Check check;
	check.characterCount = _characterCount;
	check.word = _word;
	checkList.push_back(check);
	_characterCount = 0;
	_word = "";
}

void Text_Manager::resetAll(void)
{
	messagePrompt = 0;
	cooldown = 0.f;
	displayingText = false;
	characterCount = 0;
	initialise = false;
	textState = 0;
	playOnce = false;

	storeText = "";
	count = 0;
	erase = false;
	lineCount = 0;
	wordCount = 0;
	pass = true;

	if (textList.size() > 0)
	{
		CText* text = textList.back();

		while (text->textConversation.size() > 0)
		{
			text->textConversation.back() = "";
			text->textConversation.pop_back();
		}

		while (checkList.size() > 0)
		{
			checkList.back().characterCount = 0;
			checkList.back().word = "";
			checkList.pop_back();
		}

		text->message = "";
		text->durationElapsed = 0.f;

		delete text;
		text = nullptr;

		textList.pop_back();
	}
}
