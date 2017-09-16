#include "Application.h"
#include "MouseController.h"
#include "KeyboardController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "SoundEngine.h"

/*For deleting singletons.*/
#include "Cinematic\Cinematic.h"
#include "Debugger\Debugger.h"
#include "Text_Display\Text_Manager\Text_Manager.h"
#include "FileManager.h"
#include "Map_Editor\Map_Editor.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "Enemy\Enemy_Manager.h"
#include "ShaderProgram.h"
#include "DepthFBO.h"
#include "Battle\Bullet_Manager.h"
#include "Adjuster\Adjuster.h"
#include "Editor\Battle_Editor.h"
//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

//#include "vld.h"

GLFWwindow* m_window;

const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

										   //Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	Application::GetInstance().m_window_width = w;
	Application::GetInstance().m_window_height = h;
	glViewport(0, 0, w, h);
}

bool Application::IsKeyPressed(unsigned short key)
{
	return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

void Application::MakeFullScreen(void)
{
	/*Get Monitor*/
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	m_window_width = mode->width;
	m_window_height = mode->height;
	//glfwSetWindowSize(m_window, m_window_width, m_window_height);
	
	glfwSetWindowMonitor(m_window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
	screenMode = true;
}

void Application::MakeWindowedMode(void)
{
	/*Get Monitor*/
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	m_window_width = 800;
	m_window_height = 600;
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	//glfwSetWindowSize(m_window, m_window_width, m_window_height);
	glfwSetWindowMonitor(m_window, NULL, 0, 0, m_window_width, m_window_height, mode->refreshRate);
	screenMode = false;
}

Application::Application() :
	screenMode(false)
{
}

Application::~Application()
{
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
												   //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	/*Get Monitor*/
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	//Create a window and create its OpenGL context
	//m_window_width = mode->width;
	//m_window_height = mode->height;
	m_window_width = 800;
	m_window_height = 600;

	/*glfwGetPrimaryMonitor() to set full screen.*/
	//m_window = glfwCreateWindow(m_window_width, m_window_height, "DM2294", glfwGetPrimaryMonitor(), NULL);

	/*Windowed Full-Screen*/
	//m_window = glfwCreateWindow(mode->width, mode->height, "DM2294", NULL, NULL);

	/*Windowed Mode*/
	m_window = glfwCreateWindow(m_window_width, m_window_height, "DM2294", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	// Set windows position
	glfwSetWindowPos(m_window, 10, 30);

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glewExperimental = true; // Needed for core profile
							 //Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	glfwSetMouseButtonCallback(m_window, &Application::MouseButtonCallbacks);
	glfwSetScrollCallback(m_window, &Application::MouseScrollCallbacks);

	/*Full screen*/
	//system("650");

	// Init systems
	GraphicsManager::GetInstance()->Init();

	/*Init Sounds*/
	CSoundEngine::GetInstance()->AddSound("PISTOL", "Sound\\SFX\\PISTOL.ogg");
	CSoundEngine::GetInstance()->AddSound("ASSAULT", "Sound\\SFX\\ASSAULT.ogg");
	CSoundEngine::GetInstance()->AddSound("TAKEDAMAGE", "Sound\\SFX\\TAKEDAMAGE.ogg");

	/*Enable mouse cursor to debug.*/
	//glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// Hide the cursor
	//glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	/*Disable and lock mouse inside window for mouse movement.*/
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Application::Run()
{

	SceneManager::GetInstance()->SetActiveScene("World");
	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
	{

		glfwPollEvents();
		UpdateInput();

		/*Switching between Fullscreen and Windowed mode.*/
		if (KeyboardController::GetInstance()->IsKeyDown(VK_LMENU) && KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) && !screenMode)
			MakeFullScreen();
		else if (KeyboardController::GetInstance()->IsKeyDown(VK_LMENU) && KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) && screenMode)
			MakeWindowedMode();
		float dt = static_cast<float>(m_timer.getElapsedTime());
		if (dt > 0.015f)
			dt = 0.015f;

		SceneManager::GetInstance()->Update(dt);

		SceneManager::GetInstance()->Render();

		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...

		PostInputUpdate();
		m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   
	}
	SceneManager::GetInstance()->Exit();
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();

	/*Destroying Singletons to prevent memory leaks.*/
	SceneManager::GetInstance()->DestroyAll();
	Text_Manager::GetInstance()->DestroyAll();
	MeshBuilder::GetInstance()->Destroy();
	GraphicsManager::GetInstance()->removeLightMap();
	GraphicsManager::GetInstance()->removeShaderMap();
	GraphicsManager::GetInstance()->Destroy();
	RenderHelper::GetInstance()->Destroy();
	CEnemy_Manager::GetInstance()->DestroyAll();
	ShaderProgram::GetInstance()->Destroy();
	Debugger::GetInstance()->Destroy();
	DepthFBO::GetInstance()->Destroy();
	CBullet_Manager::GetInstance()->DestroyAll();
	CSoundEngine::GetInstance()->DestroyAll();
	CAdjuster::GetInstance()->DestroyAll();
	KeyboardController::GetInstance()->DestroyAll();
	MouseController::GetInstance()->DestroyAll();
	CBattle_Editor::GetInstance()->DestroyAll();
	//CCinematic::GetInstance()->Destroy();
	//ParticleManager::GetInstance()->Destroy();
	//SceneManager::GetInstance()->Destroy();
	//Text_Manager::GetInstance()->Destroy();
	//EquipmentManager::GetInstance()->Destroy();
	//Inventory::GetInstance()->Destroy();
	//Map_Editor::GetInstance()->Destroy();
	//MeshBuilder::GetInstance()->Destroy();
	//GraphicsManager::GetInstance()->Destroy();
	//CCameraEffects::GetInstance()->Destroy();
}

void Application::UpdateInput()
{
	// Update Mouse Position
	double mouse_currX, mouse_currY;
	glfwGetCursorPos(m_window, &mouse_currX, &mouse_currY);
	MouseController::GetInstance()->UpdateMousePosition(mouse_currX, mouse_currY);

	// Update Keyboard Input
	for (int i = 0; i < KeyboardController::MAX_KEYS; ++i)
		KeyboardController::GetInstance()->UpdateKeyboardStatus(i, IsKeyPressed(i));
}

void Application::PostInputUpdate()
{
	// If mouse is centered, need to update the center position for next frame
	if (MouseController::GetInstance()->GetKeepMouseCentered())
	{
		double mouse_currX, mouse_currY;
		mouse_currX = m_window_width >> 1;
		mouse_currY = m_window_height >> 1;
		MouseController::GetInstance()->UpdateMousePosition(mouse_currX, mouse_currY);
		glfwSetCursorPos(m_window, mouse_currX, mouse_currY);
	}

	// Call input systems to update at end of frame
	MouseController::GetInstance()->EndFrameUpdate();
	KeyboardController::GetInstance()->EndFrameUpdate();
}

void Application::MouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods)
{
	// Send the callback to the mouse controller to handle
	if (action == GLFW_PRESS)
		MouseController::GetInstance()->UpdateMouseButtonPressed(button);
	else
		MouseController::GetInstance()->UpdateMouseButtonReleased(button);
}

void Application::MouseScrollCallbacks(GLFWwindow* window, double xoffset, double yoffset)
{
	MouseController::GetInstance()->UpdateMouseScroll(xoffset, yoffset);
}

float Application::GetWindowHeight()
{
	return static_cast<float>(m_window_height);
}

float Application::GetWindowWidth()
{
	return static_cast<float>(m_window_width);
}

void Application::GetCursorPos(double *xpos, double *ypos)
{
	glfwGetCursorPos(m_window, xpos, ypos);
}