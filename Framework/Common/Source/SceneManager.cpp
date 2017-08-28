#include "SceneManager.h"
#include "Scene.h"

#include "GL\glew.h"
#include "GLFW\glfw3.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "Mesh.h"
#include "../../Base/Source/Application.h"
#include "Utility.h"
#include "LoadTGA.h"

#include "../../Base/Source/Debugger/Debugger.h"
#include "KeyboardController.h"


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

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateAxes("reference");
	MeshBuilder::GetInstance()->GenerateCrossHair("crosshair", 1.f, 1.f, 0.f, 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//comic.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);

	MeshBuilder::GetInstance()->GenerateText("IMPACT", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("IMPACT")->textureID = LoadTGA("Image//IMPACT.tga");

	/*Enemy Turret*/
	MeshBuilder::GetInstance()->GenerateOBJ("turret", "OBJ//turret.obj");
	MeshBuilder::GetInstance()->GetMesh("turret")->textureID = LoadTGA("Image//turret.tga");

	/*Crate*/
	MeshBuilder::GetInstance()->GenerateOBJ("crate", "OBJ//crate.obj");
	MeshBuilder::GetInstance()->GetMesh("crate")->textureID = LoadTGA("Image//crate.tga");

	/*WatchTower*/
	MeshBuilder::GetInstance()->GenerateOBJ("WatchTower", "OBJ//WatchTower.obj");
	MeshBuilder::GetInstance()->GetMesh("WatchTower")->textureID = LoadTGA("Image//WatchTower.tga");

	/*Barricade*/
	MeshBuilder::GetInstance()->GenerateOBJ("Barricade", "OBJ//Barricade.obj");
	MeshBuilder::GetInstance()->GetMesh("Barricade")->textureID = LoadTGA("Image//Barricade.tga");

	/*Statue*/
	MeshBuilder::GetInstance()->GenerateOBJ("Statue", "OBJ//Statue.obj");
	MeshBuilder::GetInstance()->GetMesh("Statue")->textureID = LoadTGA("Image//Statue.tga");

	/*Barrel*/
	MeshBuilder::GetInstance()->GenerateOBJ("Barrel", "OBJ//Barrel.obj");
	MeshBuilder::GetInstance()->GetMesh("Barrel")->textureID = LoadTGA("Image//crate.tga");

	/*Tower*/
	MeshBuilder::GetInstance()->GenerateOBJ("TOWER", "OBJ//Tower.obj");
	MeshBuilder::GetInstance()->GetMesh("TOWER")->textureID = LoadTGA("Image//Tower.tga");
	
	/*Wall*/
	MeshBuilder::GetInstance()->GenerateOBJ("Wall", "OBJ//Wall.obj");
	MeshBuilder::GetInstance()->GetMesh("Wall")->textureID = LoadTGA("Image//Wall_Texture.tga");


	MeshBuilder::GetInstance()->GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	MeshBuilder::GetInstance()->GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	MeshBuilder::GetInstance()->GenerateSphere("sphere", Color(1, 1, 0), 18, 36, 0.5f);
	MeshBuilder::GetInstance()->GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GenerateCube("cubeBox", Color(1.0f, 0.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GenerateCube("ENEMY", Color(1.0f, 0.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GenerateCube("ALLY", Color(0.f, 0.0f, 1.0f), 1.0f);
	MeshBuilder::GetInstance()->GenerateCube("BLACK", Color(0.f, 0.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GenerateCube("PATH", Color(0.f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);

	MeshBuilder::GetInstance()->GenerateQuad("snowGround", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("snowGround")->textureID = LoadTGA("Image//SkyBox//nottingham_dn.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_FRONT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BACK", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_LEFT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_RIGHT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_TOP", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BOTTOM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_FRONT")->textureID = LoadTGA("Image//SkyBox//nottingham_ft.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BACK")->textureID = LoadTGA("Image//SkyBox//nottingham_bk.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_LEFT")->textureID = LoadTGA("Image//SkyBox//nottingham_lf.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_RIGHT")->textureID = LoadTGA("Image//SkyBox//nottingham_rt.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_TOP")->textureID = LoadTGA("Image//SkyBox//nottingham_up.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BOTTOM")->textureID = LoadTGA("Image//SkyBox//nottingham_dn.tga");

	MeshBuilder::GetInstance()->GenerateQuad("PISTOL", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("PISTOL")->textureID = LoadTGA("Image//PISTOL.tga");

	MeshBuilder::GetInstance()->GenerateQuad("ASSAULT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("ASSAULT")->textureID = LoadTGA("Image//ASSAULT.tga");

	MeshBuilder::GetInstance()->GenerateQuad("PAUSE", Color(1, 0, 0), 1.f);

	MeshBuilder::GetInstance()->GenerateQuad("PLAYER_PISTOL", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("PLAYER_PISTOL")->textureID = LoadTGA("Image//PLAYER_PISTOL.tga");

	MeshBuilder::GetInstance()->GenerateQuad("PLAYER_ASSAULT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("PLAYER_ASSAULT")->textureID = LoadTGA("Image//PLAYER_ASSAULT.tga");

	/*Bullet*/
	//MeshBuilder::GetInstance()->GenerateCube("POWERUP_BULLET", Color(0.f, 0.f, 1.f), 1.0f);
	MeshBuilder::GetInstance()->GenerateOBJ("POWERUP_BULLET", "OBJ//POWERUP_BULLET.obj");
	MeshBuilder::GetInstance()->GetMesh("POWERUP_BULLET")->textureID = LoadTGA("Image//POWERUP_BULLET.tga");
	/*Health*/
	//MeshBuilder::GetInstance()->GenerateCube("health", Color(0.f, 1.f, 0.f), 1.0f);
	MeshBuilder::GetInstance()->GenerateOBJ("POWERUP_HEALTH", "OBJ//POWERUP_HEALTH.obj");
	MeshBuilder::GetInstance()->GetMesh("POWERUP_HEALTH")->textureID = LoadTGA("Image//POWERUP_HEALTH.tga");
	/*Hit*/
	MeshBuilder::GetInstance()->GenerateQuad("HIT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("HIT")->textureID = LoadTGA("Image//HIT.tga");
	/*Game Over*/
	MeshBuilder::GetInstance()->GenerateQuad("GAMEOVER", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GAMEOVER")->textureID = LoadTGA("Image//GAMEOVER.tga");

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

	MeshBuilder::GetInstance()->GenerateQuad("SELECTION", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SELECTION")->textureID = LoadTGA("Image//Selection.tga");

	/*Robot Troop*/
	MeshBuilder::GetInstance()->GenerateOBJ("ROBOT", "OBJ//ROBOT.obj");
	MeshBuilder::GetInstance()->GetMesh("ROBOT")->textureID = LoadTGA("Image//ROBOT.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("ROBOT_CORE", "OBJ//Robot_Torso.obj");
	MeshBuilder::GetInstance()->GetMesh("ROBOT_CORE")->textureID = LoadTGA("Image//ROBOT.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("ROBOT_LeftArm", "OBJ//Robot_LeftArm.obj");
	MeshBuilder::GetInstance()->GetMesh("ROBOT_LeftArm")->textureID = LoadTGA("Image//ROBOT.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("ROBOT_RightArm", "OBJ//Robot_RightArm.obj");
	MeshBuilder::GetInstance()->GetMesh("ROBOT_RightArm")->textureID = LoadTGA("Image//ROBOT.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("ROBOT_LeftLeg", "OBJ//Robot_LeftLeg.obj");
	MeshBuilder::GetInstance()->GetMesh("ROBOT_LeftLeg")->textureID = LoadTGA("Image//ROBOT.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("ROBOT_RightLeg", "OBJ//Robot_RightLeg.obj");
	MeshBuilder::GetInstance()->GetMesh("ROBOT_RightLeg")->textureID = LoadTGA("Image//ROBOT.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("ROBOT_Head", "OBJ//Robot_Head.obj");
	MeshBuilder::GetInstance()->GetMesh("ROBOT_Head")->textureID = LoadTGA("Image//ROBOT.tga");

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

	MeshBuilder::GetInstance()->GenerateQuad("MainMenu", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("MainMenu")->textureID = LoadTGA("Image//Mainmenu/Menubackground.tga");
	MeshBuilder::GetInstance()->GenerateQuad("MainMenuTitle", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("MainMenuTitle")->textureID = LoadTGA("Image//Mainmenu/Menutitle.tga");
	MeshBuilder::GetInstance()->GenerateQuad("MainMenuStart", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("MainMenuStart")->textureID = LoadTGA("Image//Mainmenu/Menustart.tga");
	MeshBuilder::GetInstance()->GenerateQuad("MainMenuLevels", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("MainMenuLevels")->textureID = LoadTGA("Image//Mainmenu/Menulevels.tga");
	MeshBuilder::GetInstance()->GenerateQuad("MainMenuQuit", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("MainMenuQuit")->textureID = LoadTGA("Image//Mainmenu/Menuquit.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SelectionMenu", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SelectionMenu")->textureID = LoadTGA("Image//Mainmenu/MenuBackground.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SelectionTitle", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SelectionTitle")->textureID = LoadTGA("Image//Selection/LevelSelection.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SelectionTutorial", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SelectionTutorial")->textureID = LoadTGA("Image//Selection/Tutorial.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SelectionVillageTown", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SelectionVillageTown")->textureID = LoadTGA("Image//Selection/VillageTown.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SelectionLevel1", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SelectionLevel1")->textureID = LoadTGA("Image//Selection/Level1.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SelectionLevel2", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SelectionLevel2")->textureID = LoadTGA("Image//Selection/Level2.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SelectionLevel3", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SelectionLevel3")->textureID = LoadTGA("Image//Selection/Level3.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SelectionLevel4", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SelectionLevel4")->textureID = LoadTGA("Image//Selection/Level4.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SelectionBack", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SelectionBack")->textureID = LoadTGA("Image//Selection/Back.tga");

}