#ifndef QUEST_H
#define QUEST_H

#include <vector>
#include <string>

#include "../PlayerInfo/PlayerInfo.h"

//class Mesh;

using std::vector;
using std::string;

class CQuest: public CPlayerInfo
{
public:
	/*******************************************************
	Quest Type descriptions:
	~Quest_Main - Main quest to be completed by player
	~Quest_Side - Side quest that can be completed by player
	********************************************************/
	enum QUEST_TYPE
	{
		QUEST_MAIN = 0,
		QUEST_SIDE,/*
		QUEST_PLAYER,*/
		QUEST_NONE
	};
	enum COMPARISON_TYPE
	{
		COMPARE_EQUAL = 0,
		COMPARE_LESS_THAN,
		COMPARE_MORE_THAN,
		COMPARISON_NUM
	};
	CQuest(const string& _questName, const string& _questDescription, const QUEST_TYPE& _questType);
	~CQuest();

	//Getters
	/****************
	Get quest's name
	*****************/
	string& getQuestName();
	/**********************
	Get quest's description
	***********************/
	string& getQuestDescription();
	/****************
	Get quest's type
	*****************/
	QUEST_TYPE& getQuestType();
	/**********************************
	For player quest
	~ Get quest's of player's attribute
	***********************************/
	CAttributes::ATTRIBUTE_TYPES& getQuestPlayerType();
	/*******************
	For player quest
	~ Gets quest's goal
	********************/
	int getQuestGoalValue();
	/***********************
	Get quest's done status
	***********************/
	bool getIsQuestDone();
	/*******************************************************************************
	For player quest
	~ Get comparison type between player's attribute's value and quest's goal value
	*******************************************************************************/
	COMPARISON_TYPE getComparisonType();

	//Setters
	/***************
	Set quest's name
	****************/
	void setQuestName(const string& _newName);
	/**********************
	Set quest's description
	***********************/
	void setQuestDescription(const string& _newDescription);
	/***************
	Set quest's type
	Quest Types:
	--QUEST_MAIN
	--QUEST_SIDE
	****************/
	void setQuestType(const QUEST_TYPE& _newType);
	/***************************************************
	For QUEST_PLAYER
	~ Set attribute type for quest for player's attribute
	****************************************************/
	void setQuestPlayerAttributeType(const CAttributes::ATTRIBUTE_TYPES _playersAttribute);
	/****************************
	For QUEST_PLAYER
	~ Set value for quest's goal
	*****************************/
	void setQuestGoalValue(int _goalValue);
	/**************************************
	Set quest's status to done or not done
	***************************************/
	void setQuestIsDone(bool _isDoneStatus);
	/***************************************************************************************
	For QUEST_PLAYER
	~ Set how to compare the value between player's attribute's value and quest's goal value
	****************************************************************************************/
	void setComparisonType(COMPARISON_TYPE _comparisonType);

	void Update(double dt);

protected:
	/************
	Name of quest
	*************/
	string questName;
	/********************
	Description of quest
	*********************/
	string questDescription;
	/*******************
	Type of Quest
	Quest Types:
	--QUEST_MAIN
	--QUEST_SIDE
	*******************/
	QUEST_TYPE questType;
	/***************************************
	Boolean to denote whether quest is done
	~ true = done
	~ false = not done
	****************************************/
	bool isQuestDone;
	/*******************************************************************
	For QUEST_PLAYER
	~ A value the player have to reach for a certain attribute of theirs
	********************************************************************/
	int questGoalValue;
	/*******************************
	For QUEST_PLAYER
	~ Player's attribute for quest
	*******************************/
	CAttributes::ATTRIBUTE_TYPES questPlayerAttributeType;
	/**************************
	For QUEST_PLAYER
	~ Comparison type for value
	***************************/
	COMPARISON_TYPE valueComparisonType;
};

namespace Create
{
	//CQuest* Quest(const std::string& _meshName = "Quest", string _message = "", float _durationElapsed = 0.f, float _maxDuration = 1.f, CQuest::QUEST_TYPE _QuestType = CQuest::Quest_NONE);

	/****************************************************
	Creates quest for player's attribute
	For example - Player's Gold count reaches 1,000,000
	\param1 - Name of quest
	\param2 - Description of quest
	\param3 - Quest_Type
	\param4 - Player's attribute type
	\param5 - Target value to achieve
	****************************************************/
	CQuest* Quest(const string& _questName = "Name", const string& _questDescription = "Description of quest", CQuest::QUEST_TYPE _questType = CQuest::QUEST_NONE, bool _questIsDone = true, CAttributes::ATTRIBUTE_TYPES _playersAttributeType = CAttributes::ATTRIBUTE_TYPES::TYPE_NONE, CQuest::COMPARISON_TYPE _compareType = CQuest::COMPARISON_TYPE::COMPARISON_NUM, int _questGoalValue = 0);
	/*********************************************************
	TO DO: Overload function to create more (different) quests
	**********************************************************/
};
#endif