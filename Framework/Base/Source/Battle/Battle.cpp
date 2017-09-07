#include "Battle.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "KeyboardController.h"
#include "../Application.h"
#include "../SoundEngine.h"
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"
#include "Battle_Option.h"
#include "SceneManager.h"
#include "../Text_Display/Text/Text.h"
#include "../Enemy/Enemy_Manager.h"

/*Debugging purpose.*/
#include <iostream>

using std::cout;
using std::endl;

CBattle::CBattle() :
	battleState(false)
	, checkTurn(false)
	, turn(NO_ONE_TURN)
	, shake(0.f)
	, shakeAll(0.f)
	, fightState(NO_ATTACK)
	, adjustX(0.f)
	, adjustY(0.f)
	, scaleX(0.f)
	, scaleY(0.f)
	, critical(false)
{
	Init();
}

CBattle::~CBattle()
{
}

void CBattle::Init(void)
{
	battleState = false;
	checkTurn = false;
	turn = NO_ONE_TURN;
	shake = 0.f;
	shakeAll = 0.f;
	fightState = NO_ATTACK;
	option = ECBattle_Option::ATTACK_OPTION;
	adjustX = 0.f;
	adjustY = 0.f;
	scaleX = 0.f;
	scaleY = 0.f;
	critical = false;

	while (battleList.size() > 0)
	{
		CEnemy* object = battleList.back();
		delete object;
		object = nullptr;
		battleList.pop_back();
	}

	while (playerList.size() > 0)
	{
		CPlayer* object = playerList.back();
		delete object;
		object = nullptr;
		playerList.pop_back();
	}
}

void CBattle::SetBattleState(bool _battleState)
{
	battleState = _battleState;
}

bool CBattle::GetBattleState(void)
{
	return battleState;
}

void CBattle::AddPlayer(CPlayer * _player)
{
	playerList.push_back(_player);
}

void CBattle::AddEnemy(CEnemy * _enemy)
{
	battleList.push_back(_enemy);
}

void CBattle::CheckTurnToGoFirst(double dt)
{
	int totalEnemySpeed = 0;
	int totalPlayerSpeed = 0;

	for (vector<CEnemy*>::iterator it = battleList.begin(); it != battleList.end(); ++it)
	{
		CEnemy* enemy = (CEnemy*)*it;
		totalEnemySpeed += enemy->GetAttribute(CAttributes::ATTRIBUTE_TYPES::TYPE_SPEED);
	}
	for (vector<CPlayer*>::iterator it = playerList.begin(); it != playerList.end(); ++it)
	{
		CPlayer* player = (CPlayer*)*it;
		totalPlayerSpeed += player->GetAttribute(CAttributes::ATTRIBUTE_TYPES::TYPE_SPEED);
	}

	if (totalEnemySpeed > totalPlayerSpeed)
		turn = ENEMY_TURN;
	else
		turn = PLAYER_TURN;
}

void CBattle::PlayerAttack(double dt)
{
	static int damage = 0;
	static bool playAttack = false;
	static bool playDamaged = false;
	static float waitForSound = 0.f;
	static int shakeDirection = 0;

	if (shake > 1.f)
		shakeDirection = 1;
	else if (shake < -1.f)
		shakeDirection = 0;

	if (!playAttack)
	{
		CSoundEngine::GetInstance()->GetSoundEngine()->play2D("Sound\\SFX\\BATTLE\\PLAYER_ATTACK.ogg", false);
		playAttack = true;
	}

	waitForSound += static_cast<float>(dt);

	/*0.209f is the duration of PLAYER_ATTACK.ogg.*/
	if (waitForSound > 0.5f && playAttack && !playDamaged)
	{
		srand(static_cast<unsigned>(time((NULL))));
		int criticalChance = rand() % 2; /*Randomise from 0 to 1*/
										 /*
										 int criticalChance = rand() % 1 + 1;
										 This does not work because it will force it to 1.
										 It means randonmise a value between 0 and 0 but start at 1 which is always just 1.
										 In simple words, it means that criticalChance is always randomised as 0 and it will just + 1 to it afterwards which gives 1.
										 */

		if (battleList.size() == 1)
		{
			if (criticalChance == 0)
			{
				CSoundEngine::GetInstance()->GetSoundEngine()->play2D("Sound\\SFX\\BATTLE\\DAMAGED.ogg", false);
				battleList.back()->deductHealthBy(playerList.back()->GetAttribute(CAttributes::ATTRIBUTE_TYPES::TYPE_ATTACK));
				damage = playerList.back()->GetAttribute(CAttributes::ATTRIBUTE_TYPES::TYPE_ATTACK);
			}
			else
			{
				CSoundEngine::GetInstance()->GetSoundEngine()->play2D("Sound\\SFX\\BATTLE\\CRITICAL.ogg", false);
				battleList.back()->deductHealthBy(playerList.back()->GetAttribute(CAttributes::ATTRIBUTE_TYPES::TYPE_ATTACK) * 3);
				damage = playerList.back()->GetAttribute(CAttributes::ATTRIBUTE_TYPES::TYPE_ATTACK) * 3;
				critical = true;
			}
		}
		waitForSound = 0.f;
		playDamaged = true;
	}
	else if (waitForSound < 0.5f && playDamaged)
	{
		if (shakeDirection == 0)
			shake += static_cast<float>(dt) * 100.f;
		else if (shakeDirection == 1)
			shake -= static_cast<float>(dt) * 100.f;
	}
	/*0.220f is the duration of DAMAGED.ogg.*/
	else if (waitForSound > 0.5f && playDamaged)
	{
		string message = "";
		if (critical)
			message = "Player CRITICALLY DAMAGED ";
		else
			message = "Player damaged ";
		critical = false;
		message += std::to_string(damage);
		message += " to ";
		message += battleList.back()->GetName();
		message += ".";
		message += " DOES THIS EVEN WORK FOR THE ENTER KEY OH GREAT IT SEEMS TO WORK FOR 6 LINES OF TEXT ON SCREEN IS IT REAL";
		Create::Text("text", message, 0.f, 1.f, CText::TEXT_BATTLE);

		fightState = ENEMY_ATTACK;
		playAttack = false;
		playDamaged = false;
		waitForSound = 0.f;
		shakeDirection = 0;
		shake = 0.f;
		damage = 0;
	}
}

void CBattle::EnemyAttack(double dt)
{
	static int damage = 0;
	static bool playAttack = false;
	static bool playDamaged = false;
	static float waitForSound = 0.f;
	static int shakeDirection = 0;

	if (shakeAll > 1.f)
		shakeDirection = 1;
	else if (shakeAll < -1.f)
		shakeDirection = 0;

	if (battleList.back()->GetAttribute(CAttributes::ATTRIBUTE_TYPES::TYPE_HEALTH) > 0)
	{

		if (!playAttack)
		{
			CSoundEngine::GetInstance()->GetSoundEngine()->play2D("Sound\\SFX\\BATTLE\\ENEMY_ATTACK.ogg", false);
			playAttack = true;
		}

		waitForSound += static_cast<float>(dt);

		/*0.228f is the duration of ENEMY_ATTACK.ogg.*/
		if (waitForSound > 0.5f && playAttack && !playDamaged)
		{
			CSoundEngine::GetInstance()->GetSoundEngine()->play2D("Sound\\SFX\\BATTLE\\DAMAGED.ogg", false);
			playerList.back()->deductHealthBy(battleList.back()->GetAttribute(CAttributes::ATTRIBUTE_TYPES::TYPE_ATTACK));
			damage = battleList.back()->GetAttribute(CAttributes::ATTRIBUTE_TYPES::TYPE_ATTACK);

			waitForSound = 0.f;
			playDamaged = true;
		}
		else if (waitForSound < 0.5f && playDamaged)
		{
			if (shakeDirection == 0)
				shakeAll += static_cast<float>(dt) * 100.f;
			//GetCamera()->SetCameraPos(Vector3(GetCamera()->GetCameraPos().x + static_cast<float>(dt) * 100.f, /*X-Axis.*/
			//	GetCamera()->GetCameraPos().y, /*Y-Axis.*/
			//	GetCamera()->GetCameraPos().z)); /*Z-Axis.*/
			else if (shakeDirection == 1)
				shakeAll -= static_cast<float>(dt) * 100.f;
			//camera->MoveCamera(static_cast<float>(dt) * 100.f, FPSCamera::MOVE_RIGHT);
			//GetCamera()->SetCameraPos(Vector3(GetCamera()->GetCameraPos().x - static_cast<float>(dt) * 100.f, /*X-Axis.*/
			//	GetCamera()->GetCameraPos().y, /*Y-Axis.*/
			//	GetCamera()->GetCameraPos().z)); /*Z-Axis.*/

		}
		/*0.220f is the duration of DAMAGED.ogg.*/
		else if (waitForSound > 0.5f && playDamaged)
		{
			string message = battleList.back()->GetName();
			message += " damaged ";
			message += std::to_string(damage);
			message += " to ";
			message += "Player";
			message += ".";
			Create::Text("text", message, 0.f, 1.f, CText::TEXT_BATTLE);

			turn = NO_ONE_TURN;
			fightState = NO_ATTACK;
			playAttack = false;
			playDamaged = false;
			waitForSound = 0.f;
			checkTurn = false;
			shakeAll = 0.f;
			damage = 0;
			//GetCamera()->SetCameraPos(Vector3(0.f, /*X-Axis.*/
			//	GetCamera()->GetCameraPos().y, /*Y-Axis.*/
			//	GetCamera()->GetCameraPos().z)); /*Z-Axis.*/
		}
	}
	else
	{
		static bool playVictory = false;
		static float waitForSound = 0.f;
		if (!playVictory)
		{
			CSoundEngine::GetInstance()->GetSoundEngine()->play2D("Sound\\SFX\\BATTLE\\VICTORY.ogg", false);
			playVictory = true;
		}
		waitForSound += static_cast<float>(dt);
		/*1.727f is the duration of DAMAGED.ogg.*/
		if (waitForSound > 1.727f)
		{
			turn = NO_ONE_TURN;
			fightState = NO_ATTACK;
			playAttack = false;
			playDamaged = false;
			waitForSound = 0.f;
			checkTurn = false;
			shakeAll = 0.f;

			playerList.back()->GetBattle()->SetBattleState(false);
			Exit();
			battleState = false;
			playVictory = false;
		}
	}
}

void CBattle::Render()
{
	if (battleState)
	{
		const float healthBar = Application::GetInstance().GetWindowWidth() * 0.1f;
		/*Background*/
		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		Mesh* _mesh = MeshBuilder::GetInstance()->GetMesh("B_GRASS");
		modelStack.PushMatrix();
		modelStack.Translate(shakeAll, 0.f, 0.f);
		modelStack.Scale(Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight(), 0.f);
		RenderHelper::GetInstance()->RenderMesh(_mesh);
		modelStack.PopMatrix();
		
		/*Enemy Section*/
		for (vector<CEnemy*>::iterator it = battleList.begin(); it != battleList.end(); ++it)
		{
			CEnemy* enemy = (CEnemy*)*it;
			
			/*Enemy*/
			modelStack.PushMatrix();
			modelStack.Translate(shake, 0.f, 0.f);
			modelStack.Translate(-shakeAll, 0.f, 0.f);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.1f, Application::GetInstance().GetWindowWidth() * 0.1f, 0.f);
			RenderHelper::GetInstance()->RenderMesh(enemy->GetModelMesh());
			modelStack.PopMatrix();

			/*Render Health Bar*/
			float offsetHealth = healthBar -
				(static_cast<float>(enemy->GetAttribute(CAttributes::ATTRIBUTE_TYPES::TYPE_HEALTH)) /
					static_cast<float>(enemy->GetAttribute(CAttributes::ATTRIBUTE_TYPES::TYPE_MAXHEALTH)) * healthBar);
			_mesh = MeshBuilder::GetInstance()->GetMesh("ENEMY");
			modelStack.PushMatrix();
			modelStack.Translate(-offsetHealth * 0.5f, Application::GetInstance().GetWindowWidth() * 0.11f, 0.f);
			modelStack.Translate(-shakeAll, 0.f, 0.f);
			modelStack.Scale(static_cast<float>(enemy->GetAttribute(CAttributes::ATTRIBUTE_TYPES::TYPE_HEALTH)) / /*X-Axis*/
				static_cast<float>(enemy->GetAttribute(CAttributes::ATTRIBUTE_TYPES::TYPE_MAXHEALTH)) * healthBar, /*X-Axis*/
				Application::GetInstance().GetWindowWidth() * 0.01f, /*Y-Axis*/
				0.f); /*Z-Axis*/
			RenderHelper::GetInstance()->RenderMesh(_mesh);
			modelStack.PopMatrix();
		}

		/*Option Section*/
		_mesh = MeshBuilder::GetInstance()->GetMesh("BLACK");
		modelStack.PushMatrix();
		modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.5f, -Application::GetInstance().GetWindowHeight() * 0.5f, 0.f);
		modelStack.Translate(shakeAll, 0.f, 0.f);
		modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.5f, Application::GetInstance().GetWindowWidth() * 0.5f, 0.f);
		RenderHelper::GetInstance()->RenderMesh(_mesh);
		modelStack.PopMatrix();

		_mesh = MeshBuilder::GetInstance()->GetMesh("text");
		modelStack.PushMatrix();
		modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.42f, -Application::GetInstance().GetWindowHeight() * 0.2025f, 0.f);
		modelStack.Translate(shakeAll, 0.f, 0.f);
		modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.05f, Application::GetInstance().GetWindowWidth() * 0.05f, 0.f);
		RenderHelper::GetInstance()->RenderText(_mesh, "ATTACK", Color(1.f, 1.f, 0.f));
		modelStack.PopMatrix();

		_mesh = MeshBuilder::GetInstance()->GetMesh("text");
		modelStack.PushMatrix();
		modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.42f, -Application::GetInstance().GetWindowHeight() * 0.3075f, 0.f);
		modelStack.Translate(shakeAll, 0.f, 0.f);
		modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.05f, Application::GetInstance().GetWindowWidth() * 0.05f, 0.f);
		RenderHelper::GetInstance()->RenderText(_mesh, "DEFEND", Color(1.f, 1.f, 0.f));
		modelStack.PopMatrix();

		_mesh = MeshBuilder::GetInstance()->GetMesh("text");
		modelStack.PushMatrix();
		modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.42f, -Application::GetInstance().GetWindowHeight() * 0.405f, 0.f);
		modelStack.Translate(shakeAll, 0.f, 0.f);
		modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.05f, Application::GetInstance().GetWindowWidth() * 0.05f, 0.f);
		RenderHelper::GetInstance()->RenderText(_mesh, "RUN", Color(1.f, 1.f, 0.f));
		modelStack.PopMatrix();

		if (option == ECBattle_Option::ATTACK_OPTION)
		{
			_mesh = MeshBuilder::GetInstance()->GetMesh("PATH");
			modelStack.PushMatrix();
			modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.2f, -Application::GetInstance().GetWindowHeight() * 0.2f, 0.f);
			modelStack.Translate(shakeAll, 0.f, 0.f);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.05f, Application::GetInstance().GetWindowWidth() * 0.05f, 0.f);
			RenderHelper::GetInstance()->RenderMesh(_mesh);
			modelStack.PopMatrix();
		}
		else if (option == ECBattle_Option::DEFEND_OPTION)
		{
			_mesh = MeshBuilder::GetInstance()->GetMesh("PATH");
			modelStack.PushMatrix();
			modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.2f, -Application::GetInstance().GetWindowHeight() * 0.3f, 0.f);
			modelStack.Translate(shakeAll, 0.f, 0.f);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.05f, Application::GetInstance().GetWindowWidth() * 0.05f, 0.f);
			RenderHelper::GetInstance()->RenderMesh(_mesh);
			modelStack.PopMatrix();
		}
		else if (option == ECBattle_Option::RUN_OPTION)
		{
			_mesh = MeshBuilder::GetInstance()->GetMesh("PATH");
			modelStack.PushMatrix();
			modelStack.Translate(-Application::GetInstance().GetWindowWidth() * 0.2f, -Application::GetInstance().GetWindowHeight() * 0.4f, 0.f);
			modelStack.Translate(shakeAll, 0.f, 0.f);
			modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.05f, Application::GetInstance().GetWindowWidth() * 0.05f, 0.f);
			RenderHelper::GetInstance()->RenderMesh(_mesh);
			modelStack.PopMatrix();
		}
	}
}

void CBattle::Update(double dt)
{
	if (battleState)
	{
		static int damage = 0;
		if (fightState == NO_ATTACK)
		{
			if (!checkTurn)
			{
				CheckTurnToGoFirst(dt);
				checkTurn = true;
			}

			else if (checkTurn)
			{
				if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
					++option;

				if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
					--option;

				if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
				{
					if (option == ECBattle_Option::ATTACK_OPTION)
						fightState = PLAYER_ATTACK;
					else if (option == ECBattle_Option::DEFEND_OPTION)
						fightState = PLAYER_DEFEND;
					else if (option == ECBattle_Option::RUN_OPTION)
						fightState = PLAYER_RUN;
				}
			}
		}
		else if (turn == ENEMY_TURN)
		{
			if (fightState == ENEMY_ATTACK)
				EnemyAttack(dt);
			else if (fightState == PLAYER_ATTACK)
				PlayerAttack(dt);
		}
		else if (turn == PLAYER_TURN)
		{
			if (fightState == PLAYER_ATTACK)
				PlayerAttack(dt);
			else if (fightState == ENEMY_ATTACK)
				EnemyAttack(dt);
		}
	}
}

void CBattle::Exit(void)
{
	for (map<string, CEnemy*>::iterator thisEnemy = CEnemy_Manager::GetInstance()->GetEnemyList()->begin(); thisEnemy != CEnemy_Manager::GetInstance()->GetEnemyList()->end();)
	{
		bool deleted = false;
		for (vector<CEnemy*>::iterator thatEnemy = battleList.begin(); thatEnemy != battleList.end();)
		{
			if (thisEnemy->second != (*thatEnemy))
			{
				++thatEnemy;
				continue;
			}

			deleted = true;
			CEnemy* enemy = thisEnemy->second;
			delete enemy;
			enemy = nullptr;

			thatEnemy = battleList.erase(thatEnemy);
			thisEnemy = CEnemy_Manager::GetInstance()->GetEnemyList()->erase(thisEnemy);
		}
		if (!deleted)
			++thisEnemy;
	}
	while (battleList.size() > 0)
	{
		CEnemy* object = battleList.back();
		if (object->GetModelMesh())
		{
			Mesh* mesh = object->GetModelMesh();
			delete mesh;
			mesh = nullptr;
		}
		delete object;
		object = nullptr;
		battleList.pop_back();
	}

	while (playerList.size() > 0)
	{
		//CPlayer* object = playerList.back();
		//delete object;
		//object = nullptr;
		playerList.pop_back();
	}

	//battleList.back()->setHealthTo(10);
	//battleList.back()->setMaxHealthTo(10);
	//battleList.back()->setAttackTo(1);
	//battleList.back()->setSpeed(1);
	CSoundEngine::GetInstance()->GetSoundEngine()->stopAllSounds();
	if (SceneManager::GetInstance()->GetActiveScene() == "World")
		CSoundEngine::GetInstance()->GetSoundEngine()->play2D("Sound\\BGM\\INTENSE.ogg", true);
	
}

void Activate::BattleScene(CBattle* _battle, CEnemy * _enemy, CPlayer* _player)
{
	_battle->SetBattleState(true);
	CSoundEngine::GetInstance()->GetSoundEngine()->stopAllSounds();
	CSoundEngine::GetInstance()->GetSoundEngine()->play2D("Sound\\BGM\\BATTLE\\B_GRASS.ogg", true);
	_battle->AddEnemy(_enemy);
	_battle->AddPlayer(_player);
}
