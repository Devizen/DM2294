#ifndef QUEST_MANAGER_H
#define QUEST_MANAGER_H

#include "SingletonTemplate.h"
#include <vector>

using std::vector;

class CQuest;

class Quest_Manager : public Singleton <Quest_Manager>
{
public:
	Quest_Manager();
	~Quest_Manager();

	vector<CQuest*>&returnQuestList();
	void updateQuest(double dt);
	void renderQuest(void);
	void addQuest(CQuest* _Quest);

private:
	vector<CQuest*>QuestList;

protected:
};

#endif
