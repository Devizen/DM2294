#include "SceneText.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "EntityManager.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"
#include "SkyBox/SkyBoxEntity.h"
#include "Minimap\Minimap.h"
#include "Object\Furniture.h"
#include "Object\Bullet.h"
#include "Object\Health.h"

/*Options Manager*/
#include "Options\OptionsManager.h"
#include "Options\Controls.h"

/*Rendering*/
#include "RenderHelper.h"

/*Sound*/
#include "SoundEngine.h"

/*Enemies*/
#include "Enemy\Enemy3D.h"
#include "Enemy/AnimatedEnemy/AnimatedEnemy.h"
#include "Enemy\Patrol\Patrol.h"

#include "Inventory.h"
#include "Items\Helmet.h"
#include "FileManager.h"

/*Map Editor*/
#include "Map_Editor\Map_Editor.h"

#include <iostream>
using namespace std;

SceneText* SceneText::sInstance = new SceneText(SceneManager::GetInstance());

Mesh* TEMPDEPTHQUAD;
Mesh* TEMPSPHERE;
Mesh* TEMPQUAD;

SceneText::SceneText()
	: theMinimap(NULL)
	, theCameraEffects(NULL)
	, currentHighscore(0)
	//, m_worldHeight(0.f)
	//, m_worldWidth(0.f)
{
}

SceneText::SceneText(SceneManager* _sceneMgr)
	: theMinimap(NULL)
	, theCameraEffects(NULL)
	, currentHighscore(0)
{
	_sceneMgr->AddScene("Start", this);
}

SceneText::~SceneText()
{
}

void SceneText::Init()
{
	//Calculating aspect ratio
	Application::GetInstance().setAspectRatioHeight(100.f);
	Application::GetInstance().setAspectRatioWidth(Application::GetInstance().getAspectRatioHeight() * (float)Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight());
	//m_worldWidth = m_worldHeight * (float)Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight();
	//m_worldHeight = (float)Application::GetInstance().GetWindowHeight();
	//m_worldWidth = (float)Application::GetInstance().GetWindowWidth();

	//currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");
	currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Shadow.vertexshader", "Shader//Shadow.fragmentshader");
	m_gPassShaderID = GraphicsManager::GetInstance()->LoadShader("gpass",	"Shader//GPass.vertexshader", "Shader//GPass.fragmentshader");
	
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
	
	//m_parameters[U_FOG_COLOR] = glGetUniformLocation(m_programID, "fogParam.color");
	//m_parameters[U_FOG_START] = glGetUniformLocation(m_programID, "fogParam.start");
	//m_parameters[U_FOG_END] = glGetUniformLocation(m_programID, "fogParam.end");
	//m_parameters[U_FOG_DENSITY] = glGetUniformLocation(m_programID, "fogParam.density");
	//m_parameters[U_FOG_TYPE] = glGetUniformLocation(m_programID, "fogParam.type");
	//m_parameters[U_FOG_ENABLED] = glGetUniformLocation(m_programID, "fogParam.enabled");

	///*Shadow*/
	//m_parameters[U_LIGHT_DEPTH_MVP_GPASS] = glGetUniformLocation(m_gPassShaderID, "lightDepthMVP");
	//m_parameters[U_LIGHT_DEPTH_MVP] = glGetUniformLocation(m_programID, "lightDepthMVP");
	//m_parameters[U_SHADOW_MAP] = glGetUniformLocation(m_programID, "shadowMap");

	//m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[0]");
	//m_parameters[U_SHADOW_COLOR_TEXTURE] = glGetUniformLocation(m_gPassShaderID, "colorTexture[0]");
	//m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[1]");
	//m_parameters[U_SHADOW_COLOR_TEXTURE1] = glGetUniformLocation(m_gPassShaderID, "colorTexture[1]");
	//m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED2] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[2]");
	//m_parameters[U_SHADOW_COLOR_TEXTURE2] = glGetUniformLocation(m_gPassShaderID, "colorTexture[2]");
	
	// Tell the graphics manager to use the shader we just loaded
	GraphicsManager::GetInstance()->SetActiveShader("default");

	lights[0] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	lights[0]->type = Light::LIGHT_DIRECTIONAL;
	lights[0]->position.Set(-6, 2, 0);
	lights[0]->color.Set(1, 1, 1);
	lights[0]->power = 1;
	lights[0]->kC = 1.f;
	lights[0]->kL = 0.01f;
	lights[0]->kQ = 0.001f;
	lights[0]->cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0]->cosInner = cos(Math::DegreeToRadian(30));
	lights[0]->exponent = 3.f;
	lights[0]->spotDirection.Set(0.f, 1.f, 0.f);
	lights[0]->name = "lights[0]";

	lights[1] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[1]", lights[1]);
	lights[1]->type = Light::LIGHT_POINT;
	lights[1]->position.Set(1, 1, 0);
	lights[1]->color.Set(1, 1, 0.5f);
	lights[1]->power = 0.4f;
	lights[1]->name = "lights[1]";

	currProg->UpdateInt("numLights", 1);
	currProg->UpdateInt("textEnabled", 0);

	Color fogColor(0.5f, 0.5f, 0.5f); //Vec3 Color
	currProg->UpdateVector3("fogParam.color", &fogColor.r);
	currProg->UpdateFloat("fogParam.start", 10);
	currProg->UpdateFloat("fogParam.end", 2000);
	currProg->UpdateFloat("fogParam.density", 0.005f);
	currProg->UpdateInt("fogParam.type", 0);
	currProg->UpdateInt("fogParam.enabled", 0);
	
	// Create the playerinfo instance, which manages all information about the player
	playerInfo = CPlayerInfo::GetInstance();
	playerInfo->Init();

	// Create and attach the camera to the scene
	//camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.Init(playerInfo->GetPos(), playerInfo->GetTarget(), playerInfo->GetUp());
	playerInfo->AttachCamera(&camera);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateAxes("reference");
	MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//comic.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);

	/*Enemy Turret*/
	MeshBuilder::GetInstance()->GenerateOBJ("turret", "OBJ//turret.obj");
	MeshBuilder::GetInstance()->GetMesh("turret")->textureID = LoadTGA("Image//turret.tga");

	/*Crate*/
	MeshBuilder::GetInstance()->GenerateOBJ("crate", "OBJ//crate.obj");
	MeshBuilder::GetInstance()->GetMesh("crate")->textureID = LoadTGA("Image//crate.tga");

	MeshBuilder::GetInstance()->GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	MeshBuilder::GetInstance()->GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	MeshBuilder::GetInstance()->GenerateSphere("sphere", Color(0, 0, 0), 18, 36, 0.5f);
	MeshBuilder::GetInstance()->GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GenerateCube("ENEMY", Color(1.0f, 0.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);

	MeshBuilder::GetInstance()->GenerateQuad("snowGround", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("snowGround")->textureID = LoadTGA("Image//snowGround.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_FRONT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BACK", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_LEFT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_RIGHT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_TOP", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BOTTOM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_FRONT")->textureID = LoadTGA("Image//SkyBox//skybox_front.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BACK")->textureID = LoadTGA("Image//SkyBox//skybox_back.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_LEFT")->textureID = LoadTGA("Image//SkyBox//skybox_left.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_RIGHT")->textureID = LoadTGA("Image//SkyBox//skybox_right.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_TOP")->textureID = LoadTGA("Image//SkyBox//skybox_top.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BOTTOM")->textureID = LoadTGA("Image//SkyBox//skybox_bottom.tga");

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

	MeshBuilder::GetInstance()->GenerateQuad("EMPHELM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("EMPHELM")->textureID = LoadTGA("Image//EmperorHelmet.tga");

	MeshBuilder::GetInstance()->GenerateQuad("BHELM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("BHELM")->textureID = LoadTGA("Image//BasicHelmet.tga");

	MeshBuilder::GetInstance()->GenerateQuad("BARMOR", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("BARMOR")->textureID = LoadTGA("Image//BasicArmor.tga");

	MeshBuilder::GetInstance()->GenerateQuad("BGLOVE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("BGLOVE")->textureID = LoadTGA("Image//BasicGlove.tga");

	MeshBuilder::GetInstance()->GenerateQuad("BSHOE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("BSHOE")->textureID = LoadTGA("Image//BasicShoe.tga");

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

	/*Minimap Enlarged*/
	MeshBuilder::GetInstance()->GenerateQuad("MINIMAP_ENLARGED", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("MINIMAP_ENLARGED")->textureID = LoadTGA("Image//MINIMAP_ENLARGED.tga");

	// Create entities into the scene
	//Create::Entity("reference", Vector3(0.0f, 0.0f, 0.0f)); // Reference
	//Create::Entity("lightball", Vector3(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z)); // Lightball
	////GenericEntity* aCube = Create::Entity("cube", Vector3(-20.0f, 0.0f, -20.0f));
	//Create::Entity("ring", Vector3(0.0f, 0.0f, 0.0f)); // Reference

	groundEntity = Create::Ground("snowGround", "snowGround");
	groundEntity->SetLight(true);

//	Create::Text3DObject("text", Vector3(0.0f, 0.0f, 0.0f), "DM2210", Vector3(10.0f, 10.0f, 10.0f), Color(0, 1, 1));
	Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), true);

	///*Chair Test*/
	//CFurniture* chair = Create::Furniture("Chair", Vector3(20.f, 0.f, 0.f), Vector3(1.f, 1.f, 1.f));
	//chair->SetCollider(true);
	//chair->SetLight(false);
	//chair->SetAABB(Vector3(5.f, 5.f, 5.f), Vector3(-5.f, -5.f, -5.f));
	//


	SkyBoxEntity* theSkyBox = Create::SkyBox("SKYBOX_FRONT", "SKYBOX_BACK",
											 "SKYBOX_LEFT", "SKYBOX_RIGHT",
											 "SKYBOX_TOP", "SKYBOX_BOTTOM");
	// Customise the ground entity
	groundEntity->SetPosition(Vector3(0, -10, 0));
	groundEntity->SetScale(Vector3(100.0f, 100.0f, 100.0f));
	groundEntity->SetGrids(Vector3(10.0f, 1.0f, 10.0f));
	groundEntity->SetLight(true);
	playerInfo->SetTerrain(groundEntity);

	//Create::Entity("PAUSE", Vector3(Application::GetInstance().GetWindowWidth() / 2.0f, Application::GetInstance().GetWindowHeight() / 2.0f, 1.f),
	//	Vector3(100.f, 100.f, 100.f));

	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 25.0f;
	float halfFontSize = fontSize / 2.0f;
	for (int i = 0; i < 30; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth + fontSize, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(1.0f, 0.f,0.0f));
	}

	for (int i = 0; i < 17; ++i)
	{
		controlText[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, (-halfWindowHeight / 2) + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.f, 0.f, 0.0f));
	}


	// Hardware Abstraction
	theKeyboard = new CKeyboard();
	theKeyboard->Create(playerInfo);

	// Activate the Blend Function
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Minimap
	theMinimap = Create::Minimap(false);
	theMinimap->SetBackground(MeshBuilder::GetInstance()->GenerateQuad("MINIMAP", Color(1, 1, 1), 1.f));
	theMinimap->GetBackground()->textureID = LoadTGA("Image//snowGround.tga");
	theMinimap->SetBorder(MeshBuilder::GetInstance()->GenerateCircle("MINIMAPBORDER", Color(1, 1, 1), 1.05f));
	theMinimap->SetAvatar(MeshBuilder::GetInstance()->GenerateQuad("MINIMAPAVATAR", Color(1, 1, 0), 0.25f));
	theMinimap->GetAvatar()->textureID = LoadTGA("Image//Avatar.tga");
	theMinimap->SetStencil(MeshBuilder::GetInstance()->GenerateCircle("MINIMAP_STENCIL", Color(1, 1, 1), 1.0f));

	// CameraEffects
	theCameraEffects = Create::CameraEffects(false);
	theCameraEffects->SetBloodScreen(MeshBuilder::GetInstance()->GenerateQuad("CAMERAEFFECTS_BLOODSCREEN", 
										Color(1, 1, 1), 1.f));
	theCameraEffects->GetBloodScreen()->textureID = LoadTGA("Image//CameraEffects_Blood.tga");
	theCameraEffects->SetStatus_BloodScreen(false);

	theMouse = new CMouse();
	theMouse->Create(playerInfo);

	/*Load Controls*/
	OptionsManager::GetInstance()->loadConfig("Data//keyconfig.txt");
	/*Load Highscore*/
	OptionsManager::GetInstance()->loadHighscore("Data//highscore.txt");
	primaryWeapon = playerInfo->getPrimaryWeapon();
	// << "Primary Weapon From Scene: " << primaryWeapon << endl;
	weaponManager = playerInfo->getWeaponManager();

	/*Initialise Sounds*/
	CSoundEngine::GetInstance()->AddSound("PISTOL", "Sound\\SFX\\PISTOL.ogg");
	CSoundEngine::GetInstance()->AddSound("ASSAULT", "Sound\\SFX\\ASSAULT.ogg");
	CSoundEngine::GetInstance()->AddSound("TAKEDAMAGE", "Sound\\SFX\\TAKEDAMAGE.ogg");
	CSoundEngine::GetInstance()->AddSound("RELOAD", "Sound\\SFX\\RELOAD.ogg");
	CSoundEngine::GetInstance()->AddSound("EXPLODE", "Sound\\SFX\\EXPLODE.ogg");
	CSoundEngine::GetInstance()->AddSound("HEAL", "Sound\\SFX\\HEAL.ogg");
	CSoundEngine::GetInstance()->GetSoundEngine()->play2D("Sound\\BGM\\HURRY.ogg", true);
	/*Shadow*/
	DepthFBO::GetInstance()->Init(1024, 1024);
	//m_lightDepthFBO.Init(1024, 1024);

	TEMPDEPTHQUAD = MeshBuilder::GetInstance()->GenerateQuad("TEMPDEPTH", Color(1, 0, 1), 1);
	TEMPDEPTHQUAD->textureID = DepthFBO::GetInstance()->GetTexture();
	TEMPSPHERE = MeshBuilder::GetInstance()->GenerateSphere("TEMPSPHERE", Color(1, 1, 1), 10, 10, 1);
	TEMPQUAD = MeshBuilder::GetInstance()->GenerateQuad("TEMPQUADGROUND", Color(1, 1, 1), 1);

	ParticleManager* particleManager = ParticleManager::GetInstance();
	vector<ParticleManager*>particleList = particleManager->getParticleList();
	for (int i = 0; i < particleManager->getMaximumParticles(); ++i)
	{
		particleManager->pushParticle(particleObject_type::P_Water);
	}
	cout << "Particle List Size in Scene: " << particleList.size() << endl;
	Inventory::GetInstance()->Init();

	openInventory = false;
	FileManager::GetInstance()->init();
}

void SceneText::Update(double dt)
{
	//Calculating aspect ratio
	Application::GetInstance().setAspectRatioHeight(100.f);
	Application::GetInstance().setAspectRatioWidth(Application::GetInstance().getAspectRatioHeight() * (float)Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight());
	//m_worldHeight = 100.f;
	//m_worldWidth = m_worldHeight * (float)Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight();
	//m_worldHeight = (float)Application::GetInstance().GetWindowHeight();
	//m_worldWidth = (float)Application::GetInstance().GetWindowWidth();

	static bool pause = false;
	static int renderOnce = 0;

	if (KeyboardController::GetInstance()->IsKeyPressed('I'))
	{
		OptionsManager::GetInstance()->setEditingState(true);
		openInventory = true;
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('U'))
	{
		OptionsManager::GetInstance()->setEditingState(false);
		openInventory = false;
	}

	if (openInventory)
	{
		FileManager::GetInstance()->update(dt);
	}

	//if (KeyboardController::GetInstance()->IsKeyDown('O')) {
	//	Mtx44 rotate;
	//	rotate.SetToRotation(90 * dt, 1, 0, 0);
	//	lights[0]->position = rotate * lights[0]->position;
	//}

	if (KeyboardController::GetInstance()->IsKeyPressed('K'))
	{
		FileManager::GetInstance()->ReadWeaponFile("Files//Inventory.csv");
		FileManager::GetInstance()->CreateWeapon();
		//cout << _msize(Inventory::GetInstance()->ReturnType()) << endl;
	}

	if (playerInfo->getAttribute(CAttributes::TYPE_HEALTH) > 0)
	{
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_BACK))
			pause = true;

		if (pause)
		{
			pauseOptions(dt, pause);
			displayControls();
		}

		if (!OptionsManager::GetInstance()->getEditingState())
		{
			// Update our entities
			EntityManager::GetInstance()->Update(dt);
			clearKeyDisplay();
			/*Create random enemies around the map.*/
			createEnemies(dt);
			/*Create random crates for player to hide behind.*/
			createCrates(dt);
			/*Create random bullet power-up for player.*/
			createBullets(dt);
			/*Create random health power-up for player.*/
			createHealth(dt);

			//cout << "Light Position X: " << lights[0]->position.x << endl;
			//cout << "Light Position Y: " << lights[0]->position.y << endl;
			//cout << "Light Position Z: " << lights[0]->position.z << endl;

			//if (KeyboardController::GetInstance()->IsKeyDown('I'))
			//	lights[0]->position.z += 100.f * dt;

			//if (KeyboardController::GetInstance()->IsKeyDown('K'))
			//	lights[0]->position.z -= 100.f * dt;

			//if (KeyboardController::GetInstance()->IsKeyDown('L'))
			//	lights[0]->position.x += 100.f * dt;

			//if (KeyboardController::GetInstance()->IsKeyDown('J'))
			//	lights[0]->position.x -= 100.f * dt;

			//if (KeyboardController::GetInstance()->IsKeyDown('U'))
			//	lights[0]->position.y -= 100.f * dt;

			//if (KeyboardController::GetInstance()->IsKeyDown('O'))
			//	lights[0]->position.y += 100.f * dt;

			if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
			{
				cout << "Left Mouse Button was released!" << endl;
			}
			if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
			{
				cout << "Right Mouse Button was released!" << endl;
			}
			if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
			{
				cout << "Middle Mouse Button was released!" << endl;
			}
			if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
			{
				cout << "Mouse Wheel has offset in X-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) << endl;
			}

			static float printInterval = 0;
			printInterval += static_cast<float>(dt);

			if (printInterval > 0.1f)
			{
				std::ostringstream weaponType;
				string weaponName = "";

				weaponType.precision(4);
				if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
				{
					//cout << "Mouse Wheel has offset in Y-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) << endl;
					if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) == 1)
					{
						weaponName = "Assault Rifle";
						EntityManager::GetInstance()->RemoveEntity(weaponUI);
						//weaponUI = Create::Sprite2DObject("ASSAULT", Vector3(-350.0f, -250.0f, 0.f), Vector3(100.0f, 40.0f, 100.0f), true);
					}
				}
				if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) == 0)
				{
					weaponName = "Pistol";
					EntityManager::GetInstance()->RemoveEntity(weaponUI);
					//weaponUI = Create::Sprite2DObject("PISTOL", Vector3(-350.0f, -250.0f, 0.f), Vector3(50.0f, 50.0f, 50.0f), true);
				}

				weaponType << weaponName;
				textObj[4]->SetPosition(Vector3(-360.f, -220.f, 0.f));
				textObj[4]->SetText(weaponType.str());
				printInterval = 0.f;
			}

			/*Display weapon info.*/
			std::ostringstream ss;
			ss << weaponManager[playerInfo->GetWeapon()]->GetMagRound() << "/" << weaponManager[playerInfo->GetWeapon()]->GetTotalRound();
			textObj[5]->SetPosition(Vector3(-350.f, -200.f, 0.f));
			textObj[5]->SetText(ss.str());

			/*Display player health.*/
			ss.str("");
			ss << "Health:" << playerInfo->getAttribute(CAttributes::TYPE_HEALTH);
			textObj[23]->SetColor(Color(1.f, 0.f, 0.f));
			textObj[23]->SetText(ss.str());

			/*Display score*/
			ss.str("");
			ss << "Score:" << playerInfo->getScore();
			textObj[24]->SetPosition(Vector3(textObj[23]->GetPosition().x + 325.f, textObj[23]->GetPosition().y, textObj[23]->GetPosition().z));
			textObj[24]->SetColor(Color(1.f, 0.f, 0.f));
			textObj[24]->SetText(ss.str());

			ss.str("");
			ss << "Highscore:" << OptionsManager::GetInstance()->getHighscore();
			textObj[25]->SetPosition(Vector3(textObj[24]->GetPosition().x- 200.f, textObj[24]->GetPosition().y -570.f, textObj[24]->GetPosition().z));
			textObj[25]->SetColor(Color(1.f, 0.f, 0.f));
			textObj[25]->SetText(ss.str());


			if (playerInfo->getScore() > OptionsManager::GetInstance()->getHighscore())
			{
				OptionsManager::GetInstance()->setHighscore(playerInfo->getScore());
				OptionsManager::GetInstance()->saveHighscore();
			}

			// Hardware Abstraction
			theKeyboard->Read(dt);
			theMouse->Read(dt);

			// Update the player position and other details based on keyboard and mouse inputs
			playerInfo->Update(dt);

			// Update NPC
			//enemyInfo->Update(dt);
			//anEnemy3D->SetTarget(playerInfo->GetPos());
			GraphicsManager::GetInstance()->UpdateLights(dt);

			// Update camera effects
			theCameraEffects->Update(dt);


			/*Map Editor*/
			if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD7) && !Map_Editor::GetInstance()->mapEditing)
				Map_Editor::GetInstance()->mapEditing = true;

			if (Map_Editor::GetInstance()->mapEditing)
				Map_Editor::GetInstance()->updateOption(dt);

			ParticleManager::GetInstance()->updateParticle(dt);


			if (KeyboardController::GetInstance()->IsKeyPressed('M'))
			{
				if (theMinimap->enlargedMap)
					theMinimap->enlargedMap = false;
				else
					theMinimap->enlargedMap = true;
			}
		}
	}
	else
	{
		if (KeyboardController::GetInstance()->IsKeyPressed('G'))
		{
			CPlayerInfo::GetInstance()->setHealthTo(100.f);
			CPlayerInfo::GetInstance()->setScore(0.f);
		}
	}
}

void SceneText::Render()
{
	RenderPassGPass();
	RenderPassMain();
}

void SceneText::displayControls(void)
{
	std::ostringstream ss;
	ss << "[Enter] to save changes.";
	controlText[16]->SetText(ss.str());
	ss.str("");
	ss << "[TAB] to go back";
	controlText[15]->SetText(ss.str());
	ss.str("");
	ss << "LEFT [1]: " << checkInput(OptionsManager::GetInstance()->getInput(0));
	controlText[14]->SetText(ss.str());
	ss.str("");
	ss << "RIGHT [2]: " << checkInput(OptionsManager::GetInstance()->getInput(1));
	controlText[13]->SetText(ss.str());
	ss.str("");
	ss << "UP [3]: " << checkInput(OptionsManager::GetInstance()->getInput(2));
	controlText[12]->SetText(ss.str());
	ss.str("");
	ss << "DOWN [4]: " << checkInput(OptionsManager::GetInstance()->getInput(3));
	controlText[11]->SetText(ss.str());
	ss.str("");
	ss << "LOOKLEFT [5]: " << checkInput(OptionsManager::GetInstance()->getInput(4));
	controlText[10]->SetText(ss.str());
	ss.str("");
	ss << "LOOKRIGHT [6]: " << checkInput(OptionsManager::GetInstance()->getInput(5));
	controlText[9]->SetText(ss.str());
	ss.str("");
	ss << "LOOKUP [7]: " << checkInput(OptionsManager::GetInstance()->getInput(6));
	controlText[8]->SetText(ss.str());
	ss.str("");
	ss << "LOOKDOWN [8]: " << checkInput(OptionsManager::GetInstance()->getInput(7));
	controlText[7]->SetText(ss.str());
	ss.str("");
	ss << "RUN [9]: " << checkInput(OptionsManager::GetInstance()->getInput(8));
	controlText[6]->SetText(ss.str());
	ss.str("");
	ss << "CROUCH [0]: " << checkInput(OptionsManager::GetInstance()->getInput(9));
	controlText[5]->SetText(ss.str());
	ss.str("");
	ss << "JUMP [Q]: " << checkInput(OptionsManager::GetInstance()->getInput(10));
	controlText[4]->SetText(ss.str());
	ss.str("");
	ss << "RELOAD [W]: " << checkInput(OptionsManager::GetInstance()->getInput(11));
	controlText[3]->SetText(ss.str());
	ss.str("");
	ss << "CHANGE [E]: " << checkInput(OptionsManager::GetInstance()->getInput(12));
	controlText[2]->SetText(ss.str());
	ss.str("");
	ss << "RESET [R]: " << checkInput(OptionsManager::GetInstance()->getInput(13));
	controlText[1]->SetText(ss.str());
	ss.str("");
	ss << "FIRE [T]: " << checkInput(OptionsManager::GetInstance()->getInput(14));
	controlText[0]->SetText(ss.str());
}

void SceneText::renderWeapon(void)
{
	Mesh* modelMesh;
	if (playerInfo->GetWeapon() == 0)
		modelMesh = MeshBuilder::GetInstance()->GetMesh("PLAYER_PISTOL");
	if (playerInfo->GetWeapon() == 1)
		modelMesh = MeshBuilder::GetInstance()->GetMesh("PLAYER_ASSAULT");

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	if (playerInfo->GetWeapon() == 0)
	{
		modelStack.Translate(300.f, -250.f, 0.f);
		if (!weaponManager[playerInfo->GetWeapon()]->GetCanFire())
		{
			CSoundEngine::GetInstance()->PlayASound("PISTOL");
			modelStack.Rotate(-20.f, 0.f, 0.f, 1.f);
		}
		modelStack.Scale(290.f, 200.f, 50.f);
	}
	if (playerInfo->GetWeapon() == 1)
	{
		modelStack.Translate(250.f, -250.f, 0.f);
		if (!weaponManager[playerInfo->GetWeapon()]->GetCanFire())
		{
			CSoundEngine::GetInstance()->PlayASound("ASSAULT");
			modelStack.Rotate(-10.f, 0.f, 0.f, 1.f);
		}
		modelStack.Scale(400.f, 250.f, 1.f);
	}
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

void SceneText::renderWeaponUI(void)
{
	Mesh* modelMesh;
	if (playerInfo->GetWeapon() == 0)
		modelMesh = MeshBuilder::GetInstance()->GetMesh("PISTOL");
	if (playerInfo->GetWeapon() == 1)
		modelMesh = MeshBuilder::GetInstance()->GetMesh("ASSAULT");

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	if (playerInfo->GetWeapon() == 0)
	{
		modelStack.Translate(-330.f, -150.f, 0.f);
		if (!weaponManager[playerInfo->GetWeapon()]->GetCanFire())
			modelStack.Rotate(-10.f, 0.f, 0.f, 1.f);
		modelStack.Scale(50.0f, 50.0f, 1.f);
	}
	if (playerInfo->GetWeapon() == 1)
	{
		modelStack.Translate(-330.f, -150.f, 0.f);
		if (!weaponManager[playerInfo->GetWeapon()]->GetCanFire())
			modelStack.Rotate(-10.f, 0.f, 0.f, 1.f);
		modelStack.Scale(100.0f, 40.0f, 1.f);
	}
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

void SceneText::createEnemies(double dt)
{
	//static bool once = false;

	//if (!once)
	//{
	//	srand(time(NULL));
	//	CPatrol* enemy = Create::Patrol("ROBOT", Vector3(50.f, -10.f, 0.f), Vector3(3.f, 3.f, 3.f));
	//	enemy->SetAABB(Vector3(3.f, 7.f, 3.f), Vector3(-3.f, -7.f, -3.f));
	//	//for (int i = 0; i < 10; ++i)
	//	//{
	//	//	enemy->addWaypoint(Vector3(Math::RandFloatMinMax(-50.f, 50.f), /*Math::RandFloatMinMax(0.f, 10.f)*/ -10.f, Math::RandFloatMinMax(-50.f, 50.f)));
	//	//}
	//	enemy->SetLight(true);
	//	once = true;
	//}

	/*Tower*/
	//static float increaseEnemies = 0.f;
	//increaseEnemies += (float)dt;

	//static int count = 0;
	//if (count < 100)
	//{
	//	if (EntityManager::GetInstance()->enemyCount() < increaseEnemies / 3.f)
	//	{
	//		Vector3 newPosition(Math::RandFloatMinMax(-350.f, 350.f), 0.f, Math::RandFloatMinMax(-350.f, 350.f));
	//		Vector3 _minAABB(-5.f, 0.f, -5.f);
	//		Vector3 _maxAABB(5.f, 5.f, 5.f);
	//		Vector3 _scale(3.f, 3.f, 3.f);
	//		/*While the new enemy collides with any other objects in the scene, keep randomising the position.*/
	//		while (EntityManager::GetInstance()->getSpawnPosition(_minAABB, _maxAABB, newPosition))
	//		{
	//			newPosition.Set(Math::RandFloatMinMax(-350.f, 350.f), 0.f, Math::RandFloatMinMax(-350.f, 350.f));
	//		}
	//		// Create a CEnemy instance
	//		//anEnemy3D = Create::Enemy3D("crate", Vector3(-20.0f, 0.0f, -20.0f), Vector3(2.f, 10.f, 3.f));
	//		anEnemy3D = Create::Enemy3D("turret", newPosition, _scale);
	//		//anEnemy3D->Init();
	//		anEnemy3D->setAlertBoundary(Vector3(-150.f, -10.f, -150.f), Vector3(150.f, 10.f, 150.f));
	//		anEnemy3D->SetCollider(true);
	//		anEnemy3D->SetLight(true);
	//		anEnemy3D->SetAABB(Vector3(_scale.x, 0.f, _scale.z), Vector3(-_scale.x, -5.f, -_scale.z));
	//		anEnemy3D->setMaxHealthTo(10.f);
	//		anEnemy3D->setHealthTo(10.f);
	//		anEnemy3D->setAttackTo(1.f);
	//		anEnemy3D->setDefenseTo(1.f);
	//		//anEnemy3D->SetTerrain(groundEntity);
	//		anEnemy3D->SetLight(true);

	//		++count;
	//	}
	//}

}

void SceneText::createCrates(double dt)
{
	static float increaseCrates = 0.f;
	increaseCrates += (float)dt;
	static int crateCount = 0;

	if (crateCount < increaseCrates / 2.f)
	{

		Vector3 newPosition(Math::RandFloatMinMax(-350.f, 350.f), -10.f, Math::RandFloatMinMax(-350.f, 350.f));
		Vector3 _minAABB(-6.f, -6.f, -6.f);
		Vector3 _maxAABB(6.f, 6.f, 6.f);
		/*While the new enemy collides with any other objects in the scene, keep randomising the position.*/
		while (EntityManager::GetInstance()->getSpawnPosition(_minAABB, _maxAABB, newPosition))
		{
			newPosition.Set(Math::RandFloatMinMax(-350.f, 350.f), -10.f, Math::RandFloatMinMax(-350.f, 350.f));
		}
		CFurniture* crate = Create::Furniture("crate", newPosition, Vector3(5.f, 5.f, 5.f));
		crate->SetCollider(true);
		crate->SetLight(true);
		crate->SetAABB(_maxAABB, _minAABB);
		crate->SetLight(true);
		++crateCount;
	}
}

void SceneText::createBullets(double dt)
{
	static float increaseBullets = 0.f;
	increaseBullets += (float)dt;
	static int bulletCount = 0;

	if (bulletCount < increaseBullets / 4.f)
	{
		Vector3 newPosition(Math::RandFloatMinMax(-350.f, 350.f), 0.f, Math::RandFloatMinMax(-350.f, 350.f));
		Vector3 _minAABB(-5.f, -5.f, -5.f);
		Vector3 _maxAABB(5.f, 5.f, 5.f);
		/*While the new enemy collides with any other objects in the scene, keep randomising the position.*/
		while (EntityManager::GetInstance()->getSpawnPosition(_minAABB, _maxAABB, newPosition))
		{
			newPosition.Set(Math::RandFloatMinMax(-350.f, 350.f), 0.f, Math::RandFloatMinMax(-350.f, 350.f));
		}
		CBullet* bullet = Create::Bullet("POWERUP_BULLET", newPosition, Vector3(5.f, 5.f, 5.f));
		bullet->SetCollider(true);
		bullet->SetLight(true);
		bullet->SetAABB(_maxAABB, _minAABB);
		bullet->SetItem(EntityBase::BULLET);
		++bulletCount;
	}
}

void SceneText::createHealth(double dt)
{
	static float increaseHealth = 0.f;
	increaseHealth += (float)dt;
	static int healthCount = 0;

	if (healthCount < increaseHealth / 4.f)
	{
		Vector3 newPosition(Math::RandFloatMinMax(-350.f, 350.f), 0.f, Math::RandFloatMinMax(-350.f, 350.f));
		Vector3 _minAABB(-5.f, -5.f, -5.f);
		Vector3 _maxAABB(5.f, 5.f, 5.f);
		/*While the new enemy collides with any other objects in the scene, keep randomising the position.*/
		while (EntityManager::GetInstance()->getSpawnPosition(_minAABB, _maxAABB, newPosition))
		{
			newPosition.Set(Math::RandFloatMinMax(-350.f, 350.f), 0.f, Math::RandFloatMinMax(-350.f, 350.f));
		}
		CHealth* health = Create::Health("POWERUP_HEALTH", newPosition, Vector3(5.f, 5.f, 5.f));
		health->SetCollider(true);
		health->SetLight(true);
		health->SetAABB(_maxAABB, _minAABB);
		health->SetItem(EntityBase::HEALTH);
		++healthCount;
	}
}

void SceneText::renderHit(void)
{

	Mesh* modelMesh;
	modelMesh = MeshBuilder::GetInstance()->GetMesh("HIT");
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(0.f, 150.f, 0.f);
	modelStack.Rotate(0.f, 0.f, 0.f, 1.f);
	modelStack.Scale(200.f, 200.f, 1.f);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

void SceneText::pauseOptions(double dt, bool &pause)
{
	OptionsManager::GetInstance()->setEditingState(true);
	static bool choseType = false;
	static bool changedInput = false;

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_TAB))
	{
		choseType = false;
		changedInput = false;
		OptionsManager::GetInstance()->setEditingState(false);
		pause = false;
	}

	if (!choseType && !changedInput)
	{
		static int option = 20;
		cout << "Choose Type to Edit." << endl;
		if (KeyboardController::GetInstance()->IsKeyDown('1'))
			option = 0;
		if (KeyboardController::GetInstance()->IsKeyDown('2'))
			option = 1;
		if (KeyboardController::GetInstance()->IsKeyDown('3'))
			option = 2;
		if (KeyboardController::GetInstance()->IsKeyDown('4'))
			option = 3;
		if (KeyboardController::GetInstance()->IsKeyDown('5'))
			option = 4;
		if (KeyboardController::GetInstance()->IsKeyDown('6'))
			option = 5;
		if (KeyboardController::GetInstance()->IsKeyDown('7'))
			option = 6;
		if (KeyboardController::GetInstance()->IsKeyDown('8'))
			option = 7;
		if (KeyboardController::GetInstance()->IsKeyDown('9'))
			option = 8;
		if (KeyboardController::GetInstance()->IsKeyDown('0'))
			option = 9;
		if (KeyboardController::GetInstance()->IsKeyDown('Q'))
			option = 10;
		if (KeyboardController::GetInstance()->IsKeyDown('W'))
			option = 11;
		if (KeyboardController::GetInstance()->IsKeyDown('E'))
			option = 12;
		if (KeyboardController::GetInstance()->IsKeyDown('R'))
			option = 13;
		if (KeyboardController::GetInstance()->IsKeyDown('T'))
			option = 14;

		for (vector<Controls*>::iterator it = OptionsManager::GetInstance()->getControls().begin(); it != OptionsManager::GetInstance()->getControls().end(); ++it)
		{
			Controls* findControl = (Controls*)*it;
			if (findControl->checkType(option))
			{
				input = findControl;
				choseType = true;
			}
		}

	}

	if (choseType && !changedInput)
	{
		cout << input << endl;
		//cout << "Current Input: " << input->getKey() << endl;
		if (KeyboardController::GetInstance()->IsKeyPressed('A'))
			input->setKey('A');
		if (KeyboardController::GetInstance()->IsKeyPressed('B'))
			input->setKey('B');
		if (KeyboardController::GetInstance()->IsKeyPressed('C'))
			input->setKey('C');
		if (KeyboardController::GetInstance()->IsKeyPressed('D'))
			input->setKey('D');
		if (KeyboardController::GetInstance()->IsKeyPressed('E'))
			input->setKey('E');
		if (KeyboardController::GetInstance()->IsKeyPressed('F'))
			input->setKey('F');
		if (KeyboardController::GetInstance()->IsKeyPressed('G'))
			input->setKey('G');
		if (KeyboardController::GetInstance()->IsKeyPressed('H'))
			input->setKey('H');
		if (KeyboardController::GetInstance()->IsKeyPressed('I'))
			input->setKey('I');
		if (KeyboardController::GetInstance()->IsKeyPressed('J'))
			input->setKey('J');
		if (KeyboardController::GetInstance()->IsKeyPressed('K'))
			input->setKey('K');
		if (KeyboardController::GetInstance()->IsKeyPressed('L'))
			input->setKey('L');
		if (KeyboardController::GetInstance()->IsKeyPressed('M'))
			input->setKey('M');
		if (KeyboardController::GetInstance()->IsKeyPressed('N'))
			input->setKey('N');
		if (KeyboardController::GetInstance()->IsKeyPressed('O'))
			input->setKey('O');
		if (KeyboardController::GetInstance()->IsKeyPressed('P'))
			input->setKey('P');
		if (KeyboardController::GetInstance()->IsKeyPressed('Q'))
			input->setKey('Q');
		if (KeyboardController::GetInstance()->IsKeyPressed('R'))
			input->setKey('R');
		if (KeyboardController::GetInstance()->IsKeyPressed('S'))
			input->setKey('S');
		if (KeyboardController::GetInstance()->IsKeyPressed('T'))
			input->setKey('T');
		if (KeyboardController::GetInstance()->IsKeyPressed('U'))
			input->setKey('U');
		if (KeyboardController::GetInstance()->IsKeyPressed('V'))
			input->setKey('V');
		if (KeyboardController::GetInstance()->IsKeyPressed('W'))
			input->setKey('W');
		if (KeyboardController::GetInstance()->IsKeyPressed('X'))
			input->setKey('X');
		if (KeyboardController::GetInstance()->IsKeyPressed('Y'))
			input->setKey('Y');
		if (KeyboardController::GetInstance()->IsKeyPressed('Z'))
			input->setKey('Z');
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
			input->setKey(32);
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_LEFT))
			input->setKey(39);
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RIGHT))
			input->setKey(37);
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			input->setKey(40);
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			input->setKey(38);
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_LSHIFT))
			input->setKey(160);
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_LCONTROL))
			input->setKey(162);
		if (MouseController::GetInstance()->IsButtonPressed(MouseController::LMB))
			input->setKey(1);

		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
		{
			OptionsManager::GetInstance()->setEditingState(false);
			OptionsManager::GetInstance()->saveConfig();
			choseType = false;
			changedInput = false;
			pause = false;
		}

		if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD5))
		{
			OptionsManager::GetInstance()->setEditingState(false);
			OptionsManager::GetInstance()->defaultConfig();
			OptionsManager::GetInstance()->loadConfig("Data//keyconfig");
			choseType = false;
			changedInput = false;
			pause = false;
		}
	}

	cout << "TEST" << endl;

}

void SceneText::clearKeyDisplay(void)
{
	for (int i = 0; i < 17; ++i)
	{
		controlText[i]->SetText("");
	}
}

void SceneText::RenderPassGPass(void)
{
	DepthFBO::GetInstance()->m_renderPass = DepthFBO::RENDER_PASS_PRE;

	DepthFBO::GetInstance()->BindForWriting();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT);

	GraphicsManager* g = GraphicsManager::GetInstance();
	g->SetActiveShader("gpass");

	/*glUseProgram(g);*/

	if (lights[0]->type == Light::LIGHT_DIRECTIONAL)
	{
		DepthFBO::GetInstance()->m_lightDepthProj.SetToOrtho(-150, 150, -150, 150, -1000, 2000);
	}
	else
	{
		DepthFBO::GetInstance()->m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 20);
	}

	//if (lights[1]->type == Light::LIGHT_POINT)
	{
	//	DepthFBO::GetInstance()->m_lightDepthProj.SetToOrtho(-150, 150, -150, 150, -1000, 2000);
	}
	//else
	{
	//	DepthFBO::GetInstance()->m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 20);
	}

	DepthFBO::GetInstance()->m_lightDepthView.SetToLookAt(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z, 0, 0, 0, 0, 1, 0);


	RenderWorld();
}

void SceneText::RenderPassMain(void)
{
	DepthFBO::GetInstance()->m_renderPass = DepthFBO::RENDER_PASS_MAIN;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glViewport(0, 0, 800, 600);
	glViewport(0, 0, Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight());
	//glViewport(0, 0, m_worldWidth, m_worldHeight);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->SetActiveShader("default");
	//glUseProgram(m_programID);

	//pass light depth texture 
	DepthFBO::GetInstance()->BindForReading(GL_TEXTURE8);
	currProg->UpdateInt("shadowMap", 8);
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);

	// Camera matrix
	GraphicsManager::GetInstance()->GetViewMatrix().SetToIdentity();
	GraphicsManager::GetInstance()->GetViewMatrix().SetToLookAt(
		camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z,
		camera.GetCameraTarget().x, camera.GetCameraTarget().y, camera.GetCameraTarget().z,
		camera.GetCameraUp().x, camera.GetCameraUp().y, camera.GetCameraUp().z);

	// Model matrix : an identity matrix (model will be at the origin)
	GraphicsManager::GetInstance()->GetModelStack().LoadIdentity();

	if (lights[0]->type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z);
		Vector3 lightDirection_cameraspace = GraphicsManager::GetInstance()->GetViewMatrix() * lightDir;
		currProg->UpdateVector3("lights[0].position_cameraspace", &lightDirection_cameraspace.x);
	}
	else if (lights[0]->type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = GraphicsManager::GetInstance()->GetViewMatrix() * lights[0]->position;
		currProg->UpdateVector3("lights[0].position_cameraspace", &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = GraphicsManager::GetInstance()->GetViewMatrix() * lights[0]->spotDirection;
		currProg->UpdateVector3("lights[0].spotDirection", &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = GraphicsManager::GetInstance()->GetViewMatrix() * lights[0]->position;
		currProg->UpdateVector3("lights[0].position_cameraspace", &lightPosition_cameraspace.x);
	}

	if (lights[1]->type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[1]->position.x, lights[1]->position.y, lights[1]->position.z);
		Vector3 lightDirection_cameraspace = GraphicsManager::GetInstance()->GetViewMatrix() * lightDir;
		currProg->UpdateVector3("lights[1].position_cameraspace", &lightDirection_cameraspace.x);
	}
	else if (lights[1]->type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = GraphicsManager::GetInstance()->GetViewMatrix() * lights[1]->position;
		currProg->UpdateVector3("lights[1].position_cameraspace", &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = GraphicsManager::GetInstance()->GetViewMatrix() * lights[1]->spotDirection;
		currProg->UpdateVector3("lights[1].spotDirection", &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = GraphicsManager::GetInstance()->GetViewMatrix() * lights[1]->position;
		currProg->UpdateVector3("lights[1].position_cameraspace", &lightPosition_cameraspace.x);
	}


	//RenderMesh(meshList[GEO_AXES], false);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	/*Debug Quad*/
	/*MS& ms = GraphicsManager::GetInstance()->GetModelStack();
	ms.PushMatrix();
	ms.Scale(100, 100, 100);
	RenderHelper::RenderMesh(TEMPDEPTHQUAD);
	ms.PopMatrix();*/
	/*------------------------------------------*/

	//currProg->UpdateInt("fogParam.enabled", 1);
	RenderWorld();
	//currProg->UpdateInt("fogParam.enabled", 0);



	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;
	GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();
	EntityManager::GetInstance()->RenderUI();

	/*Render Weapon*/
	renderWeapon();

	//*Render Camera Effects*/
	theCameraEffects->RenderUI();

	//*Render Weapon UI*/
	renderWeaponUI();

	//*Render Hit*/
	if (EntityManager::GetInstance()->getHit())
		renderHit();

	if (openInventory)
	{
		FileManager::GetInstance()->RenderWeapon();
	}

	glEnable(GL_DEPTH_TEST);

	/*Render Minimap*/
	theMinimap->RenderUI();

	/*Render on Screen*/
	glDisable(GL_DEPTH_TEST);
	//*Render KO Count*/
	RenderHelper::GetInstance()->renderKOCount();
	//*Render Player Health Bar*/
	RenderHelper::GetInstance()->renderPlayerHealth();

	/*Render Map Editor Options.*/
	if (Map_Editor::GetInstance()->mapEditing)
		Map_Editor::GetInstance()->renderOption();

	/*3D Render*/
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	
	// Render LightBall
	//modelStack.PushMatrix();
	//modelStack.Rotate(lightMove, 0.f, 1.f, 0.f);
	//modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	//RenderMesh(meshList[GEO_LIGHTBALL], false);
	//modelStack.PopMatrix();

	//// Render the crosshair
	//RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 10.0f);

	//if (debugInfo)
	//{
	//	std::ostringstream ss;
	//	ss.precision(5);
	//	ss << "Particles: " << m_particleCount;
	//	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 1.5, 15);


	//	ss.str("");
	//	ss.precision(5);
	//	ss << "FPS: " << fps;
	//	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 1.5, 12);

	//	std::ostringstream ss1;
	//	ss1.precision(4);
	//	ss1 << "Light(" << lights[0].position.x << ", " << lights[0].position.y << ", " << lights[0].position.z << ")";
	//	RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(0, 1, 0), 3, 1.5, 9);

	//	std::string xPosition = "X: " + to_string((int)camera.position.x);
	//	std::string yPosition = "Y: " + to_string((int)camera.position.y);
	//	std::string zPosition = "Z: " + to_string((int)camera.position.z);

	//	RenderTextOnScreen(meshList[GEO_TEXT], xPosition, Color(0, 1, 0), 3, 1.5, 6);
	//	RenderTextOnScreen(meshList[GEO_TEXT], yPosition, Color(0, 1, 0), 3, 1.5, 3);
	//	RenderTextOnScreen(meshList[GEO_TEXT], zPosition, Color(0, 1, 0), 3, 1.5, 0);
	//}
}

void SceneText::RenderWorld(void)
{
	/*Debug*/
	//CPlayerInfo::GetInstance()->setHealth(CPlayerInfo::GetInstance()->getHealth() - 5);

	if (playerInfo->getAttribute(CAttributes::TYPE_HEALTH) <= 0)
	{
		Mesh* modelMesh;
		modelMesh = MeshBuilder::GetInstance()->GetMesh("GAMEOVER");
		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		modelStack.PushMatrix();
		modelStack.Translate(0.f, 0.f, 0.f);
		modelStack.Rotate(0.f, 0.f, 0.f, 1.f);
		modelStack.Scale(400.f, 300.f, 1.f);
		RenderHelper::RenderMesh(modelMesh);
		modelStack.PopMatrix();
	}
	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	EntityManager::GetInstance()->Render();


	if (Map_Editor::GetInstance()->mapEditing)
		Map_Editor::GetInstance()->renderObject();

	ParticleManager::GetInstance()->renderAllParticle();


	/*Debug Quad for Shadow*/
	/*MS& ms = GraphicsManager::GetInstance()->GetModelStack();
	ms.PushMatrix();
	ms.Translate(0, 10, 10);
	ms.Scale(5, 5, 5);
	RenderHelper::RenderMeshWithLight(TEMPSPHERE);
	ms.PopMatrix();

	ms.PushMatrix();
	ms.Translate(0, -8, 0);
	ms.Rotate(-90, 1, 0, 0);
	ms.Scale(1000, 1000, 1000);
	RenderHelper::RenderMeshWithLight(TEMPQUAD);
	ms.PopMatrix();*/
	/*--------------------------------------------------------*/
}

void SceneText::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
	playerInfo->DetachCamera();

	if (playerInfo->DropInstance() == false)
	{
#if _DEBUGMODE==1
		cout << "Unable to drop PlayerInfo class" << endl;
#endif
	}


	delete TEMPSPHERE;
	delete TEMPDEPTHQUAD;


	// Delete the lights
	delete lights[0];
	delete lights[1];

	if (theCameraEffects)
	{
		delete theCameraEffects;
		theCameraEffects = NULL;
	}
	if (theMinimap)
	{
		delete theMinimap;
		theMinimap = NULL;
	}
	if (theMouse)
	{
		delete theMouse;
		theMouse = NULL;
	}
	if (theKeyboard)
	{
		delete theKeyboard;
		theKeyboard = NULL;
	}

	if (currProg)
	{
		delete currProg;
		currProg = nullptr;
	}

	if (m_gPassShaderID)
	{
		delete m_gPassShaderID;
		m_gPassShaderID = nullptr;
	}

	//if (playerInfo)
	//{
	//	delete playerInfo;
	//	playerInfo = nullptr;
	//}

	//if (groundEntity)
	//{
	//	delete groundEntity;
	//	groundEntity = nullptr;
	//}

	//if (theOption)
	//{
	//	delete theOption;
	//	theOption = nullptr;
	//}

	//if (theCube)
	//{
	//	delete theCube;
	//	theCube = nullptr;
	//}

	//if (anEnemy3D)
	//{
	//	delete anEnemy3D;
	//	anEnemy3D = nullptr;
	//}

	//if (input)
	//{
	//	delete input;
	//	input = nullptr;
	//}

	//if (optionUI)
	//{
	//	delete optionUI;
	//	optionUI = nullptr;
	//}

	//if (weaponUI)
	//{
	//	delete weaponUI;
	//	weaponUI = nullptr;
	//}

	//if (hitDisplay)
	//{
	//	delete hitDisplay;
	//	hitDisplay = nullptr;
	//}

	//if (camera)
	//{
	//	delete camera;
	//	camera = nullptr;
	//}

	while (theObjects.size() > 0)
	{
		GenericEntity* object = theObjects.back();
		delete object;
		object = nullptr;
		theObjects.pop_back();
	}

	while (controls.size() > 0)
	{
		Controls* object = controls.back();
		delete object;
		object = nullptr;
		controls.pop_back();
	}

	for (int i = 0; i < 17; ++i)
	{
		delete controlText[i];
	}

	for (int i = 0; i < 30; ++i)
	{
		delete textObj[i];
	}
}
