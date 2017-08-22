#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include <irrKlang.h>

using namespace irrklang;

struct GLFWwindow;

class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void Run();
	void Exit();

	void UpdateInput();
	void PostInputUpdate();
	
	static void MouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods);
	static void MouseScrollCallbacks(GLFWwindow* window, double xoffset, double yoffset);
	static void GetCursorPos(double *xpos, double *ypos);
	float GetWindowHeight();
	float GetWindowWidth();
	static bool IsKeyPressed(unsigned short key);

	int m_window_width;
	int m_window_height;
private:
	Application();
	~Application();


	//Declare a window object
	StopWatch m_timer;
};

#endif