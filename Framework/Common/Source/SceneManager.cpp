#include "SceneManager.h"
#include "Scene.h"

/*DepthFBO Quad.*/
#include "../../../Framework/Base/Source/DepthFBO.h"

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

std::vector<unsigned char>& SceneManager::GetHeightMap(void)
{
	return heightMap;
}

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
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID[0] = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID[0] = LoadTGA("Image//TEXT.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);

	MeshBuilder::GetInstance()->GenerateText("IMPACT", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("IMPACT")->textureID[0] = LoadTGA("Image//IMPACT.tga");

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
	MeshBuilder::GetInstance()->GenerateCube("AABB", Color(1.f, 1.f, 1.f), 1.f);
	//MeshBuilder::GetInstance()->GetMesh("AABB")->textureID[0] = LoadTGA("Image//TRANS_QUAD.tga");
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);

	MeshBuilder::GetInstance()->GenerateQuad("INVENTORY", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("INVENTORY")->textureID[0] = LoadTGA("Image//Inventory.tga");

	MeshBuilder::GetInstance()->GenerateQuad("EQWINDOW", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("EQWINDOW")->textureID[0] = LoadTGA("Image//Equipment.tga");

	MeshBuilder::GetInstance()->GenerateQuad("EMPHELM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("EMPHELM")->textureID[0] = LoadTGA("Image//EmperorHelmet.tga");

	MeshBuilder::GetInstance()->GenerateQuad("EMPARMOR", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("EMPARMOR")->textureID[0] = LoadTGA("Image//EmperorArmor.tga");

	MeshBuilder::GetInstance()->GenerateQuad("EMPGLOVE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("EMPGLOVE")->textureID[0] = LoadTGA("Image//EmperorGlove.tga");

	MeshBuilder::GetInstance()->GenerateQuad("EMPSHOE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("EMPSHOE")->textureID[0] = LoadTGA("Image//EmperorShoe.tga");

	MeshBuilder::GetInstance()->GenerateQuad("CAPHELM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("CAPHELM")->textureID[0] = LoadTGA("Image//CaptainHelmet.tga");

	MeshBuilder::GetInstance()->GenerateQuad("CAPARMOR", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("CAPARMOR")->textureID[0] = LoadTGA("Image//CaptainArmor.tga");

	MeshBuilder::GetInstance()->GenerateQuad("CAPSHOE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("CAPSHOE")->textureID[0] = LoadTGA("Image//CaptainShoe.tga");

	MeshBuilder::GetInstance()->GenerateQuad("CAPGLOVE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("CAPGLOVE")->textureID[0] = LoadTGA("Image//CaptainGlove.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SOLDIERHELM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SOLDIERHELM")->textureID[0] = LoadTGA("Image//SoldierHelmet.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SOLDIERARMOR", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SOLDIERARMOR")->textureID[0] = LoadTGA("Image//SoldierArmor.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SOLDIERSHOE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SOLDIERSHOE")->textureID[0] = LoadTGA("Image//SoldierShoe.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SOLDIERGLOVE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SOLDIERGLOVE")->textureID[0] = LoadTGA("Image//SoldierGlove.tga");

	MeshBuilder::GetInstance()->GenerateQuad("BHELM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("BHELM")->textureID[0] = LoadTGA("Image//BasicHelmet.tga");

	MeshBuilder::GetInstance()->GenerateQuad("BARMOR", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("BARMOR")->textureID[0] = LoadTGA("Image//BasicArmor.tga");

	MeshBuilder::GetInstance()->GenerateQuad("BGLOVE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("BGLOVE")->textureID[0] = LoadTGA("Image//BasicGlove.tga");

	MeshBuilder::GetInstance()->GenerateQuad("BSHOE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("BSHOE")->textureID[0] = LoadTGA("Image//BasicShoe.tga");

	MeshBuilder::GetInstance()->GenerateQuad("BGUN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("BGUN")->textureID[0] = LoadTGA("Image//BasicGun.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SOLDIERSWORD", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SOLDIERSWORD")->textureID[0] = LoadTGA("Image//SoldierSword.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SOLDIERGUN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SOLDIERGUN")->textureID[0] = LoadTGA("Image//SoldierGun.tga");

	MeshBuilder::GetInstance()->GenerateQuad("CAPSWORD", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("CAPSWORD")->textureID[0] = LoadTGA("Image//CaptainSword.tga");

	MeshBuilder::GetInstance()->GenerateQuad("CAPGUN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("CAPGUN")->textureID[0] = LoadTGA("Image//CaptainGun.tga");

	MeshBuilder::GetInstance()->GenerateQuad("EMPSWORD", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("EMPSWORD")->textureID[0] = LoadTGA("Image//EmperorSword.tga");

	MeshBuilder::GetInstance()->GenerateQuad("EMPGUN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("EMPGUN")->textureID[0] = LoadTGA("Image//EmperorGun.tga");

	MeshBuilder::GetInstance()->GenerateQuad("BSWORD", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("BSWORD")->textureID[0] = LoadTGA("Image//BasicSword.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SHOP", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SHOP")->textureID[0] = LoadTGA("Image//Shop.tga");

	/*Player Health Bar Color*/
	MeshBuilder::GetInstance()->GenerateCube("PLAYER_HEALTH_BAR", Color(0.f, 1.0f, 0.0f), 1.0f);

	/*Translucent Quad*/
	MeshBuilder::GetInstance()->GenerateQuad("TRANS_QUAD", Color(1.f, 1.f, 1.f), 1.f);
	MeshBuilder::GetInstance()->GetMesh("TRANS_QUAD")->textureID[0] = LoadTGA("Image//TRANS_QUAD.tga");
	/*Minimap Enlarged*/
	MeshBuilder::GetInstance()->GenerateQuad("MINIMAP_ENLARGED", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("MINIMAP_ENLARGED")->textureID[0] = LoadTGA("Image//minimap//minimapStencil.tga");

	MeshBuilder::GetInstance()->GenerateCircle("MINIMAP", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("MINIMAP")->textureID[0] = LoadTGA("Image//minimap/minimapBackground.tga");
	MeshBuilder::GetInstance()->GenerateQuad("MINIMAPBORDER", Color(1, 1, 1), 1.05f);
	MeshBuilder::GetInstance()->GetMesh("MINIMAPBORDER")->textureID[0] = LoadTGA("Image//minimap//minimapBorder.tga");
	MeshBuilder::GetInstance()->GenerateQuad("MINIMAPAVATAR", Color(1, 1, 0), 0.25f);
	MeshBuilder::GetInstance()->GetMesh("MINIMAPAVATAR")->textureID[0] = LoadTGA("Image//minimap//minimapAvatar.tga");
	MeshBuilder::GetInstance()->GenerateQuad("MINIMAP_STENCIL", Color(1, 1, 1), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("MINIMAP_STENCIL")->textureID[0] = LoadTGA("Image//minimap//minimapStencil.tga");

	/*Battle Scene.*/
	MeshBuilder::GetInstance()->GenerateQuad("B_GRASS", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("B_GRASS")->textureID[0] = LoadTGA("Image//BATTLE//B_GRASS.tga");

	/*World Tiles.*/
	//MeshBuilder::GetInstance()->GenerateQuad("W_SNOW", Color(1, 1, 1), 1.f);
	//MeshBuilder::GetInstance()->GetMesh("W_SNOW")->textureID[0] = LoadTGA("Image//WORLD//W_SNOW.tga");

	//MeshBuilder::GetInstance()->GenerateQuad("W_GRASS", Color(1, 1, 1), 1.f);
	//MeshBuilder::GetInstance()->GetMesh("W_GRASS")->textureID[0] = LoadTGA("Image//WORLD//W_GRASS.tga");

	/*Terrain.*/
	MeshBuilder::GetInstance()->GenerateTerrain("TERRAIN", "Image//TERRAIN//heightMap.raw", heightMap);
	MeshBuilder::GetInstance()->GetMesh("TERRAIN")->textureID[0] = LoadTGA("Image//WORLD//W_GRASS.tga");
	MeshBuilder::GetInstance()->GetMesh("TERRAIN")->textureID[1] = LoadTGA("Image//WORLD//W_SNOW.tga");
	MeshBuilder::GetInstance()->GetMesh("TERRAIN")->textureID[2] = LoadTGA("Image//WORLD//W_WATER.tga");

	/*Shadow Quad.*/
	MeshBuilder::GetInstance()->GenerateQuad("LIGHT_DEPTH_TEXTURE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("LIGHT_DEPTH_TEXTURE")->textureID[0] = DepthFBO::GetInstance()->GetTexture();

	/*Monk.*/
	MeshBuilder::GetInstance()->GenerateOBJ("Head", "OBJ//MONK//Head.obj");
	MeshBuilder::GetInstance()->GetMesh("Head")->textureID[0] = LoadTGA("Image//MONK//bad tang_diffuse.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("Body", "OBJ//MONK//Body.obj");
	MeshBuilder::GetInstance()->GetMesh("Body")->textureID[0] = LoadTGA("Image//MONK//bad tang_diffuse.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("Left arm", "OBJ//MONK//Left arm.obj");
	MeshBuilder::GetInstance()->GetMesh("Left arm")->textureID[0] = LoadTGA("Image//MONK//bad tang_diffuse.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("Right arm", "OBJ//MONK//Right arm.obj");
	MeshBuilder::GetInstance()->GetMesh("Right arm")->textureID[0] = LoadTGA("Image//MONK//bad tang_diffuse.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("Left leg", "OBJ//MONK//Left leg.obj");
	MeshBuilder::GetInstance()->GetMesh("Left leg")->textureID[0] = LoadTGA("Image//MONK//bad tang_diffuse.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("Right leg", "OBJ//MONK//Right leg.obj");
	MeshBuilder::GetInstance()->GetMesh("Right leg")->textureID[0] = LoadTGA("Image//MONK//bad tang_diffuse.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("Gourd", "OBJ//MONK//Gourd.obj");
	MeshBuilder::GetInstance()->GetMesh("Gourd")->textureID[0] = LoadTGA("Image//MONK//bad tang_diffuse.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("Talisman", "OBJ//MONK//Talisman.obj");
	MeshBuilder::GetInstance()->GetMesh("Talisman")->textureID[0] = LoadTGA("Image//MONK//bad tang_diffuse.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("Weapon", "OBJ//MONK//Weapon.obj");
	MeshBuilder::GetInstance()->GetMesh("Weapon")->textureID[0] = LoadTGA("Image//MONK//bad tang_diffuse.tga");
	//MeshBuilder::GetInstance()->GenerateOBJ("MONK", "OBJ//MONK.obj");
	//MeshBuilder::GetInstance()->GetMesh("MONK")->textureID[0] = LoadTGA("Image//MONK//bad tang_alphapart.tga");
	//MeshBuilder::GetInstance()->GetMesh("MONK")->textureID[1] = LoadTGA("Image//MONK//bad tang_ao.tga");
	//MeshBuilder::GetInstance()->GetMesh("MONK")->textureID[2] = LoadTGA("Image//MONK//bad tang_diffuse.tga");
	//MeshBuilder::GetInstance()->GetMesh("MONK")->textureID[3] = LoadTGA("Image//MONK//bad tang_emissive.tga");
	//MeshBuilder::GetInstance()->GetMesh("MONK")->textureID[4] = LoadTGA("Image//MONK//bad tang_height.tga");
	//MeshBuilder::GetInstance()->GetMesh("MONK")->textureID[5] = LoadTGA("Image//MONK//bad tang_normal.tga");
	//MeshBuilder::GetInstance()->GetMesh("MONK")->textureID[6] = LoadTGA("Image//MONK//bad tang_roughness.tga");
	//MeshBuilder::GetInstance()->GetMesh("MONK")->textureID[7] = LoadTGA("Image//MONK//bad tang_spec-metallic.tga");
}