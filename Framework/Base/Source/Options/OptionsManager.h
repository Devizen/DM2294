#ifndef OPTIONSMANAGER_H
#define OPTIONSMANAGER_H

#include <vector>
#include <string>
#include "SingletonTemplate.h"
using std::vector;
using std::string;

/*Forward Declaration*/
class Controls;

class OptionsManager : public Singleton<OptionsManager>
{
public:
	OptionsManager();
	~OptionsManager();
	vector<Controls*>&getControls(void);

	/*Setters*/
	Controls* &setInput(int _type);
	void setEditingState(bool _editingState);

	/*Getters*/
	int getInput(int _type);
	bool getEditingState(void);

	bool loadConfig(string filePath);
	bool saveConfig(void);
	bool defaultConfig(void);

	/*Highscore*/
	bool loadHighscore(string filePath);
	bool saveHighscore(void);
	void setHighscore(int _highscore);
	int getHighscore(void);

private:
	static OptionsManager* s_instance;
	vector<Controls*> controls;
	bool editingState;
	int highscore;
};

#endif