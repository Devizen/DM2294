#include "Quest.h"
#include "Quest_Manager/Quest_Manager.h"

CQuest::CQuest(const string& _questName, const string& _questDescription, const QUEST_TYPE& _questType)
	: questName(_questName)
	, questDescription(_questDescription)
	, questType(_questType)
	, questGoalValue(0)
	, questPlayerAttributeType(CAttributes::ATTRIBUTE_TYPES::TYPE_NONE)
	, valueComparisonType(COMPARISON_TYPE::COMPARISON_NUM)
{
}

CQuest::~CQuest()
{
}

//Getters

/****************
Get quest's name
*****************/
string& CQuest::getQuestName()
{
	return questName;
}
/**********************
Get quest's description
***********************/
string& CQuest::getQuestDescription()
{
	return questDescription;
}
/****************
Get quest's type
*****************/
CQuest::QUEST_TYPE& CQuest::getQuestType()
{
	return questType;
}
/**********************************
For QUEST_PLAYER
~ Get quest's of player's attribute
***********************************/
CAttributes::ATTRIBUTE_TYPES & CQuest::getQuestPlayerType()
{
	return questPlayerAttributeType;
}
/*******************
For QUEST_PLAYER
~ Gets quest's goal
********************/
int CQuest::getQuestGoalValue()
{
	return questGoalValue;
}
/***********************
Get quest's done status
***********************/
bool CQuest::getIsQuestDone()
{
	return isQuestDone;
}
/*******************************************************************************
For player quest
~ Get comparison type between player's attribute's value and quest's goal value
*******************************************************************************/
CQuest::COMPARISON_TYPE CQuest::getComparisonType()
{
	return valueComparisonType;
}

//Setters

/***************
Set quest's name
****************/
void CQuest::setQuestName(const string & _newName)
{
	questName = _newName;
}
/**********************
Set quest's description
***********************/
void CQuest::setQuestDescription(const string & _newDescription)
{
	questDescription = _newDescription;
}
/***************
Set quest's type
Quest Types:
--QUEST_MAIN
--QUEST_sIDE
--QUEST_PLAYER
****************/
void CQuest::setQuestType(const QUEST_TYPE & _newType)
{
	questType = _newType;
}
/***************************************************
For QUEST_PLAYER
~ Set attribute type for quest for player's attribute
****************************************************/
void CQuest::setQuestPlayerAttributeType(const CAttributes::ATTRIBUTE_TYPES _playersAttribute)
{
	questPlayerAttributeType = _playersAttribute;
}
/****************************
For QUEST_PLAYER
~ Set value for quest's goal
*****************************/
void CQuest::setQuestGoalValue(int _goalValue)
{
	questGoalValue = _goalValue;
}
/**************************************
Set quest's status to done or not done
***************************************/
void CQuest::setQuestIsDone(bool _isDoneStatus)
{
	isQuestDone = _isDoneStatus;
}
/***************************************************************************************
For QUEST_PLAYER
~ Set how to compare the value between player's attribute's value and quest's goal value
****************************************************************************************/
void CQuest::setComparisonType(COMPARISON_TYPE _comparisonType)
{
	valueComparisonType = _comparisonType;
}

void CQuest::Update(double dt)
{
	if (questPlayerAttributeType != CAttributes::ATTRIBUTE_TYPES::TYPE_NONE)
	{
		for (int i = 0; i < CAttributes::ATTRIBUTE_TYPES::TYPE_NONE; ++i)
		{
			CAttributes::ATTRIBUTE_TYPES temp = (CAttributes::ATTRIBUTE_TYPES)i;
			if (temp != questPlayerAttributeType || isQuestDone)
				continue;
			switch (valueComparisonType)
			{
			case COMPARE_EQUAL:
				if (CPlayerInfo::GetInstance()->getAttribute(temp) == questGoalValue)
					isQuestDone = true;
				break;
			case COMPARE_LESS_THAN:
				if (CPlayerInfo::GetInstance()->getAttribute(temp) < questGoalValue)
					isQuestDone = true;
				break;
			case COMPARE_MORE_THAN:
				if (CPlayerInfo::GetInstance()->getAttribute(temp) > questGoalValue)
					isQuestDone = true;
				break;
			}
		}
	}
}

/****************************************************
Creates quest for player's attribute
For example - Player's Gold count reaches 1,000,000
****************************************************/
CQuest * Create::Quest(const string & _questName, const string & _questDescription, CQuest::QUEST_TYPE _questType, CAttributes::ATTRIBUTE_TYPES _playersAttributeType, int _questGoalValue)
{
	CQuest* temp = new CQuest(_questName, _questDescription, _questType);
	temp->setQuestPlayerAttributeType(_playersAttributeType);
	temp->setQuestGoalValue(_questGoalValue);
	Quest_Manager::GetInstance()->addQuest(temp);
	return temp;
}
