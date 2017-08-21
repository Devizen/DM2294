#ifndef GAMEPAD_H
#define GAMEPAD_H

#include "Controller.h"

#include <fstream>
#include <string>
#include <windows.h>
#include <Xinput.h>

static const WORD XINPUT_Buttons[] = {
	XINPUT_GAMEPAD_A,
	XINPUT_GAMEPAD_B,
	XINPUT_GAMEPAD_X,
	XINPUT_GAMEPAD_Y,
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT,
	XINPUT_GAMEPAD_DPAD_RIGHT,
	XINPUT_GAMEPAD_LEFT_SHOULDER,
	XINPUT_GAMEPAD_RIGHT_SHOULDER,
	XINPUT_GAMEPAD_LEFT_THUMB,
	XINPUT_GAMEPAD_RIGHT_THUMB,
	XINPUT_GAMEPAD_START,
	XINPUT_GAMEPAD_BACK
};

struct XButtonIDs
{
	//XButtonIDs();
	int A = 0;
	int B = 1;
	int X = 2;
	int Y = 3;
	int DPad_Up = 4;
	int DPad_Down = 5;
	int DPad_Left = 6;
	int DPad_Right = 7;
	int L_Shoulder = 8;
	int R_Shoulder = 9;
	int L_Thumbstick = 10;
	int R_Thumbstick = 11;
	int Start = 12;
	int Back = 13;
};
using std::string;

class CGamepad :
	public CController
{
private:
	XINPUT_STATE m_State;
	int m_iGamepadIndex;
public:
	CGamepad();
	CGamepad(int _index);
	virtual ~CGamepad();

	XINPUT_STATE GetState();
	int GetIndex();
	bool isConnected();

	void UpdateGamePadState();

	bool LStick_InDeadZone();
	bool RStick_InDeadZone();

	float LeftStick_X();
	float LeftStick_Y();
	float RightStick_X();
	float RightStick_Y();

	float LeftTrigger();
	float RightTrigger();

/*
	bool GetButtonPressed(int _button);
*/
	void Rumble(float _LeftMotor = 0.0f, float _RightMotor = 0.0f);

	// Create this controller
	virtual bool Create(CPlayerInfo* thePlayerInfo = NULL);
	// Read from the controller
	virtual int Read(const float deltaTime);
};

#endif
extern XButtonIDs XButtons;