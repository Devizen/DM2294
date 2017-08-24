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
	for (vector<CQuest*>::iterator it = QuestList.begin(); it != QuestList.end(); ++it)
	{
		(*it)->Update(dt);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('B'))
	{
		renderQuest();
	}
}

void CQuest_Manager::renderQuest(void)
{
	for (; printPos >= 0; ++printPos)
	{
		if (printPos == QuestList.size())
		{
			printPos = 0;
		}
		if (!QuestList.at(printPos)->getIsQuestDone())
		{
			string temp("");
			temp += QuestList.at(printPos)->getQuestName() + "\n" + QuestList.at(printPos)->getQuestDescription();
			Create::Text("text", temp, 0.f, 0.2f, CText::TEXT_CONVERSATION);
			++printPos;
			break;
		}

	}
}

void CQuest_Manager::addQuest(CQuest * _Quest)
{
	QuestList.push_back(_Quest);
}
