#ifndef GAMEPAD_CONTROLLER_H
#define GAMEPAD_CONTROLLER_H

#include "SingletonTemplate.h"
#include <bitset>
#include <windows.h>

class GamepadController : public Singleton<GamepadController>
{
	friend Singleton<GamepadController>;
public:
	const static int MAX_KEYS = 14;

	// System Interface
	void UpdateGamepadStatus(unsigned char _slot, bool _isPressed);
	void EndFrameUpdate();
	
	// User Interface
	bool IsKeyDown(unsigned char _slot);
	bool IsKeyUp(unsigned char _slot);
	bool IsKeyPressed(unsigned char _slot);
	bool IsKeyReleased(unsigned char _slot);

private:
	GamepadController();
	~GamepadController();

	std::bitset<MAX_KEYS> currStatus, prevStatus;
};

#endif // Gamepad_CONTROLLER_H