#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"
#include "timer.h"
#include "..//Application.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"

class SceneManager
{
private:
	void Update();
	Scene* currentScene = 0;
	Scene* pendingScene = 0;

	bool _hasPendingScene;

	StopWatch m_timer;
	friend class Application; //make stuff in application accessible in Scenemanager
	static SceneManager* _instance;

public:
	static SceneManager* getInstance();
	bool hasPendingScene();
	void changeScene(Scene* scene);

protected:
	SceneManager();
	~SceneManager();
};

#endif // !SCENEMANAGER_H