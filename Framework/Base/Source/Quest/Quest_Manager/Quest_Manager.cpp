#include "Quest_Manager.h"
#include "../Quest.h"/*
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"*/
#include "../../Text_Display/Text/Text.h"
#include "../../Application.h"
#include "KeyboardController.h"
#include <iostream>

using std::cout;
using std::endl;

CQuest_Manager::CQuest_Manager()
	: printPos(0)
{
}

CQuest_Manager::~CQuest_Manager()
{
}

vector<CQuest*>& CQuest_Manager::returnQuestList()
{
	return QuestList;
}

void CQuest_Manager::updateQuest(double dt)
{
	for (vector<CQuest*>::iterator it = QuestList.begin(); it != QuestList.end(); ++it)								//Updates all quest
	{
		(*it)->Update(dt);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('B'))														//Key press 'B' renders an uncompleted quest
	{
		renderQuest();
	}
}

void CQuest_Manager::renderQuest(void)
{
	for (; printPos >= 0; ++printPos)																				//Iterate to the next quest in vector
	{
		if (printPos == QuestList.size())																			//If it reaches the questlist.size, reset to zero
		{
			printPos = 0;
			break;																									//Prevents the renderQuest() to go into infinite loop if all quests in vector are done
		}
		if (!QuestList.at(printPos)->getIsQuestDone())																//When an undone quest is encountered
		{
			string temp("");
			temp += QuestList.at(printPos)->getQuestName() + "\n" + QuestList.at(printPos)->getQuestDescription();	//Combines the quest name and description into one string
			Create::Text("text", temp, 0.f, 1.f, CText::TEXT_BATTLE);												//Use Text manger to print out Text on screen
			++printPos;																								//Increment printpos to the next quest in vector
			break;																									//Stop the loop when the text is created
		}

	}
}

void CQuest_Manager::addQuest(CQuest * _Quest)
{
	QuestList.push_back(_Quest);
}
