#include "Text_Manager.h"
#include "../Text/Text.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../../Application.h"

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
