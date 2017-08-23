/*Text Display was done by Lai Wei Jing, Eugene (160540A)*/
/*Tested by Chan Zhi Hao, Lim Guan Sheng, Marcus and Kwong Yu Ming*/

#ifndef TEXT_MANAGER_H
#define TEXT_MANAGER_H

#include "SingletonTemplate.h"
#include <vector>

using std::vector;

class CText;

class Text_Manager : public Singleton <Text_Manager>
{
public:
	Text_Manager();
	~Text_Manager();

	vector<CText*>&returnTextList();
	void updateText(double dt);
	void renderText(void);
	void addText(CText* _text);
	bool displayingText;

	size_t messagePrompt;
	float cooldown;
	void resetAll(void);

private:
	vector<CText*>textList;

protected:
};

#endif
