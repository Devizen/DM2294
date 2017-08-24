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
}

void CQuest_Manager::renderQuest(void)
{
	while(QuestList.at(printPos)->getIsQuestDone())
	{
		++printPos;
		if (printPos == QuestList.size())
			break;
	}
	string temp("");
	temp += QuestList.at(printPos)->getQuestName() + "\n" + QuestList.at(printPos)->getQuestDescription();
	Create::Text("text", temp, 0.f, 0.2f, CText::TEXT_CONVERSATION);
	++printPos;
	if (printPos == QuestList.size())
		printPos = 0;
}

void CQuest_Manager::addQuest(CQuest * _Quest)
{
	QuestList.push_back(_Quest);
}
