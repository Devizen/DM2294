#include "Debug_Message.h"

Debug_Message::Debug_Message() :
	messageType(POSITION)
	, message("")
	, vector3(0.f, 0.f, 0.f)
{
}

Debug_Message::~Debug_Message()
{
}

Vector3 Debug_Message::GetVector3(void)
{
	return vector3;
}

void Debug_Message::SetVector3(Vector3 _vector3)
{
	vector3 = _vector3;
}

Debug_Message::MESSAGE_TYPE Debug_Message::SetMessageType(MESSAGE_TYPE _messageType)
{
	return messageType = _messageType;
}

Debug_Message::MESSAGE_TYPE Debug_Message::GetMessageType(void)
{
	return messageType;
}

void Debug_Message::SetMessage(string _message)
{
	message = _message;
}

string Debug_Message::GetMessage(void)
{
	return message;
}
