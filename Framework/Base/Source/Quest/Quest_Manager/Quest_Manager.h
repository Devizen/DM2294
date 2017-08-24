#ifndef CQuest_Manager_H
#define CQuest_Manager_H

#include "SingletonTemplate.h"
#include <vector>

using std::vector;

class CQuest;

class CQuest_Manager : public Singleton <CQuest_Manager>
{
public:
	CQuest_Manager();
	~CQuest_Manager();

	vector<CQuest*>&returnQuestList();
	void updateQuest(double dt);
	void renderQuest(void);
	void addQuest(CQuest* _Quest);
private:
	vector<CQuest*>QuestList;
	int printPos; //For printing quest
protected:
};

#endif
