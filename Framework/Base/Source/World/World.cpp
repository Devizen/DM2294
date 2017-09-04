#include "World.h"
#include "GL\glew.h"
//#include "GLFW\glfw3.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "../Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "../HardwareAbstraction/Keyboard.h"
#include "../HardwareAbstraction/Mouse.h"
#include "../FPSCamera.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "../Light.h"
#include "../DepthFBO.h"


/*Rendering*/
#include "RenderHelper.h"

/*Sound*/
#include "../SoundEngine.h"


/*Cinematic Camera*/
#include "../Cinematic\Cinematic.h"

/*DIsplaying Text*/
#include "../Text_Display\Text\Text.h"
#include "../Text_Display\Text_Manager\Text_Manager.h"

#include <iostream>

#include "../Attributes.h"

#include "../Player/Player.h"
#include "../Enemy/Enemy.h"
#include "../Enemy/Enemy_Manager.h"

/*Battle System*/
#include "../Battle/Battle.h"

/*Adjusting position of mesh.*/
#include "../Adjuster/Adjuster.h"

//#include "vld.h"
using namespace std;

World* World::sInstance = new World(SceneManager::GetInstance());

World::World()
	:  windowHeight(0.f)
	, windowWidth(0.f)
	, loopCredits(0)
{
}

World::World(SceneManager* _sceneMgr)
	: loopCredits(0)
{
	_sceneMgr->AddScene("World", this);
}

World::~World()
{
}

void World::Init()
{
	//Calculating aspect ratio
	windowHeight = Application::GetInstance().GetWindowHeight();
	windowWidth = Application::GetInstance().GetWindowWidth();

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

	ShaderProgram::GetInstance()->currProg->UpdateInt("numLights", 1);
	ShaderProgram::GetInstance()->currProg->UpdateInt("textEnabled", 0);

	Color fogColor(0.5f, 0.5f, 0.5f); //Vec3 Color
	ShaderProgram::GetInstance()->currProg->UpdateVector3("fogParam.color", &fogColor.r);
	ShaderProgram::GetInstance()->currProg->UpdateFloat("fogParam.start", 10);
	ShaderProgram::GetInstance()->currProg->UpdateFloat("fogParam.end", 2000);
	ShaderProgram::GetInstance()->currProg->UpdateFloat("fogParam.density", 0.005f);
	ShaderProgram::GetInstance()->currProg->UpdateInt("fogParam.type", 0);
	ShaderProgram::GetInstance()->currProg->UpdateInt("fogParam.enabled", 0);

	// Create and attach the camera to the scene
	camera = new FPSCamera();
	camera->Init(Vector3(0.f, 100.f, 0.f), Vector3(0.f ,0.f ,0.f), Vector3(0.f, 0.f, -1.f));
	cinematic = new CCinematic();
	cinematic->Init(Vector3(0.f, 100.f, 0.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f));

	//playerInfo->AttachCamera(&camera);
	GraphicsManager::GetInstance()->AttachCamera(camera);

	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = Application::GetInstance().GetWindowWidth() * 0.025f;
	float halfFontSize = fontSize / 2.0f;


	// Hardware Abstraction
	//theKeyboard = new CKeyboard();
	//theKeyboard->Create(playerInfo);

	// Activate the Blend Function
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//theMouse = new CMouse();
	//theMouse->Create(playerInfo);

	CSoundEngine::GetInstance()->GetSoundEngine()->play2D("Sound\\BGM\\INTENSE.ogg", true);

	loopCredits = 0;
	player = new CPlayer();
	player->setHealthTo(10);
	player->setMaxHealthTo(10);
	player->setAttackTo(1);
	player->setSpeed(2);

	enemy = Create::Enemy("ENEMY", Vector3(20.f, 0.f, 20.f));
	enemy->setHealthTo(10);
	enemy->setMaxHealthTo(10);
	enemy->setAttackTo(1);
	enemy->setSpeed(1);
}

void World::Update(double dt)
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

	/*Update text display.*/
	if (Text_Manager::GetInstance()->returnTextList().size() > 0)
		Text_Manager::GetInstance()->updateText(dt);

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD0))
		SceneManager::GetInstance()->SetActiveScene("Mainmenu");

	if (KeyboardController::GetInstance()->IsKeyPressed('B'))
	{
		enemy = Create::Enemy("ENEMY", Vector3(20.f, 0.f, 20.f));
		//enemy->setHealthTo(10);
		//enemy->setMaxHealthTo(10);
		//enemy->setAttackTo(1);
		//enemy->setSpeed(1);
		//Activate::BattleScene(player->GetBattle(), enemy, player);
	}

	//EntityManager::GetInstance()->Update(dt); // Update our entities

	//theKeyboard->Read(static_cast<float>(dt));
	//theMouse->Read(static_cast<float>(dt));

	if (!player->GetBattle()->GetBattleState())
	{
		/*Update camera.*/
		UpdateCameraMovement(dt);

		/*Update player.*/
		player->Update(dt);
		UpdatePlayerMovement(dt);

		/*Update enemy.*/
		CEnemy_Manager::GetInstance()->Update(dt);
	}

	/*Update battle system.*/
	if (!Text_Manager::GetInstance()->displayingText)
		player->GetBattle()->Update(dt);


	/*Update adjuster values for positioning.*/
	CAdjuster::GetInstance()->Update(dt);
}

void World::Render()
{
	RenderPassGPass();
	RenderPassMain();
}


void World::RenderPassGPass(void)
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

void World::RenderPassMain(void)
{
	DepthFBO::GetInstance()->m_renderPass = DepthFBO::RENDER_PASS_MAIN;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glViewport(0, 0, 800, 600);
	glViewport(0, 0, static_cast<GLsizei>(Application::GetInstance().GetWindowWidth()), static_cast<GLsizei>(Application::GetInstance().GetWindowHeight()));
	//glViewport(0, 0, m_worldWidth, m_worldHeight);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->SetActiveShader("default");
	//glUseProgram(m_programID);

	//pass light depth texture 
	DepthFBO::GetInstance()->BindForReading(GL_TEXTURE8);
	ShaderProgram::GetInstance()->currProg->UpdateInt("shadowMap", 8);
	//GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);

	// Camera matrix
	GraphicsManager::GetInstance()->GetViewMatrix().SetToIdentity();
	GraphicsManager::GetInstance()->GetViewMatrix().SetToLookAt(
		camera->GetCameraPos().x, camera->GetCameraPos().y, camera->GetCameraPos().z,
		camera->GetCameraTarget().x, camera->GetCameraTarget().y, camera->GetCameraTarget().z,
		camera->GetCameraUp().x, camera->GetCameraUp().y, camera->GetCameraUp().z);

	// Model matrix : an identity matrix (model will be at the origin)
	GraphicsManager::GetInstance()->GetModelStack().LoadIdentity();

	if (lights[0]->type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z);
		Vector3 lightDirection_cameraspace = GraphicsManager::GetInstance()->GetViewMatrix() * lightDir;
		ShaderProgram::GetInstance()->currProg->UpdateVector3("lights[0].position_cameraspace", &lightDirection_cameraspace.x);
	}
	else if (lights[0]->type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = GraphicsManager::GetInstance()->GetViewMatrix() * lights[0]->position;
		ShaderProgram::GetInstance()->currProg->UpdateVector3("lights[0].position_cameraspace", &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = GraphicsManager::GetInstance()->GetViewMatrix() * lights[0]->spotDirection;
		ShaderProgram::GetInstance()->currProg->UpdateVector3("lights[0].spotDirection", &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = GraphicsManager::GetInstance()->GetViewMatrix() * lights[0]->position;
		ShaderProgram::GetInstance()->currProg->UpdateVector3("lights[0].position_cameraspace", &lightPosition_cameraspace.x);
	}


	//RenderMesh(meshList[GEO_AXES], false);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


	GraphicsManager::GetInstance()->SetPerspectiveProjection(70.f, (float)Application::GetInstance().m_window_width / (float)Application::GetInstance().m_window_height, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(camera);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	RenderWorld();

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	int halfWindowWidth = static_cast<int>(Application::GetInstance().GetWindowWidth() / 2);
	int halfWindowHeight = static_cast<int>(Application::GetInstance().GetWindowHeight() / 2);
	GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();


	//Mesh* modelMesh;
	//MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	//modelMesh = MeshBuilder::GetInstance()->GetMesh("BLACK");
	//modelStack.PushMatrix();
	//modelStack.Scale(Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight(), 0);
	//RenderHelper::RenderMesh(modelMesh);
	//modelStack.PopMatrix();

	player->GetBattle()->Render();

	/*Render text display.*/
	if (Text_Manager::GetInstance()->returnTextList().size() > 0)
		Text_Manager::GetInstance()->renderText();

	glEnable(GL_DEPTH_TEST);

	/*Render on Screen*/
	glDisable(GL_DEPTH_TEST);

	/*3D Render*/
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void World::RenderWorld()
{
	Mesh* modelMesh;
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	modelMesh = MeshBuilder::GetInstance()->GetMesh("PLAYER");
	modelStack.PushMatrix();
	modelStack.Translate(player->GetPosition().x, player->GetPosition().y, player->GetPosition().z);
	modelStack.Scale(10.f, 10.f, 10.f);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();

	modelMesh = MeshBuilder::GetInstance()->GetMesh("BLACK");
	modelStack.PushMatrix();
	modelStack.Translate(0.f, 0.f, 0.f);
	modelStack.Scale(Application::GetInstance().GetWindowWidth(), 0.f, Application::GetInstance().GetWindowHeight());
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();

	//int grid = 0;

	//for (unsigned i = 0; i < 10; ++i)
	//{
	//	modelMesh = MeshBuilder::GetInstance()->GetMesh("cube");
	//	modelStack.PushMatrix();
	//	modelStack.Translate(0.f + grid, player->GetPosition().y, 0.f);
	//	modelStack.Scale(10.f, 10.f, 10.f);
	//	RenderHelper::RenderMesh(modelMesh);
	//	modelStack.PopMatrix();

	//	grid += 20;
	//}

	CEnemy_Manager::GetInstance()->Render();
}

void World::UpdatePlayerMovement(double dt)
{
	const float moveSpeed = 10.f;

	if (KeyboardController::GetInstance()->IsKeyDown(VK_UP))
		player->MovePlayer(static_cast<float>(dt) * moveSpeed, CPlayer::MOVE_UP);
	if (KeyboardController::GetInstance()->IsKeyDown(VK_DOWN))
		player->MovePlayer(static_cast<float>(dt) * moveSpeed, CPlayer::MOVE_DOWN);
	if (KeyboardController::GetInstance()->IsKeyDown(VK_LEFT))
		player->MovePlayer(static_cast<float>(dt) * moveSpeed, CPlayer::MOVE_LEFT);
	if (KeyboardController::GetInstance()->IsKeyDown(VK_RIGHT))
		player->MovePlayer(static_cast<float>(dt) * moveSpeed, CPlayer::MOVE_RIGHT);
}

void World::UpdateCameraMovement(double dt)
{
	const float moveSpeed = 10.f;
	if (KeyboardController::GetInstance()->IsKeyDown('W'))
		camera->MoveCamera(static_cast<float>(dt) * moveSpeed, FPSCamera::MOVE_UP);
	if (KeyboardController::GetInstance()->IsKeyDown('S'))
		camera->MoveCamera(static_cast<float>(dt) * moveSpeed, FPSCamera::MOVE_DOWN);
	if (KeyboardController::GetInstance()->IsKeyDown('A'))
		camera->MoveCamera(static_cast<float>(dt) * moveSpeed, FPSCamera::MOVE_LEFT);
	if (KeyboardController::GetInstance()->IsKeyDown('D'))
		camera->MoveCamera(static_cast<float>(dt) * moveSpeed, FPSCamera::MOVE_RIGHT);
	if (KeyboardController::GetInstance()->IsKeyDown('Q'))
		camera->MoveCamera(static_cast<float>(dt) * moveSpeed, FPSCamera::MOVE_BACKWARD);
	if (KeyboardController::GetInstance()->IsKeyDown('E'))
		camera->MoveCamera(static_cast<float>(dt) * moveSpeed, FPSCamera::MOVE_FORWARD);
}

void World::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();

	// Delete the lights
	GraphicsManager::GetInstance()->RemoveLight("lights[0]");
	GraphicsManager::GetInstance()->RemoveLight("lights[1]");

	MeshBuilder::GetInstance()->removeMeshMap();
	GraphicsManager::GetInstance()->removeLightMap();

	if (cinematic)
	{
		delete cinematic;
		cinematic = nullptr;
	}
	Text_Manager::GetInstance()->resetAll();
	CSoundEngine::GetInstance()->GetSoundEngine()->stopAllSounds();

	if (player)
	{
		delete player;
		player = nullptr;
	}
	if (camera)
	{
		delete camera;
		camera = nullptr;
	}
}
