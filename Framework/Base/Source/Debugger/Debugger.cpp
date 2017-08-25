#include "Debugger.h"

using std::to_string;

Debugger::Debugger() :
	debuggerMode(false)
{
}

Debugger::~Debugger()
{
}

void Debugger::AddMessage(Debug_Message* _debug)
{
	if (_debug->GetMessageType() == Debug_Message::POSITION)
	{
		string x, y, z;
		x = to_string(static_cast<int>(_debug->GetVector3().x));
		y = to_string(static_cast<int>(_debug->GetVector3().y));
		z = to_string(static_cast<int>(_debug->GetVector3().z));

		string _position = "(" + x + "," + y + "," + z + ")";
		if(_debug->GetMessage() == "")
			_debug->SetMessage("Position: ");

		messageList[_debug->GetMessage()] = _position;
		//messageList.insert(_position, _message);
	}
	else if (_debug->GetMessageType() == Debug_Message::DEFINED)
		messageList["Defined Message: "] = _debug->GetMessage();
}

void Debugger::SwitchDebuggerMode(bool _debuggerMode)
{
	debuggerMode = _debuggerMode;
}

bool Debugger::GetDebuggerMode(void)
{
	return debuggerMode;
}

void Debugger::Update()
{
	for (map<string, string>::iterator it = messageList.begin(); it != messageList.end(); ++it)
	{
		cout << it->first << it->second << endl;
	}

	if (messageList.size() > 0)
		messageList.clear();
}

//void Debugger::Update()
//{
//	for (map<Debug_Message::MESSAGE_TYPE, Vector3>::iterator messageList.begin(); it != messageList.end(); ++it)
//	{
//		if (it->first == Debug_Message::POSITION)
//			cout << "Position: " << it->second << endl;
//	}
//}
