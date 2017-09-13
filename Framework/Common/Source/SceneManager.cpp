#include "SceneManager.h"
#include "Scene.h"

#include "GLFW\glfw3.h"
#include "ShaderProgram.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "Mesh.h"
#include "LoadTGA.h"

#include "../../Base/Source/Debugger/Debugger.h"
#include "KeyboardController.h"

SceneManager* SceneManager::s_instance = 0;

void SceneManager::DestroyAll(void)
{
	for (std::map<std::string, Scene*>::iterator it = sceneMap.begin(); it != sceneMap.end();)
	{
		if (it->second != nullptr)
		{
			Scene* scene = it->second;
			delete scene;
			scene = nullptr;
			it = sceneMap.erase(it);
			continue;
		}
		++it;
	}

	if (activeScene)
	{
		delete activeScene;
		activeScene = nullptr;
	}

	if (this)
		delete this;
}

SceneManager::SceneManager() : activeScene(nullptr), nextScene(nullptr)
//, currProg(nullptr)
//, m_gPassShaderID(nullptr)
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
		InitShader();
		activeScene->Init();
	}

	if (activeScene)
	{
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RCONTROL) && !Debugger::GetInstance()->GetDebuggerMode())
			Debugger::GetInstance()->SwitchDebuggerMode(true);
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_RCONTROL) && Debugger::GetInstance()->GetDebuggerMode())
			Debugger::GetInstance()->SwitchDebuggerMode(false);

		/*Displaying Debug Messages.*/
		if (Debugger::GetInstance()->GetDebuggerMode())
			Debugger::GetInstance()->Update();

		activeScene->Update(_dt);
	}
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

SceneManager * SceneManager::GetInstance()
{
	if (s_instance == nullptr)
		s_instance = new SceneManager();

	return s_instance;
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

string SceneManager::GetActiveScene(void)
{
	std::string sceneName = "";
	for (std::map<std::string, Scene*>::iterator it = sceneMap.begin(); it != sceneMap.end(); ++it)
	{
		if (activeScene != it->second)
			continue;

		sceneName = it->first;
		break;
	}
	return sceneName;
}

void SceneManager::InitShader(void)
{
	//currProg = ShaderProgram::GetInstance()->currProg;
	//m_gPassShaderID = ShaderProgram::GetInstance()->m_gPassShaderID;

	ShaderProgram::GetInstance()->currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Shadow.vertexshader", "Shader//Shadow.fragmentshader");
	ShaderProgram::GetInstance()->m_gPassShaderID = GraphicsManager::GetInstance()->LoadShader("gpass", "Shader//GPass.vertexshader", "Shader//GPass.fragmentshader");

	// Tell the shader program to store these uniform locations
	ShaderProgram::GetInstance()->currProg->AddUniform("MVP");
	ShaderProgram::GetInstance()->currProg->AddUniform("MV");
	ShaderProgram::GetInstance()->currProg->AddUniform("MV_inverse_transpose");
	ShaderProgram::GetInstance()->currProg->AddUniform("material.kAmbient");
	ShaderProgram::GetInstance()->currProg->AddUniform("material.kDiffuse");
	ShaderProgram::GetInstance()->currProg->AddUniform("material.kSpecular");
	ShaderProgram::GetInstance()->currProg->AddUniform("material.kShininess");
	ShaderProgram::GetInstance()->currProg->AddUniform("lightEnabled");
	ShaderProgram::GetInstance()->currProg->AddUniform("numLights");
	ShaderProgram::GetInstance()->currProg->AddUniform("lights[0].type");
	ShaderProgram::GetInstance()->currProg->AddUniform("lights[0].position_cameraspace");
	ShaderProgram::GetInstance()->currProg->AddUniform("lights[0].color");
	ShaderProgram::GetInstance()->currProg->AddUniform("lights[0].power");
	ShaderProgram::GetInstance()->currProg->AddUniform("lights[0].kC");
	ShaderProgram::GetInstance()->currProg->AddUniform("lights[0].kL");
	ShaderProgram::GetInstance()->currProg->AddUniform("lights[0].kQ");
	ShaderProgram::GetInstance()->currProg->AddUniform("lights[0].spotDirection");
	ShaderProgram::GetInstance()->currProg->AddUniform("lights[0].cosCutoff");
	ShaderProgram::GetInstance()->currProg->AddUniform("lights[0].cosInner");
	ShaderProgram::GetInstance()->currProg->AddUniform("lights[0].exponent");
	ShaderProgram::GetInstance()->currProg->AddUniform("lights[1].type");
	ShaderProgram::GetInstance()->currProg->AddUniform("lights[1].position_cameraspace");
	ShaderProgram::GetInstance()->currProg->AddUniform("lights[1].color");
	ShaderProgram::GetInstance()->currProg->AddUniform("lights[1].power");
	ShaderProgram::GetInstance()->currProg->AddUniform("lights[1].kC");
	ShaderProgram::GetInstance()->currProg->AddUniform("lights[1].kL");
	ShaderProgram::GetInstance()->currProg->AddUniform("lights[1].kQ");
	ShaderProgram::GetInstance()->currProg->AddUniform("lights[1].spotDirection");
	ShaderProgram::GetInstance()->currProg->AddUniform("lights[1].cosCutoff");
	ShaderProgram::GetInstance()->currProg->AddUniform("lights[1].cosInner");
	ShaderProgram::GetInstance()->currProg->AddUniform("lights[1].exponent");
	ShaderProgram::GetInstance()->currProg->AddUniform("colorTextureEnabled");
	ShaderProgram::GetInstance()->currProg->AddUniform("colorTexture");
	ShaderProgram::GetInstance()->currProg->AddUniform("textEnabled");
	ShaderProgram::GetInstance()->currProg->AddUniform("textColor");

	/*Fog*/
	ShaderProgram::GetInstance()->currProg->AddUniform("fogParam.color");
	ShaderProgram::GetInstance()->currProg->AddUniform("fogParam.start");
	ShaderProgram::GetInstance()->currProg->AddUniform("fogParam.end");
	ShaderProgram::GetInstance()->currProg->AddUniform("fogParam.density");
	ShaderProgram::GetInstance()->currProg->AddUniform("fogParam.type");
	ShaderProgram::GetInstance()->currProg->AddUniform("fogParam.enabled");

	/*Shadow*/
	ShaderProgram::GetInstance()->currProg->AddUniform("lightDepthMVP");
	ShaderProgram::GetInstance()->currProg->AddUniform("shadowMap");

	ShaderProgram::GetInstance()->m_gPassShaderID->AddUniform("lightDepthMVP");
	ShaderProgram::GetInstance()->m_gPassShaderID->AddUniform("colorTextureEnabled[0]");
	ShaderProgram::GetInstance()->m_gPassShaderID->AddUniform("colorTexture[0]");
	ShaderProgram::GetInstance()->m_gPassShaderID->AddUniform("colorTextureEnabled[1]");
	ShaderProgram::GetInstance()->m_gPassShaderID->AddUniform("colorTexture[1]");
	ShaderProgram::GetInstance()->m_gPassShaderID->AddUniform("colorTextureEnabled[2]");
	ShaderProgram::GetInstance()->m_gPassShaderID->AddUniform("colorTexture[2]");

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateAxes("reference");
	MeshBuilder::GetInstance()->GenerateCrossHair("crosshair", 1.f, 1.f, 0.f, 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//TEXT.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);

	MeshBuilder::GetInstance()->GenerateText("IMPACT", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("IMPACT")->textureID = LoadTGA("Image//IMPACT.tga");

	MeshBuilder::GetInstance()->GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	MeshBuilder::GetInstance()->GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	MeshBuilder::GetInstance()->GenerateSphere("sphere", Color(1, 1, 0), 18, 36, 0.5f);
	MeshBuilder::GetInstance()->GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GenerateCube("PLAYER", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GenerateCube("RAIN", Color(0.5f, 0.5f, 0.5f), 1.0f);
	MeshBuilder::GetInstance()->GenerateCube("cubeBox", Color(1.0f, 0.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GenerateCube("ENEMY", Color(1.0f, 0.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GenerateCube("ALLY", Color(0.f, 0.0f, 1.0f), 1.0f);
	MeshBuilder::GetInstance()->GenerateCube("BLACK", Color(0.f, 0.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GenerateCube("PATH", Color(0.f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);

	MeshBuilder::GetInstance()->GenerateQuad("INVENTORY", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("INVENTORY")->textureID = LoadTGA("Image//Inventory.tga");

	MeshBuilder::GetInstance()->GenerateQuad("EQWINDOW", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("EQWINDOW")->textureID = LoadTGA("Image//Equipment.tga");

	MeshBuilder::GetInstance()->GenerateQuad("EMPHELM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("EMPHELM")->textureID = LoadTGA("Image//EmperorHelmet.tga");

	MeshBuilder::GetInstance()->GenerateQuad("EMPARMOR", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("EMPARMOR")->textureID = LoadTGA("Image//EmperorArmor.tga");

	MeshBuilder::GetInstance()->GenerateQuad("EMPGLOVE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("EMPGLOVE")->textureID = LoadTGA("Image//EmperorGlove.tga");

	MeshBuilder::GetInstance()->GenerateQuad("EMPSHOE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("EMPSHOE")->textureID = LoadTGA("Image//EmperorShoe.tga");

	MeshBuilder::GetInstance()->GenerateQuad("CAPHELM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("CAPHELM")->textureID = LoadTGA("Image//CaptainHelmet.tga");

	MeshBuilder::GetInstance()->GenerateQuad("CAPARMOR", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("CAPARMOR")->textureID = LoadTGA("Image//CaptainArmor.tga");

	MeshBuilder::GetInstance()->GenerateQuad("CAPSHOE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("CAPSHOE")->textureID = LoadTGA("Image//CaptainShoe.tga");

	MeshBuilder::GetInstance()->GenerateQuad("CAPGLOVE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("CAPGLOVE")->textureID = LoadTGA("Image//CaptainGlove.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SOLDIERHELM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SOLDIERHELM")->textureID = LoadTGA("Image//SoldierHelmet.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SOLDIERARMOR", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SOLDIERARMOR")->textureID = LoadTGA("Image//SoldierArmor.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SOLDIERSHOE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SOLDIERSHOE")->textureID = LoadTGA("Image//SoldierShoe.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SOLDIERGLOVE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SOLDIERGLOVE")->textureID = LoadTGA("Image//SoldierGlove.tga");

	MeshBuilder::GetInstance()->GenerateQuad("BHELM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("BHELM")->textureID = LoadTGA("Image//BasicHelmet.tga");

	MeshBuilder::GetInstance()->GenerateQuad("BARMOR", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("BARMOR")->textureID = LoadTGA("Image//BasicArmor.tga");

	MeshBuilder::GetInstance()->GenerateQuad("BGLOVE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("BGLOVE")->textureID = LoadTGA("Image//BasicGlove.tga");

	MeshBuilder::GetInstance()->GenerateQuad("BSHOE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("BSHOE")->textureID = LoadTGA("Image//BasicShoe.tga");

	MeshBuilder::GetInstance()->GenerateQuad("BGUN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("BGUN")->textureID = LoadTGA("Image//BasicGun.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SOLDIERSWORD", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SOLDIERSWORD")->textureID = LoadTGA("Image//SoldierSword.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SOLDIERGUN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SOLDIERGUN")->textureID = LoadTGA("Image//SoldierGun.tga");

	MeshBuilder::GetInstance()->GenerateQuad("CAPSWORD", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("CAPSWORD")->textureID = LoadTGA("Image//CaptainSword.tga");

	MeshBuilder::GetInstance()->GenerateQuad("CAPGUN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("CAPGUN")->textureID = LoadTGA("Image//CaptainGun.tga");

	MeshBuilder::GetInstance()->GenerateQuad("EMPSWORD", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("EMPSWORD")->textureID = LoadTGA("Image//EmperorSword.tga");

	MeshBuilder::GetInstance()->GenerateQuad("EMPGUN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("EMPGUN")->textureID = LoadTGA("Image//EmperorGun.tga");

	MeshBuilder::GetInstance()->GenerateQuad("BSWORD", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("BSWORD")->textureID = LoadTGA("Image//BasicSword.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SHOP", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SHOP")->textureID = LoadTGA("Image//Shop.tga");

	/*Player Health Bar Color*/
	MeshBuilder::GetInstance()->GenerateCube("PLAYER_HEALTH_BAR", Color(0.f, 1.0f, 0.0f), 1.0f);

	/*Translucent Quad*/
	MeshBuilder::GetInstance()->GenerateQuad("TRANS_QUAD", Color(1.f, 1.f, 1.f), 1.f);
	MeshBuilder::GetInstance()->GetMesh("TRANS_QUAD")->textureID = LoadTGA("Image//TRANS_QUAD.tga");
	/*Minimap Enlarged*/
	MeshBuilder::GetInstance()->GenerateQuad("MINIMAP_ENLARGED", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("MINIMAP_ENLARGED")->textureID = LoadTGA("Image//minimap//minimapStencil.tga");

	MeshBuilder::GetInstance()->GenerateCircle("MINIMAP", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("MINIMAP")->textureID = LoadTGA("Image//minimap/minimapBackground.tga");
	MeshBuilder::GetInstance()->GenerateQuad("MINIMAPBORDER", Color(1, 1, 1), 1.05f);
	MeshBuilder::GetInstance()->GetMesh("MINIMAPBORDER")->textureID = LoadTGA("Image//minimap//minimapBorder.tga");
	MeshBuilder::GetInstance()->GenerateQuad("MINIMAPAVATAR", Color(1, 1, 0), 0.25f);
	MeshBuilder::GetInstance()->GetMesh("MINIMAPAVATAR")->textureID = LoadTGA("Image//minimap//minimapAvatar.tga");
	MeshBuilder::GetInstance()->GenerateQuad("MINIMAP_STENCIL", Color(1, 1, 1), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("MINIMAP_STENCIL")->textureID = LoadTGA("Image//minimap//minimapStencil.tga");

	/*Battle Scene*/
	MeshBuilder::GetInstance()->GenerateQuad("B_GRASS", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("B_GRASS")->textureID = LoadTGA("Image//Battle/B_GRASS.tga");

}