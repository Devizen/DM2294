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

private:
	vector<CText*>textList;

protected:
};

#endif
