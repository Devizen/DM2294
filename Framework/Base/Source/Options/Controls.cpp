#include "Controls.h"
#include "KeyboardController.h"
/*For debugging*/
#include <iostream>

using std::cout;
using std::endl;
#pragma once

void Controls::setKey(int _key)
{	
	key = _key;
}

void Controls::setType(string _type)
{
	if (_type == "LEFT")
		type = LEFT;

	else if (_type == "RIGHT")
		type = RIGHT;

	else if (_type == "UP")
		type = UP;

	else if (_type == "DOWN")
		type = DOWN;

	else if (_type == "LOOKLEFT")
		type = LOOKLEFT;

	else if (_type == "LOOKRIGHT")
		type = LOOKRIGHT;

	else if (_type == "LOOKUP")
		type = LOOKUP;

	else if (_type == "LOOKDOWN")
		type = LOOKDOWN;

	else if (_type == "RUN")
		type = RUN;

	else if (_type == "CROUCH")
		type = CROUCH;

	else if (_type == "JUMP")
		type = JUMP;

	else if (_type == "RELOAD")
		type = RELOAD;

	else if (_type == "CHANGE")
		type = CHANGE;

	else if (_type == "RESET")
		type = RESET;

	else if (_type == "FIRE")
		type = FIRE;
}

void Controls::set_iKey(int _key)
{
	key = _key;
}
void Controls::set_strType(string _type)
{
	str_type = _type;
}
int Controls::getKey(void)
{
	return key;
}

string Controls::getStrType(void)
{
	return str_type;
}

int Controls::getType(void)
{
	return type;
}

bool Controls::checkType(int _type)
{
	if (_type == type)
		return true;
	
	return false;
}


Controls::Controls()
{
}

Controls::Controls(string _type, int _key)
{
	if (_type == "LEFT")
		type = LEFT;

	else if (_type == "RIGHT")
		type = RIGHT;

	else if (_type == "UP")
		type = UP;

	else if (_type == "DOWN")
		type = DOWN;

	else if (_type == "LOOKLEFT")
		type = LOOKLEFT;

	else if (_type == "LOOKRIGHT")
		type = LOOKRIGHT;

	else if (_type == "LOOKUP")
		type = LOOKUP;

	else if (_type == "LOOKDOWN")
		type = LOOKDOWN;

	else if (_type == "RUN")
		type = RUN;

	else if (_type == "CROUCH")
		type = CROUCH;

	else if (_type == "JUMP")
		type = JUMP;

	else if (_type == "RELOAD")
		type = RELOAD;

	else if (_type == "CHANGE")
		type = CHANGE;

	else if (_type == "RESET")
		type = RESET;

	else if (_type == "FIRE")
		type = FIRE;

	str_type = _type;
	key = _key;
}

Controls::~Controls()
{
}
