#include "Selection.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "../Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "../EntityManager.h"

#include "../GenericEntity.h"
#include "../GroundEntity.h"
#include "../TextEntity.h"
#include "../SpriteEntity.h"
#include "../Light.h"
#include "../SkyBox/SkyBoxEntity.h"
#include "../Minimap\Minimap.h"
#include "../Object\Furniture.h"
#include "../Object\Bullet.h"
#include "../Object\Health.h"

/*Options Manager*/
#include "../Options\OptionsManager.h"
#include "../Options\Controls.h"

/*Rendering*/
#include "RenderHelper.h"

/*Sound*/
#include "../SoundEngine.h"

/*Enemies*/
#include "../Enemy\Enemy3D.h"
#include "../Enemy/AnimatedEnemy/AnimatedEnemy.h"
#include "../Enemy\Patrol\Patrol.h"

#include "../Items\Inventory.h"
#include "../Items\Helmet.h"
#include "../FileManager.h"
#include "../Items\EquipmentManager.h"

/*Map Editor*/
#include "../Map_Editor\Map_Editor.h"

/*Cinematic Camera*/
#include "../Cinematic\Cinematic.h"

/*DIsplaying Text*/
#include "../Text_Display\Text\Text.h"
#include "../Text_Display\Text_Manager\Text_Manager.h"

#include <iostream>

#include "../Attributes.h"

using namespace std;

Selection* Selection::sInstance = new Selection(SceneManager::GetInstance());

Selection::Selection()
	: theMinimap(NULL)
	, theCameraEffects(NULL)
	//, currentHighscore(0)
	//, cinematicMode(false)
	, windowHeight(0.f)
	, windowWidth(0.f)
	//, m_worldHeight(0.f)
	//, m_worldWidth(0.f)
{
}

Selection::Selection(SceneManager* _sceneMgr)
	: theMinimap(NULL)
	, theCameraEffects(NULL)
	//, currentHighscore(0)
	//, cinematicMode(false)
{
	_sceneMgr->AddScene("Selection", this);
}

Selection::~Selection()
{
}

void Selection::Init()
{
	//Calculating aspect ratio
	windowHeight = Application::GetInstance().GetWindowHeight();
	windowWidth = Application::GetInstance().GetWindowWidth();
	//Application::GetInstance().setAspectRatioHeight(100.f);
	//Application::GetInstance().setAspectRatioWidth(Application::GetInstance().getAspectRatioHeight() * (float)Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight());
	//m_worldWidth = m_worldHeight * (float)Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight();
	//m_worldHeight = (float)Application::GetInstance().GetWindowHeight();
	//m_worldWidth = (float)Application::GetInstance().GetWindowWidth();

	//currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");
	//currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Shadow.vertexshader", "Shader//Shadow.fragmentshader");
	//m_gPassShaderID = GraphicsManager::GetInstance()->LoadShader("gpass", "Shader//GPass.vertexshader", "Shader//GPass.fragmentshader");

	//// Tell the shader program to store these uniform locations
	//currProg->AddUniform("MVP");
	//currProg->AddUniform("MV");
	//currProg->AddUniform("MV_inverse_transpose");
	//currProg->AddUniform("material.kAmbient");
	//currProg->AddUniform("material.kDiffuse");
	//currProg->AddUniform("material.kSpecular");
	//currProg->AddUniform("material.kShininess");
	//currProg->AddUniform("lightEnabled");
	//currProg->AddUniform("numLights");
	//currProg->AddUniform("lights[0].type");
	//currProg->AddUniform("lights[0].position_cameraspace");
	//currProg->AddUniform("lights[0].color");
	//currProg->AddUniform("lights[0].power");
	//currProg->AddUniform("lights[0].kC");
	//currProg->AddUniform("lights[0].kL");
	//currProg->AddUniform("lights[0].kQ");
	//currProg->AddUniform("lights[0].spotDirection");
	//currProg->AddUniform("lights[0].cosCutoff");
	//currProg->AddUniform("lights[0].cosInner");
	//currProg->AddUniform("lights[0].exponent");
	//currProg->AddUniform("lights[1].type");
	//currProg->AddUniform("lights[1].position_cameraspace");
	//currProg->AddUniform("lights[1].color");
	//currProg->AddUniform("lights[1].power");
	//currProg->AddUniform("lights[1].kC");
	//currProg->AddUniform("lights[1].kL");
	//currProg->AddUniform("lights[1].kQ");
	//currProg->AddUniform("lights[1].spotDirection");
	//currProg->AddUniform("lights[1].cosCutoff");
	//currProg->AddUniform("lights[1].cosInner");
	//currProg->AddUniform("lights[1].exponent");
	//currProg->AddUniform("colorTextureEnabled");
	//currProg->AddUniform("colorTexture");
	//currProg->AddUniform("textEnabled");
	//currProg->AddUniform("textColor");

	///*Fog*/
	//currProg->AddUniform("fogParam.color");
	//currProg->AddUniform("fogParam.start");
	//currProg->AddUniform("fogParam.end");
	//currProg->AddUniform("fogParam.density");
	//currProg->AddUniform("fogParam.type");
	//currProg->AddUniform("fogParam.enabled");

	///*Shadow*/
	//currProg->AddUniform("lightDepthMVP");
	//currProg->AddUniform("shadowMap");

	//m_gPassShaderID->AddUniform("lightDepthMVP");
	//m_gPassShaderID->AddUniform("colorTextureEnabled[0]");
	//m_gPassShaderID->AddUniform("colorTexture[0]");
	//m_gPassShaderID->AddUniform("colorTextureEnabled[1]");
	//m_gPassShaderID->AddUniform("colorTexture[1]");
	//m_gPassShaderID->AddUniform("colorTextureEnabled[2]");
	//m_gPassShaderID->AddUniform("colorTexture[2]");

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

	SceneManager::GetInstance()->currProg->UpdateInt("numLights", 1);
	SceneManager::GetInstance()->currProg->UpdateInt("textEnabled", 0);

	Color fogColor(0.5f, 0.5f, 0.5f); //Vec3 Color
	SceneManager::GetInstance()->currProg->UpdateVector3("fogParam.color", &fogColor.r);
	SceneManager::GetInstance()->currProg->UpdateFloat("fogParam.start", 10);
	SceneManager::GetInstance()->currProg->UpdateFloat("fogParam.end", 2000);
	SceneManager::GetInstance()->currProg->UpdateFloat("fogParam.density", 0.005f);
	SceneManager::GetInstance()->currProg->UpdateInt("fogParam.type", 0);
	SceneManager::GetInstance()->currProg->UpdateInt("fogParam.enabled", 0);

	// Create the playerinfo instance, which manages all information about the player
	playerInfo = CPlayerInfo::GetInstance();
	playerInfo->Init();
	playerInfo->SetPos(Vector3(6.f, 0.f, 0.f));
	playerInfo->SetTarget(Vector3(6.f, 0.f, 1.f));

	// Create and attach the camera to the scene
	//camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.Init(playerInfo->GetPos(), playerInfo->GetTarget(), playerInfo->GetUp());
	cinematic = new CCinematic();
	cinematic->Init(playerInfo->GetPos(), playerInfo->GetTarget(), playerInfo->GetUp());
	playerInfo->AttachCamera(&camera);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

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
	float fontSize = Application::GetInstance().GetWindowWidth() * 0.025f;
	float halfFontSize = fontSize / 2.0f;
	for (int i = 0; i < 30; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth + fontSize, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(windowWidth * 0.04f, windowWidth * 0.04f, 1.f), Color(1.0f, 0.f, 0.0f));
	}

	for (int i = 0; i < 17; ++i)
	{
		controlText[i] = Create::Text2DObject("text", Vector3(-windowWidth * 0.25f, (-windowHeight * 0.255f) + fontSize*i + halfFontSize, 0.0f), "", Vector3(windowWidth * 0.03f, windowWidth * 0.03f, 1.f), Color(0.f, 0.f, 0.0f));
	}


	// Hardware Abstraction
	theKeyboard = new CKeyboard();
	theKeyboard->Create(playerInfo);

	// Activate the Blend Function
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*Minimap*/
	Mesh* minimapBackground;
	Mesh* minimapBorder;
	Mesh* minimapAvatar;
	Mesh* minimapStencil;

	minimapBackground = MeshBuilder::GetInstance()->GenerateCircle("MINIMAP", Color(1, 1, 1), 1.f);
	minimapBackground->textureID = MeshBuilder::GetInstance()->GetMesh("MINIMAP")->textureID = LoadTGA("Image//minimap/minimapBackground.tga");
	minimapBorder = MeshBuilder::GetInstance()->GenerateQuad("MINIMAPBORDER", Color(1, 1, 1), 1.05f);
	minimapBorder->textureID = MeshBuilder::GetInstance()->GetMesh("MINIMAPBORDER")->textureID = LoadTGA("Image//minimap//minimapBorder.tga");
	minimapAvatar = MeshBuilder::GetInstance()->GenerateQuad("MINIMAPAVATAR", Color(1, 1, 0), 0.25f);
	minimapAvatar->textureID = MeshBuilder::GetInstance()->GetMesh("MINIMAPAVATAR")->textureID = LoadTGA("Image//minimap//minimapAvatar.tga");
	minimapStencil = MeshBuilder::GetInstance()->GenerateQuad("MINIMAP_STENCIL", Color(1, 1, 1), 1.0f);
	minimapStencil->textureID = MeshBuilder::GetInstance()->GetMesh("MINIMAP_STENCIL")->textureID = LoadTGA("Image//minimap//minimapStencil.tga");
	// Minimap
	theMinimap = Create::Minimap(false);
	theMinimap->SetBackground(minimapBackground);
	theMinimap->GetBackground()->textureID = minimapBackground->textureID;
	theMinimap->SetBorder(minimapBorder);
	theMinimap->GetBorder()->textureID = minimapBorder->textureID;
	theMinimap->SetAvatar(minimapAvatar);
	theMinimap->GetAvatar()->textureID = minimapAvatar->textureID;
	theMinimap->SetStencil(minimapStencil);
	theMinimap->GetStencil()->textureID = minimapStencil->textureID;


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
	//CSoundEngine::GetInstance()->AddSound("PISTOL", "Sound\\SFX\\PISTOL.ogg");
	//CSoundEngine::GetInstance()->AddSound("ASSAULT", "Sound\\SFX\\ASSAULT.ogg");
	//CSoundEngine::GetInstance()->AddSound("TAKEDAMAGE", "Sound\\SFX\\TAKEDAMAGE.ogg");
	//CSoundEngine::GetInstance()->AddSound("RELOAD", "Sound\\SFX\\RELOAD.ogg");
	//CSoundEngine::GetInstance()->AddSound("EXPLODE", "Sound\\SFX\\EXPLODE.ogg");
	//CSoundEngine::GetInstance()->AddSound("HEAL", "Sound\\SFX\\HEAL.ogg");
	CSoundEngine::GetInstance()->GetSoundEngine()->play2D("Sound\\BGM\\HURRY.ogg", true);
	/*Shadow*/
	DepthFBO::GetInstance()->Init(1024, 1024);
	//m_lightDepthFBO.Init(1024, 1024);

	ParticleManager* particleManager = ParticleManager::GetInstance();
	vector<ParticleManager*>particleList = particleManager->getParticleList();
	for (int i = 0; i < particleManager->getMaximumParticles(); ++i)
	{
		particleManager->pushParticle(particleObject_type::P_Water);
	}
	cout << "Particle List Size in Scene: " << particleList.size() << endl;
	Inventory::GetInstance()->Init();

	/*openInventory = false;
	openEQ = false;*/
	FileManager::GetInstance()->init();
	EquipmentManager::GetInstance()->Init();

	//saveMapTime = 0;

	//FileManager::GetInstance()->ReadMapFile("Files//Selection.csv");
	//FileManager::GetInstance()->ReadEnemyFile("Files//SelectionEnemy.csv");

	selectedChoice = 1;
}

void Selection::Update(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD1))
		SceneManager::GetInstance()->SetActiveScene("Tutorial");
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD2))
		SceneManager::GetInstance()->SetActiveScene("Level01");
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD3))
		SceneManager::GetInstance()->SetActiveScene("Level02");
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD4))
		SceneManager::GetInstance()->SetActiveScene("Level03");
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD5))
		SceneManager::GetInstance()->SetActiveScene("Level04");

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
	{
		selectedChoice = Math::Max(1, selectedChoice - 1);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
	{
		selectedChoice = Math::Min(selectedChoice + 1, 7);
	}

	switch (selectedChoice)
	{
	case 1:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
		{
			SceneManager::GetInstance()->SetActiveScene("Village"); 
		}
		break;
	case 2:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
		{
			SceneManager::GetInstance()->SetActiveScene("Tutorial");
		}
		break;
	case 3:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
		{
			SceneManager::GetInstance()->SetActiveScene("Level01");
		}
		break;
	case 4:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
		{
			SceneManager::GetInstance()->SetActiveScene("Level02");
		}
		break;
	case 5:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
		{
			SceneManager::GetInstance()->SetActiveScene("Level03");
		}
		break;
	case 6:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
		{
			SceneManager::GetInstance()->SetActiveScene("Level04");
		}
		break;
	case 7:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
		{
			SceneManager::GetInstance()->SetActiveScene("Mainmenu");
		}
		break;
	default:
		break;
	}
}

void Selection::Render()
{
	RenderPassGPass();
	RenderPassMain();
}

void Selection::displayControls(void)
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

//void Selection::renderWeapon(void)
//{
//	Mesh* modelMesh;
//	if (playerInfo->GetWeapon() == 0)
//		modelMesh = MeshBuilder::GetInstance()->GetMesh("PLAYER_PISTOL");
//	if (playerInfo->GetWeapon() == 1)
//		modelMesh = MeshBuilder::GetInstance()->GetMesh("PLAYER_ASSAULT");
//
//	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
//	modelStack.PushMatrix();
//	if (playerInfo->GetWeapon() == 0)
//	{
//		if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() < 1.5f)
//			modelStack.Translate(windowWidth * 0.4f, -windowHeight * 0.4f, 0.f);
//		else
//			modelStack.Translate(windowWidth * 0.4f, -windowHeight * 0.35f, 0.f);
//
//		if (!weaponManager[playerInfo->GetWeapon()]->GetCanFire())
//		{
//			CSoundEngine::GetInstance()->PlayASound("PISTOL");
//			modelStack.Rotate(-20.f, 0.f, 0.f, 1.f);
//		}
//		modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.3f, Application::GetInstance().GetWindowWidth() * 0.2f, 1.f);
//	}
//	if (playerInfo->GetWeapon() == 1)
//	{
//		if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() < 1.5f)
//			modelStack.Translate(windowWidth * 0.3f, -windowHeight * 0.4f, 0.f);
//		else
//			modelStack.Translate(windowWidth * 0.3f, -windowHeight * 0.35f, 0.f);
//
//		if (!weaponManager[playerInfo->GetWeapon()]->GetCanFire())
//		{
//			CSoundEngine::GetInstance()->PlayASound("ASSAULT");
//			modelStack.Rotate(-10.f, 0.f, 0.f, 1.f);
//		}
//		modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.3f, Application::GetInstance().GetWindowWidth() * 0.2f, 1.f);
//	}
//	RenderHelper::RenderMesh(modelMesh);
//	modelStack.PopMatrix();
//}

//void Selection::renderWeaponUI(void)
//{
//	Mesh* modelMesh;
//	if (playerInfo->GetWeapon() == 0)
//		modelMesh = MeshBuilder::GetInstance()->GetMesh("PISTOL");
//	if (playerInfo->GetWeapon() == 1)
//		modelMesh = MeshBuilder::GetInstance()->GetMesh("ASSAULT");
//
//	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
//	modelStack.PushMatrix();
//	if (playerInfo->GetWeapon() == 0)
//	{
//		if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() < 1.5f)
//			modelStack.Translate(-windowWidth * 0.425f, -windowHeight * 0.225f, 0.f);
//		else
//			modelStack.Translate(-windowWidth * 0.425f, -windowHeight * 0.1525f, 0.f);
//
//		if (!weaponManager[playerInfo->GetWeapon()]->GetCanFire())
//			modelStack.Rotate(-10.f, 0.f, 0.f, 1.f);
//		modelStack.Scale(windowWidth * 0.05f, windowWidth * 0.05f, 1.f);
//	}
//	if (playerInfo->GetWeapon() == 1)
//	{
//		if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() < 1.5f)
//			modelStack.Translate(-windowWidth * 0.425f, -windowHeight * 0.225f, 0.f);
//		else
//			modelStack.Translate(-windowWidth * 0.425f, -windowHeight * 0.1525f, 0.f);
//
//		if (!weaponManager[playerInfo->GetWeapon()]->GetCanFire())
//			modelStack.Rotate(-10.f, 0.f, 0.f, 1.f);
//		modelStack.Scale(windowWidth * 0.1f, windowWidth * 0.04f, 1.f);
//	}
//	RenderHelper::RenderMesh(modelMesh);
//	modelStack.PopMatrix();
//}

//void Selection::renderHit(void)
//{
//
//	Mesh* modelMesh;
//	modelMesh = MeshBuilder::GetInstance()->GetMesh("HIT");
//	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
//	modelStack.PushMatrix();
//	modelStack.Translate(0.f, 150.f, 0.f);
//	modelStack.Rotate(0.f, 0.f, 0.f, 1.f);
//	modelStack.Scale(200.f, 200.f, 1.f);
//	RenderHelper::RenderMesh(modelMesh);
//	modelStack.PopMatrix();
//}

void Selection::renderSelection(void)
{
	Mesh* modelMesh;
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() < 1.5f)
	{
		modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionMenu");
		modelStack.PushMatrix();
		//modelStack.Scale(500, 500, 0);
		modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.99f, Application::GetInstance().GetWindowHeight() * 0.99f, 0);
		RenderHelper::RenderMesh(modelMesh);
		modelStack.PopMatrix();
	}
	else
	{
		modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionMenu");
		modelStack.PushMatrix();
		//modelStack.Scale(500, 500, 0);
		modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.99f, Application::GetInstance().GetWindowHeight() * 0.99f, 0);
		RenderHelper::RenderMesh(modelMesh);
		modelStack.PopMatrix();
	}
}

void Selection::renderSelectionTitle(void)
{
	Mesh* modelMesh;
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() < 1.5f)
	{
		modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionTitle");
		modelStack.PushMatrix();
		modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * 0.35, 0);
		modelStack.Scale(Application::GetInstance().GetWindowWidth() * .6, Application::GetInstance().GetWindowHeight() * .09, 1.f);
		RenderHelper::RenderMesh(modelMesh);
		modelStack.PopMatrix();
	}
	else
	{
		modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionTitle");
		modelStack.PushMatrix();
		modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * 0.35, 0);
		modelStack.Scale(Application::GetInstance().GetWindowWidth() * .6, Application::GetInstance().GetWindowHeight() * .09, 1.f);
		RenderHelper::RenderMesh(modelMesh);
		modelStack.PopMatrix();
	}
}

void Selection::renderSelectionVillage(void)
{
	Mesh* modelMesh;
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() < 1.5f)
	{
		if (selectedChoice == 1)
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionVillageTown");
			modelStack.PushMatrix();
			modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * 0.2, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .4, Application::GetInstance().GetWindowHeight() * .1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
		else
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionVillageTown");
			modelStack.PushMatrix();
			modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * 0.2, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .25, Application::GetInstance().GetWindowHeight() * .1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
	}
	else if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() > 1.5f)
	{
		if (selectedChoice == 1)
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionVillageTown");
			modelStack.PushMatrix();
			modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * 0.2, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .4, Application::GetInstance().GetWindowHeight() * .1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
		else
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionVillageTown");
			modelStack.PushMatrix();
			modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * 0.2, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .25, Application::GetInstance().GetWindowHeight() * .1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
	}
}

void Selection::renderSelectionTutorial(void)
{
	Mesh* modelMesh;
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() < 1.5f)
	{
		if (selectedChoice == 2)
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionTutorial");
			modelStack.PushMatrix();
			modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * 0.08, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .4, Application::GetInstance().GetWindowHeight() * .1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
		else
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionTutorial");
			modelStack.PushMatrix();
			modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * 0.08, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .25, Application::GetInstance().GetWindowHeight() * .1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
	}
	else if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() > 1.5f)
	{
		if (selectedChoice == 2)
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionTutorial");
			modelStack.PushMatrix();
			modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * 0.08, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .4, Application::GetInstance().GetWindowHeight() * .1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
		else
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionTutorial");
			modelStack.PushMatrix();
			modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * 0.08, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .25, Application::GetInstance().GetWindowHeight() * .1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
	}
}

void Selection::renderSelectionLevel1(void)
{
	Mesh* modelMesh;
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() < 1.5f)
	{
		if (selectedChoice == 3)
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionLevel1");
			modelStack.PushMatrix();
			modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * -0.04, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .4, Application::GetInstance().GetWindowHeight() * .1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
		else
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionLevel1");
			modelStack.PushMatrix();
			modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * -0.04 , 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .25, Application::GetInstance().GetWindowHeight() * .1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
	}
	else if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() > 1.5f)
	{
		if (selectedChoice == 3)
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionLevel1");
			modelStack.PushMatrix();
			modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * -0.04, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .4, Application::GetInstance().GetWindowHeight() * .1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
		else
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionLevel1");
			modelStack.PushMatrix();
			modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * -0.04, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .25, Application::GetInstance().GetWindowHeight() * .1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
	}
}

void Selection::renderSelectionLevel2(void)
{
	Mesh* modelMesh;
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() < 1.5f)
	{
		if (selectedChoice == 4)
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionLevel2");
			modelStack.PushMatrix();
			modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * -0.16, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .4, Application::GetInstance().GetWindowHeight() * .1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
		else
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionLevel2");
			modelStack.PushMatrix();
			modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * -0.16, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .25, Application::GetInstance().GetWindowHeight() * .1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
	}
	else if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() > 1.5f)
	{
		if (selectedChoice == 4)
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionLevel2");
			modelStack.PushMatrix();
			modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * -0.16, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .4, Application::GetInstance().GetWindowHeight() * .1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
		else
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionLevel2");
			modelStack.PushMatrix();
			modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * -0.16, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .25, Application::GetInstance().GetWindowHeight() * .1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
	}
}

void Selection::renderSelectionLevel3(void)
{
	Mesh* modelMesh;
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() < 1.5f)
	{
		if (selectedChoice == 5)
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionLevel3");
			modelStack.PushMatrix();
			modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * -0.28, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .4, Application::GetInstance().GetWindowHeight() * .1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
		else
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionLevel3");
			modelStack.PushMatrix();
			modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * -0.28, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .25, Application::GetInstance().GetWindowHeight() * .1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
	}
	else if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() > 1.5f)
	{
		if (selectedChoice == 5)
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionLevel3");
			modelStack.PushMatrix();
			modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * -0.28, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .4, Application::GetInstance().GetWindowHeight() * .1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
		else
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionLevel3");
			modelStack.PushMatrix();
			modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * -0.28, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .25, Application::GetInstance().GetWindowHeight() * .1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
	}
}

void Selection::renderSelectionLevel4(void)
{
	Mesh* modelMesh;
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() < 1.5f)
	{
		if (selectedChoice == 6)
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionLevel4");
			modelStack.PushMatrix();
			modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * -0.4, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .4, Application::GetInstance().GetWindowHeight() * .1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
		else
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionLevel4");
			modelStack.PushMatrix();
			modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * -0.4, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .25, Application::GetInstance().GetWindowHeight() * .1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
	}
	else if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() > 1.5f)
	{
		if (selectedChoice == 6)
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionLevel4");
			modelStack.PushMatrix();
			modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * -0.4, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .4, Application::GetInstance().GetWindowHeight() * .1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
		else
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionLevel4");
			modelStack.PushMatrix();
			modelStack.Translate(0, Application::GetInstance().GetWindowHeight() * -0.4, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .25, Application::GetInstance().GetWindowHeight() * .1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
	}
}

void Selection::renderSelectionBack(void)
{
	Mesh* modelMesh;
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() < 1.5f)
	{
		if (selectedChoice == 7)
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionBack");
			modelStack.PushMatrix();
			modelStack.Translate(Application::GetInstance().GetWindowWidth() * 0.4, Application::GetInstance().GetWindowHeight() * -0.4, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .1, Application::GetInstance().GetWindowHeight() * 0.1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
		else
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionBack");
			modelStack.PushMatrix();
			modelStack.Translate(Application::GetInstance().GetWindowWidth()* 0.4, Application::GetInstance().GetWindowHeight() * -0.4, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .1, Application::GetInstance().GetWindowHeight() * 0.05, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
	}
	else if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() > 1.5f)
	{
		if (selectedChoice == 7)
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionBack");
			modelStack.PushMatrix();
			modelStack.Translate(Application::GetInstance().GetWindowWidth()* 0.4, Application::GetInstance().GetWindowHeight() * -0.4, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .1, Application::GetInstance().GetWindowHeight() * 0.1, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
		else
		{
			modelMesh = MeshBuilder::GetInstance()->GetMesh("SelectionBack");
			modelStack.PushMatrix();
			modelStack.Translate(Application::GetInstance().GetWindowWidth()* 0.4, Application::GetInstance().GetWindowHeight() * -0.4, 0);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * .1, Application::GetInstance().GetWindowHeight() * 0.05, 1.f);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
	}
}

void Selection::pauseOptions(double dt, bool &pause)
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

	

}

void Selection::clearKeyDisplay(void)
{
	for (int i = 0; i < 17; ++i)
	{
		controlText[i]->SetText("");
	}
}

void Selection::RenderPassGPass(void)
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


	//RenderWorld();
}

void Selection::RenderPassMain(void)
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
	SceneManager::GetInstance()->currProg->UpdateInt("shadowMap", 8);
	//GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);

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
		SceneManager::GetInstance()->currProg->UpdateVector3("lights[0].position_cameraspace", &lightDirection_cameraspace.x);
	}
	else if (lights[0]->type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = GraphicsManager::GetInstance()->GetViewMatrix() * lights[0]->position;
		SceneManager::GetInstance()->currProg->UpdateVector3("lights[0].position_cameraspace", &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = GraphicsManager::GetInstance()->GetViewMatrix() * lights[0]->spotDirection;
		SceneManager::GetInstance()->currProg->UpdateVector3("lights[0].spotDirection", &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = GraphicsManager::GetInstance()->GetViewMatrix() * lights[0]->position;
		SceneManager::GetInstance()->currProg->UpdateVector3("lights[0].position_cameraspace", &lightPosition_cameraspace.x);
	}

	/*if (lights[1]->type == Light::LIGHT_DIRECTIONAL)
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
	}*/


	//RenderMesh(meshList[GEO_AXES], false);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


	GraphicsManager::GetInstance()->SetPerspectiveProjection(70.f, (float)Application::GetInstance().m_window_width / (float)Application::GetInstance().m_window_height, 0.1f, 10000.0f);
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
	//RenderWorld();
	//currProg->UpdateInt("fogParam.enabled", 0);

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;
	GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();
	if (OptionsManager::GetInstance()->getEditingState())
	{
		Mesh* modelMesh;
		modelMesh = MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 1.0f), 1.0f);
		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		modelStack.PushMatrix();
		modelStack.Translate(0.f, 0.f, 0.f);
		if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() < 1.5f)
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.55f, Application::GetInstance().GetWindowWidth() * 0.55f, 1.f);
		else
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.55f, Application::GetInstance().GetWindowWidth() * 0.4f, 1.f);
		RenderHelper::RenderMesh(modelMesh);
		modelStack.PopMatrix();
	}
	//EntityManager::GetInstance()->RenderUI();

	/*Render Weapon*/
	//renderWeapon();

	//*Render Camera Effects*/
	//theCameraEffects->RenderUI();

	//*Render Weapon UI*/
	//renderWeaponUI();
	
	renderSelection();
	renderSelectionTitle();
	renderSelectionVillage();
	renderSelectionTutorial();
	renderSelectionLevel1();
	renderSelectionLevel2();
	renderSelectionLevel3();
	renderSelectionLevel4();
	renderSelectionBack();


	//*Render Hit*/
	//if (EntityManager::GetInstance()->getHit())
	//renderHit();

	//if (openInventory)
	//{
	//Inventory::GetInstance()->RenderWeapon();
	//}

	//if (openEQ)
	//{
	//EquipmentManager::GetInstance()->Render();
	//CPlayerInfo::GetInstance()->RenderAttribute();

	//}

	//*Render KO Count*/
	//RenderHelper::GetInstance()->renderKOCount();
	//*Render Player Health Bar*/
	//RenderHelper::GetInstance()->renderPlayerHealth();

	/*Render Map Editor Options.*/
	if (Map_Editor::GetInstance()->mapEditing)
		Map_Editor::GetInstance()->renderOption();

	/*Render text display.*/
	if (Text_Manager::GetInstance()->returnTextList().size() > 0)
		Text_Manager::GetInstance()->renderText();

	glEnable(GL_DEPTH_TEST);

	/*Render Minimap*/
	//theMinimap->RenderUI();

	/*Render on Screen*/
	glDisable(GL_DEPTH_TEST);

	/*3D Render*/
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

//void Selection::RenderWorld(void)
//{
//	/*Debug*/
//	//CPlayerInfo::GetInstance()->setHealth(CPlayerInfo::GetInstance()->getHealth() - 5);
//
//	if (playerInfo->GetAttribute(CAttributes::TYPE_HEALTH) <= 0)
//	{
//		Mesh* modelMesh;
//		modelMesh = MeshBuilder::GetInstance()->GetMesh("GAMEOVER");
//		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
//		modelStack.PushMatrix();
//		modelStack.Translate(0.f, 0.f, 0.f);
//		modelStack.Rotate(0.f, 0.f, 0.f, 1.f);
//		modelStack.Scale(400.f, 300.f, 1.f);
//		RenderHelper::RenderMesh(modelMesh);
//		modelStack.PopMatrix();
//	}
//	// Setup 3D pipeline then render 3D
//	//GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
//	GraphicsManager::GetInstance()->AttachCamera(&camera);
//
//	EntityManager::GetInstance()->Render();
//
//
//	if (Map_Editor::GetInstance()->mapEditing)
//		Map_Editor::GetInstance()->renderObject();
//
//	ParticleManager::GetInstance()->renderAllParticle();
//
//
//	/*Debug Quad for Shadow*/
//	/*MS& ms = GraphicsManager::GetInstance()->GetModelStack();
//	ms.PushMatrix();
//	ms.Translate(0, 10, 10);
//	ms.Scale(5, 5, 5);
//	RenderHelper::RenderMeshWithLight(TEMPSPHERE);
//	ms.PopMatrix();
//
//	ms.PushMatrix();
//	ms.Translate(0, -8, 0);
//	ms.Rotate(-90, 1, 0, 0);
//	ms.Scale(1000, 1000, 1000);
//	RenderHelper::RenderMeshWithLight(TEMPQUAD);
//	ms.PopMatrix();*/
//	/*--------------------------------------------------------*/
//}

void Selection::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
	playerInfo->DetachCamera();

	//	if (playerInfo->DropInstance() == false)
	//	{
	//#if _DEBUGMODE==1
	//		cout << "Unable to drop PlayerInfo class" << endl;
	//#endif
	//	}

	// Delete the lights
	GraphicsManager::GetInstance()->RemoveLight("lights[0]");
	GraphicsManager::GetInstance()->RemoveLight("lights[1]");


	//if (currProg)
	//{
	//	delete currProg;
	//	currProg = nullptr;
	//}

	//if (m_gPassShaderID)
	//{
	//	delete m_gPassShaderID;
	//	m_gPassShaderID = nullptr;
	//}

	ParticleManager* particleManager = ParticleManager::GetInstance();
	ParticleManager::GetInstance()->deleteParticle();

	EntityManager::GetInstance()->removeAllEntities();

	while (controls.size() > 0)
	{
		Controls* object = controls.back();
		delete object;
		object = nullptr;
		controls.pop_back();
	}

	MeshBuilder::GetInstance()->removeMeshMap();
	GraphicsManager::GetInstance()->removeLightMap();

	if (cinematic)
	{
		delete cinematic;
		cinematic = nullptr;
	}
	Text_Manager::GetInstance()->resetAll();
	CPlayerInfo::GetInstance()->setKO_Count(0.f);
	CSoundEngine::GetInstance()->GetSoundEngine()->stopAllSounds();
}
