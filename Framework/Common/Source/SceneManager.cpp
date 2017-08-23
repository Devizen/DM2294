#include "SceneManager.h"
#include "Scene.h"
#include "GraphicsManager.h"

SceneManager::SceneManager() : activeScene(nullptr), nextScene(nullptr)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Update(double _dt)
{
	// Check for change of scene
	if (nextScene != activeScene)
	{
		if (activeScene)
		{
			// Scene is valid, need to call appropriate function to exit
			activeScene->Exit();
		}

		activeScene = nextScene;
		initShader();
		activeScene->Init();
	}

	if (activeScene)
		activeScene->Update(_dt);
}

void SceneManager::Render()
{
	if (activeScene)
		activeScene->Render();
}

void SceneManager::Exit()
{
	// Delete all scenes stored and empty the entire map
	activeScene->Exit();
	activeScene = nullptr;
	std::map<std::string, Scene*>::iterator it, end;
	end = sceneMap.end();
	for (it = sceneMap.begin(); it != end; ++it)
	{
		delete it->second;
	}
	sceneMap.clear();
}

void SceneManager::AddScene(const std::string& _name, Scene* _scene)
{
	if (CheckSceneExist(_name))
	{
		// Scene Exist, unable to proceed
		throw std::exception("Duplicate scene name provided");
	}

	if (_scene == nullptr)
	{
		throw std::invalid_argument("Invalid scene pointer");
	}

	// Nothing wrong, add the scene to our map
	sceneMap[_name] = _scene;
}

void SceneManager::RemoveScene(const std::string& _name)
{
	// Does nothing if it does not exist
	if (!CheckSceneExist(_name))
		return;

	Scene* target = sceneMap[_name];
	if (target == activeScene || target == nextScene)
	{
		throw std::exception("Unable to remove active/next scene");
	}

	// Delete and remove from our map
	delete target;
	sceneMap.erase(_name);
}

void SceneManager::SetActiveScene(const std::string& _name)
{
	if (!CheckSceneExist(_name))
	{
		// Scene does not exist
		throw std::exception("Scene does not exist");
	}

	// Scene exist, set the next scene pointer to that scene
	nextScene = sceneMap[_name];
}

bool SceneManager::CheckSceneExist(const std::string& _name)
{
	return sceneMap.count(_name) != 0;
}

void SceneManager::initShader(void)
{
	currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Shadow.vertexshader", "Shader//Shadow.fragmentshader");
	m_gPassShaderID = GraphicsManager::GetInstance()->LoadShader("gpass", "Shader//GPass.vertexshader", "Shader//GPass.fragmentshader");

	// Tell the shader program to store these uniform locations
	currProg->AddUniform("MVP");
	currProg->AddUniform("MV");
	currProg->AddUniform("MV_inverse_transpose");
	currProg->AddUniform("material.kAmbient");
	currProg->AddUniform("material.kDiffuse");
	currProg->AddUniform("material.kSpecular");
	currProg->AddUniform("material.kShininess");
	currProg->AddUniform("lightEnabled");
	currProg->AddUniform("numLights");
	currProg->AddUniform("lights[0].type");
	currProg->AddUniform("lights[0].position_cameraspace");
	currProg->AddUniform("lights[0].color");
	currProg->AddUniform("lights[0].power");
	currProg->AddUniform("lights[0].kC");
	currProg->AddUniform("lights[0].kL");
	currProg->AddUniform("lights[0].kQ");
	currProg->AddUniform("lights[0].spotDirection");
	currProg->AddUniform("lights[0].cosCutoff");
	currProg->AddUniform("lights[0].cosInner");
	currProg->AddUniform("lights[0].exponent");
	currProg->AddUniform("lights[1].type");
	currProg->AddUniform("lights[1].position_cameraspace");
	currProg->AddUniform("lights[1].color");
	currProg->AddUniform("lights[1].power");
	currProg->AddUniform("lights[1].kC");
	currProg->AddUniform("lights[1].kL");
	currProg->AddUniform("lights[1].kQ");
	currProg->AddUniform("lights[1].spotDirection");
	currProg->AddUniform("lights[1].cosCutoff");
	currProg->AddUniform("lights[1].cosInner");
	currProg->AddUniform("lights[1].exponent");
	currProg->AddUniform("colorTextureEnabled");
	currProg->AddUniform("colorTexture");
	currProg->AddUniform("textEnabled");
	currProg->AddUniform("textColor");

	/*Fog*/
	currProg->AddUniform("fogParam.color");
	currProg->AddUniform("fogParam.start");
	currProg->AddUniform("fogParam.end");
	currProg->AddUniform("fogParam.density");
	currProg->AddUniform("fogParam.type");
	currProg->AddUniform("fogParam.enabled");

	/*Shadow*/
	currProg->AddUniform("lightDepthMVP");
	currProg->AddUniform("shadowMap");

	m_gPassShaderID->AddUniform("lightDepthMVP");
	m_gPassShaderID->AddUniform("colorTextureEnabled[0]");
	m_gPassShaderID->AddUniform("colorTexture[0]");
	m_gPassShaderID->AddUniform("colorTextureEnabled[1]");
	m_gPassShaderID->AddUniform("colorTexture[1]");
	m_gPassShaderID->AddUniform("colorTextureEnabled[2]");
	m_gPassShaderID->AddUniform("colorTexture[2]");
}