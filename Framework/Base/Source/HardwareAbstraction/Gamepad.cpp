#include "Gamepad.h"
#pragma comment(lib, "Xinput.lib")
#include <iostream>
#include <sstream>
using namespace std;

#include "GamepadController.h"
//#include "../PlayerInfo/PlayerInfo.h"

#include "../Options/OptionsManager.h"
#include "../Options//Controls.h"
//#include "MouseController.h"

const bool _CONTROLLER_KEYBOARD_DEBUG = false;

XButtonIDs XButtons;

CGamepad::CGamepad()
{
}

CGamepad::CGamepad(int _index)
{
	m_iGamepadIndex = _index - 1;
}

CGamepad::~CGamepad()
{
}

XINPUT_STATE CGamepad::GetState()
{
	XINPUT_STATE GamepadState;

	ZeroMemory(&GamepadState, sizeof(XINPUT_STATE));

	XInputGetState(m_iGamepadIndex, &GamepadState);

	return GamepadState;
}

int CGamepad::GetIndex()
{
	return m_iGamepadIndex;
}
/*
returns true if a gamepad device is found connected
*/
bool CGamepad::isConnected()
{
	ZeroMemory(&m_State, sizeof(XINPUT_STATE));
	DWORD Result = XInputGetState(m_iGamepadIndex, &m_State);
	if (Result /*== ERROR_SUCCESS*/)
		return true;
	else
		return false;
}

void CGamepad::UpdateGamePadState()
{
	m_State = GetState();
}
/*
returns true if left stick in deadzone
*/
bool CGamepad::LStick_InDeadZone()
{
	short sX = m_State.Gamepad.sThumbLX;
	short sY = m_State.Gamepad.sThumbLY;

	if (sX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || sX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		return false;
	if (sY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || sY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		return false;
	return true;
}
/*
returns true if right stick in deadzone
*/
bool CGamepad::RStick_InDeadZone()
{
	short sX = m_State.Gamepad.sThumbRX;
	short sY = m_State.Gamepad.sThumbRY;

	if (sX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || sX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		return false;
	if (sY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || sY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		return false;
	return true;
}
/*
returns left stick's x value in float (from short)
*/
float CGamepad::LeftStick_X()
{
	short sX = m_State.Gamepad.sThumbLX;
	return (static_cast<float>(sX) / 32768.0f);
}
/*
returns left stick's y value in float (from short)
*/
float CGamepad::LeftStick_Y()
{
	short sY = m_State.Gamepad.sThumbLY;
	return (static_cast<float>(sY) / 32768.0f);
}
/*
returns right stick's x value in float (from short)
*/
float CGamepad::RightStick_X()
{
	short sX = m_State.Gamepad.sThumbRX;
	return (static_cast<float>(sX) / 32768.0f);
}
/*
returns right stick's y value in float (from short)
*/
float CGamepad::RightStick_Y()
{
	short sY = m_State.Gamepad.sThumbRY;
	return (static_cast<float>(sY) / 32768.0f);
}
/*
returns left trigger's value ranging from 0.0f - 1.0f
*/
float CGamepad::LeftTrigger()
{
	BYTE trigger = m_State.Gamepad.bLeftTrigger;

	if (trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return trigger / 255.0f;
	return 0.0f;
}
/*
returns right trigger's value ranging from 0.0f - 1.0f
*/
float CGamepad::RightTrigger()
{
	BYTE trigger = m_State.Gamepad.bRightTrigger;

	if (trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return trigger / 255.0f;
	return 0.0f;
}
/*
Activates controller rmble effect
parameter values range between 0.0f - 1.0f
NOTE:
Call function with no parameters to cancel vibration
*/
void CGamepad::Rumble(float _LeftMotor, float _RightMotor)
{
	XINPUT_VIBRATION vibrationState;

	ZeroMemory(&vibrationState, sizeof(XINPUT_VIBRATION));

	int iLeftMotor = int(_LeftMotor * 65535.f);
	int iRightMotor = int(_RightMotor * 65535.f);

	vibrationState.wLeftMotorSpeed = iLeftMotor;
	vibrationState.wRightMotorSpeed = iRightMotor;

	XInputSetState(m_iGamepadIndex, &vibrationState);
}

// Create this controller
bool CGamepad::Create(CPlayerInfo* thePlayerInfo)
{
	CController::Create(thePlayerInfo);
	if (_CONTROLLER_KEYBOARD_DEBUG)
		cout << "CGamepad::Create()" << endl;
	return false;
}


// Read from the controller
int CGamepad::Read(const float deltaTime)
{
	CController::Read(deltaTime);
	if (_CONTROLLER_KEYBOARD_DEBUG)
		cout << "CGamepad::Read()" << endl;

	/*Debugging Keyboard Controls*/
	/*vector<Controls*>controls = OptionsManager::GetInstance()->getControls();

	int count = 0;
	static bool once = false;
	if (!once)
	{
	for (vector<Controls*>::iterator it = controls.begin(); it != controls.end(); ++it)
	{
	Controls* key = (Controls*)*it;

	cout << count << ": " << key->getKey() << endl;
	cout << "Type: " << key->GetType() << endl;
	++count;
	}
	}
	once = true;*/

	/*Using customised keys from Options Manager*/
	/*Only activate controls when player is not editing the keys.*/
	//if (!OptionsManager::GetInstance()->getEditingState())
	//{

	int count = 0;
	static bool once = false;
	if (!once)
	{

		for (vector<Controls*>::iterator it = OptionsManager::GetInstance()->getControls().begin(); it != OptionsManager::GetInstance()->getControls().end(); ++it)
		{
			Controls* key = (Controls*)*it;

			cout << count << ": " << key->getKey() << endl;
			cout << "Type: " << key->GetType() << endl;
			++count;
		}
	}
	once = true;

	m_State = GetState();
	if (!LStick_InDeadZone())//Left Stick movement
	{
		/*if (GamepadController::GetInstance()->IsKeyDown(OptionsManager::GetInstance()->getInput(Controls::UP)))
			Move_FrontBack(deltaTime, true);

		else if ((GamepadController::GetInstance()->IsKeyDown(OptionsManager::GetInstance()->getInput(Controls::UP)))
			&& (GamepadController::GetInstance()->IsKeyDown(OptionsManager::GetInstance()->getInput(Controls::RUN))))
			Move_FrontBack(deltaTime, true, 2.0f);

		else if (GamepadController::GetInstance()->IsKeyReleased(OptionsManager::GetInstance()->getInput(Controls::UP)))
			StopSway(deltaTime);

		if (GamepadController::GetInstance()->IsKeyDown(OptionsManager::GetInstance()->getInput(Controls::DOWN)))
			Move_FrontBack(deltaTime, false);

		else if (GamepadController::GetInstance()->IsKeyReleased(OptionsManager::GetInstance()->getInput(Controls::DOWN)))
			StopSway(deltaTime);

		if (GamepadController::GetInstance()->IsKeyDown(OptionsManager::GetInstance()->getInput(Controls::LEFT)))
			Move_LeftRight(deltaTime, true);

		if (GamepadController::GetInstance()->IsKeyDown(OptionsManager::GetInstance()->getInput(Controls::RIGHT)))
			Move_LeftRight(deltaTime, false);*/

		Move_FrontBack(deltaTime, true, LeftStick_X());
		Move_LeftRight(deltaTime, true, LeftStick_Y());
	}
	else
	{
		StopSway(deltaTime);
	}
	if (!RStick_InDeadZone())//Right Stick camera
	{
		/*if (GamepadController::GetInstance()->IsKeyDown(OptionsManager::GetInstance()->getInput(Controls::LOOKUP)))
			Look_UpDown(deltaTime, true);

		if (GamepadController::GetInstance()->IsKeyDown(OptionsManager::GetInstance()->getInput(Controls::LOOKDOWN)))
			Look_UpDown(deltaTime, false);

		if (GamepadController::GetInstance()->IsKeyDown(OptionsManager::GetInstance()->getInput(Controls::LOOKLEFT)))
			Look_LeftRight(deltaTime, true);

		if (GamepadController::GetInstance()->IsKeyDown(OptionsManager::GetInstance()->getInput(Controls::LOOKRIGHT)))
			Look_LeftRight(deltaTime, false);*/
		Look_UpDown(deltaTime, true, RightStick_Y());
		Look_LeftRight(deltaTime, true, RightStick_X());
	}

	// If the user presses A, then make him jump
	if (GamepadController::GetInstance()->IsKeyDown(XINPUT_Buttons[0]) &&
		thePlayerInfo->isOnGround())
		Jump(true);

	// If the user presses B, reload
	if (GamepadController::GetInstance()->IsKeyDown(XINPUT_Buttons[1]))
		Reload(deltaTime);

	// If the user presses Y, change weapon
	if (GamepadController::GetInstance()->IsKeyReleased(XINPUT_Buttons[3]))
	{
		/*if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) == 0)
			MouseController::GetInstance()->setYOffset(1);
		else if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) == 1)
			MouseController::GetInstance()->setYOffset(0);*/
		Change(deltaTime);
	}

	// If the user presses R key, then reset the view to default values
	if (GamepadController::GetInstance()->IsKeyDown(OptionsManager::GetInstance()->getInput(Controls::RESET)))
		Reset();

	//If the user presses X, change Posture
	if (GamepadController::GetInstance()->IsKeyPressed(XINPUT_Buttons[2]))
		Posture(deltaTime);
	//}

	//if (KeyboardController::GetInstance()->IsKeyReleased(OptionsManager::GetInstance()->getInput(Controls::FIRE)))
	if(RightTrigger() > 0.0f) //Range 0.0f(tap action) - 0.9f(push action)
		FirePrimary(deltaTime);
	return 0;
}

