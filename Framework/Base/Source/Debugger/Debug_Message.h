/*Debugger was made by Lai Wei Jing, Eugene 160540A.*/

#ifndef DEBUG_MESSAGE_H
#define DEBUG_MESSAGE_H
#include <string>
#include "Vector3.h"

using std::string;

class Debug_Message
{
public:
	Debug_Message();
	~Debug_Message();

	enum MESSAGE_TYPE
	{
		POSITION = 0,
		DEFINED
	};

	Vector3 GetVector3(void);
	void SetVector3(Vector3 _vector3);

	MESSAGE_TYPE SetMessageType(MESSAGE_TYPE _messageType);
	MESSAGE_TYPE GetMessageType(void);

	void SetMessage(string _message);
	string GetMessage(void);
private:
	Vector3 vector3;
	MESSAGE_TYPE messageType;
	string message;
};

#endif