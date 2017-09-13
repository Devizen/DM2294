#ifndef KEYBOARD_CONTROLLER_H
#define KEYBOARD_CONTROLLER_H

#include <bitset>
#include <windows.h>

class KeyboardController
{
	static KeyboardController* s_instance;
public:
	const static int MAX_KEYS = 256;

	static KeyboardController* GetInstance(void);

	// System Interface
	void UpdateKeyboardStatus(unsigned char _slot, bool _isPressed);
	void EndFrameUpdate();
	
	// User Interface
	bool IsKeyDown(unsigned char _slot);
	bool IsKeyUp(unsigned char _slot);
	bool IsKeyPressed(unsigned char _slot);
	bool IsKeyReleased(unsigned char _slot);

	void DestroyAll(void);
private:
	KeyboardController();
	~KeyboardController();

	std::bitset<MAX_KEYS> currStatus, prevStatus;
};

#endif // KEYBOARD_CONTROLLER_H