/*Text Display was done by Lai Wei Jing, Eugene (160540A)*/
/*Tested by Chan Zhi Hao, Lim Guan Sheng, Marcus and Kwong Yu Ming*/

#ifndef TEXT_MANAGER_H
#define TEXT_MANAGER_H

#include <vector>
#include <string>

using std::vector;
using std::string;

class CText;

class Text_Manager
{
	struct Text_Check
	{
		string word = "";
		int characterCount = 0;
	};

	static Text_Manager* s_instance;

public:
	Text_Manager();
	~Text_Manager();

	static Text_Manager* GetInstance(void);

	vector<CText*>&returnTextList();
	void updateText(double dt);
	void renderText(void);
	void addText(CText* _text);
	void CheckText(string _message);
	bool displayingText;

	size_t messagePrompt;
	float cooldown;
	void resetAll(void);

	int characterCount;
	bool initialise;
	int textState;
	bool playOnce;

	string storeText;
	int count;
	bool erase;
	int lineCount;
	int wordCount;
	bool pass;
	void DestroyAll(void);
private:
	vector<CText*>textList;
	vector<Text_Check>checkList;

protected:
};

#endif
