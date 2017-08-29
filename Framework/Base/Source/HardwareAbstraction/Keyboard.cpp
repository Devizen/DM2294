#include "Keyboard.h"
#include <iostream>
#include <sstream>
using namespace std;

#include "KeyboardController.h"
#include "../PlayerInfo/PlayerInfo.h"

#include "../Options/OptionsManager.h"
#include "../Options//Controls.h"
#include "MouseController.h"

const bool _CONTROLLER_KEYBOARD_DEBUG = false;

CKeyboard::CKeyboard()
{
}


CKeyboard::~CKeyboard()
{
}

// Create this controller
bool CKeyboard::Create(CPlayerInfo* thePlayerInfo)
{
	CController::Create(thePlayerInfo);
	if (_CONTROLLER_KEYBOARD_DEBUG)
		cout << "CKeyboard::Create()" << endl;
	return false;
}


// Read from the controller
int CKeyboard::Read(const float deltaTime)
{
	CController::Read(deltaTime);
	if (_CONTROLLER_KEYBOARD_DEBUG)
		cout << "CKeyboard::Read()" << endl;

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

			++count;
		}
	}
	once = true; 

	if (KeyboardController::GetInstance()->IsKeyDown(OptionsManager::GetInstance()->getInput(Controls::UP)))
	{
		Move_FrontBack(deltaTime, true);
	}

	else if ((KeyboardController::GetInstance()->IsKeyDown(OptionsManager::GetInstance()->getInput(Controls::UP)))
		&& (KeyboardController::GetInstance()->IsKeyDown(OptionsManager::GetInstance()->getInput(Controls::RUN))))
		Move_FrontBack(deltaTime, true, 2.0f);

	else if (KeyboardController::GetInstance()->IsKeyReleased(OptionsManager::GetInstance()->getInput(Controls::UP)))
		StopSway(deltaTime);

	if (KeyboardController::GetInstance()->IsKeyDown(OptionsManager::GetInstance()->getInput(Controls::DOWN)))
		Move_FrontBack(deltaTime, false);

	else if (KeyboardController::GetInstance()->IsKeyReleased(OptionsManager::GetInstance()->getInput(Controls::DOWN)))
		StopSway(deltaTime);

	if (KeyboardController::GetInstance()->IsKeyDown(OptionsManager::GetInstance()->getInput(Controls::LEFT)))
		Move_LeftRight(deltaTime, true);

	if (KeyboardController::GetInstance()->IsKeyDown(OptionsManager::GetInstance()->getInput(Controls::RIGHT)))
		Move_LeftRight(deltaTime, false);

	if (KeyboardController::GetInstance()->IsKeyDown(OptionsManager::GetInstance()->getInput(Controls::LOOKUP)))
		Look_UpDown(deltaTime, true);

	if (KeyboardController::GetInstance()->IsKeyDown(OptionsManager::GetInstance()->getInput(Controls::LOOKDOWN)))
		Look_UpDown(deltaTime, false);

	if (KeyboardController::GetInstance()->IsKeyDown(OptionsManager::GetInstance()->getInput(Controls::LOOKLEFT)))
		Look_LeftRight(deltaTime, true);

	if (KeyboardController::GetInstance()->IsKeyDown(OptionsManager::GetInstance()->getInput(Controls::LOOKRIGHT)))
		Look_LeftRight(deltaTime, false);

	// If the user presses SPACEBAR, then make him jump
	if (KeyboardController::GetInstance()->IsKeyDown(OptionsManager::GetInstance()->getInput(Controls::JUMP)) &&
		thePlayerInfo->isOnGround())
		Jump(true);

	// Update the weapons
	if (KeyboardController::GetInstance()->IsKeyDown(OptionsManager::GetInstance()->getInput(Controls::RELOAD)))
		Reload(deltaTime);

	if (KeyboardController::GetInstance()->IsKeyReleased(OptionsManager::GetInstance()->getInput(Controls::CHANGE)))
	{
		if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) == 0)
			MouseController::GetInstance()->setYOffset(1);
		else if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) == 1)
			MouseController::GetInstance()->setYOffset(0);

		Change(deltaTime);
	}

	// If the user presses R key, then reset the view to default values
	if (KeyboardController::GetInstance()->IsKeyDown(OptionsManager::GetInstance()->getInput(Controls::RESET)))
		Reset();

	//Posture
	if (KeyboardController::GetInstance()->IsKeyPressed(OptionsManager::GetInstance()->getInput(Controls::CROUCH)))
		Posture(deltaTime);
	//}

	if (KeyboardController::GetInstance()->IsKeyReleased(OptionsManager::GetInstance()->getInput(Controls::FIRE)))
		FirePrimary(deltaTime);
	return 0;
}
