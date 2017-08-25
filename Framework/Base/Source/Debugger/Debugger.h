/*Debugger was made by Lai Wei Jing, Eugene 160540A.*/

#ifndef DEBUGGER_H
#define DEBUGGER_H
#include <iostream>
#include <string>
#include <vector>
#include "Vector3.h"
#include <map>
#include "SingletonTemplate.h"
#include "Debug_Message.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;

class Debugger : public Singleton<Debugger>, public Debug_Message
{
public:
	Debugger();
	~Debugger();

	void AddMessage(Debug_Message* _debug);

	void SwitchDebuggerMode(bool _debuggerMode);
	bool GetDebuggerMode(void);
	void Update();
private:
	bool debuggerMode;
	map<string,string>messageList;
protected:
};

#endif