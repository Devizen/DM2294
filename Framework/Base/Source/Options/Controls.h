#ifndef CONTROLS_H
#define CONTROLS_H

#include <string>

using std::string;

class Controls
{
public:

	enum KEYS
	{
		LEFT = 0,
		RIGHT,
		UP,
		DOWN,
		LOOKLEFT,
		LOOKRIGHT,
		LOOKUP,
		LOOKDOWN,
		RUN,
		CROUCH,
		JUMP,
		RELOAD,
		CHANGE,
		RESET,
		FIRE,
		NONE
	};

	void setKey(int _key);
	void set_iKey(int _key);
	void set_strType(string _type);
	void SetType(string _type);

	int getKey(void);
	string getStrType(void);
	int GetType(void);
	bool checkType(int _type);

	Controls();
	Controls(string _type, int _key);
	~Controls();
private:
	string str_type;
	KEYS type;
	int key;
};

#endif