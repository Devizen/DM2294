#include "GamepadController.h"

GamepadController::GamepadController()
{
}

GamepadController::~GamepadController()
{
}

void GamepadController::UpdateGamepadStatus(unsigned char _slot, bool _isPressed)
{
	currStatus.set(_slot, _isPressed);
}

void GamepadController::EndFrameUpdate()
{
	prevStatus = currStatus;
}

bool GamepadController::IsKeyDown(unsigned char _slot)
{
	return currStatus.test(_slot);
}

bool GamepadController::IsKeyUp(unsigned char _slot)
{
	return !currStatus.test(_slot);
}

bool GamepadController::IsKeyPressed(unsigned char _slot)
{
	return IsKeyDown(_slot) && !prevStatus.test(_slot);
}

bool GamepadController::IsKeyReleased(unsigned char _slot)
{
	return IsKeyUp(_slot) && prevStatus.test(_slot);
}