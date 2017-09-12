#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <map>
#include <string>

class Scene;
class ShaderProgram;

class SceneManager
{
	static SceneManager* s_instance;
public:
	// System Interface
	void Update(double _dt);
	void Render();
	void Exit();

	static SceneManager* GetInstance();

	// User Interface
	void AddScene(const std::string& _name, Scene* _scene);
	void RemoveScene(const std::string& _name);
	void SetActiveScene(const std::string& _name);
	bool CheckSceneExist(const std::string& _name);
	std::string GetActiveScene(void);
	void InitShader(void);
	//ShaderProgram* currProg;
	//ShaderProgram* m_gPassShaderID;

	void DestroyAll(void);

private:
	SceneManager();
	~SceneManager();

	std::map<std::string, Scene*> sceneMap;
	Scene* activeScene, *nextScene;
};

#endif // SCENE_MANAGER_H