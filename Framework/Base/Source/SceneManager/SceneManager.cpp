#include "SceneManager.h"
#include "KeyboardController.h"
#include "..//Application.h"

const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

SceneManager *SceneManager::_instance = 0;

SceneManager::SceneManager() : _hasPendingScene(false)
{

}

SceneManager::~SceneManager()
{
	delete _instance;
}

void SceneManager::Update() {

	m_timer.startTimer(); // Start timer to calculate how long it takes to render this frame

	while (!glfwWindowShouldClose(glfwGetCurrentContext()) && !Application::GetInstance().IsKeyPressed(VK_ESCAPE))
	{
		glfwPollEvents();

		Application::GetInstance().UpdateInput();

		float dt = m_timer.getElapsedTime();
		if (dt > 0.015f)
			dt = 0.015f;

		currentScene->Update(dt);
		currentScene->Render();
		//Swap buffers
		glfwSwapBuffers(glfwGetCurrentContext());
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		Application::GetInstance().PostInputUpdate();

		m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.  

											// Change scene if there is any pending scene
		if (_hasPendingScene) {
			currentScene->Exit();
			delete currentScene;
			currentScene = pendingScene;
			currentScene->Init();
			_hasPendingScene = false;
			pendingScene = nullptr;
		}

	} //Check if the ESC key had been pressed or if the window had been closed

	currentScene->Exit();
	delete currentScene;
}

void SceneManager::changeScene(Scene* scene) {
	if (!currentScene) {
		currentScene = scene;
		scene->Init();
	}
	else {
		pendingScene = scene;
		_hasPendingScene = true;
	}
}

SceneManager* SceneManager::getInstance() {
	if (!_instance) {
		_instance = new SceneManager();
	}
	return _instance;
}

bool SceneManager::hasPendingScene()
{
	return _hasPendingScene;
}