/*Map Editor was done by Lai Wei Jing, Eugene (160540A)*/
#include "Map_Editor.h"
/*For keyboard inputs.*/
#include "KeyboardController.h"
/*For modelStack push and pop.*/
#include "GraphicsManager.h"
/*For getting mesh.*/
#include "MeshBuilder.h"
/*For rendering objects temporary.*/
#include "../../Common/Source/RenderHelper.h"
/*For player position.*/
#include "../PlayerInfo/PlayerInfo.h"
/*For maintaing aspect ratio.*/
#include "../Application.h"
/*To create Objects.*/
#include "../Object/Furniture.h"
/*To create Enemies.*/
#include "../Enemy/Patrol/Patrol.h"
/*To create Horde.*/
#include "../Enemy/Horde/Horde.h"
/*To create Tower.*/
#include "../Enemy/Tower/Tower.h"
/*To create animated robot*/
#include "../Enemy/AnimatedEnemy/AnimatedEnemy.h"
/*To access objects in list for removal.*/
#include "../EntityManager.h"
/*For using string*/
#include <string>

using std::string;

Map_Editor::Map_Editor() :
	optionSelectionLevel(OPTION_SELECT_NONE)
	, objectType(OBJECT_TYPE_NONE)
	, environmentObject(ENVIRONMENT_OBJECT_NONE)
	, enemyObject(ENEMY_OBJECT_NONE)
	, mapEditing(false)
	, _displacement(0.f, -10.f, 0.f)
	, _displacementModifier(0.f, -10.f, 0.f)
	, _scale(1.f, 1.f, 1.f)
	, _rotate(0.f)
	, ss_OptionSelectLevel("")
	, ss_ObjectType("")
	, ss_EnvironmentObject("")
	, ss_EnemyObject("")
	, ss_ScaleAxis("")
	, addWaypoint(false)
	, _enemy(nullptr)
	, turret(nullptr)
	, _horde(nullptr)
	, _tower(nullptr)
	, lastCreatedType(CREATED_NONE)
	, scaleAxis(SCALE_ALL)
	, enemyNo(0)
{
}

Map_Editor::~Map_Editor()
{
	if (_enemy)
	{
		delete _enemy;
		_enemy = nullptr;
	}

	if (turret)
	{
		delete turret;
		turret = nullptr;
	}

	if (_horde)
	{
		delete _horde;
		_horde = nullptr;
	}

	if (_tower)
	{
		delete _tower;
		_tower = nullptr;
	}
}

void Map_Editor::renderObject(void)
{
	if (mapEditing)
	{
		switch (objectType)
		{
			case ENVIRONMENT:
			{
				switch (environmentObject)
				{
					case CRATE:
					{	
						MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
						modelStack.PushMatrix();
						modelStack.Translate(_displacement.x, _displacement.y, _displacement.z);
						modelStack.Rotate(_rotate, 0.f, 1.f, 0.f);
						modelStack.Scale(_scale.x, _scale.y, _scale.z);
						RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("crate"));
						modelStack.PopMatrix();
						break;
					}
					case WATCHTOWER:
					{
						MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
						modelStack.PushMatrix();
						modelStack.Translate(_displacement.x, _displacement.y, _displacement.z);
						modelStack.Rotate(_rotate, 0.f, 1.f, 0.f);
						modelStack.Scale(_scale.x, _scale.y, _scale.z);
						RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("WatchTower"));
						modelStack.PopMatrix();
						break;
					}
					case BARRICADE:
					{
						MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
						modelStack.PushMatrix();
						modelStack.Translate(_displacement.x, _displacement.y, _displacement.z);
						modelStack.Rotate(_rotate, 0.f, 1.f, 0.f);
						modelStack.Scale(_scale.x, _scale.y, _scale.z);
						RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("Barricade"));
						modelStack.PopMatrix();
						break;
					}
					case STATUE:
					{
						MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
						modelStack.PushMatrix();
						modelStack.Translate(_displacement.x, _displacement.y, _displacement.z);
						modelStack.Rotate(_rotate, 0.f, 1.f, 0.f);
						modelStack.Scale(_scale.x, _scale.y, _scale.z);
						RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("Statue"));
						modelStack.PopMatrix();
						break;
					}
					case BARREL:
					{
						MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
						modelStack.PushMatrix();
						modelStack.Translate(_displacement.x, _displacement.y, _displacement.z);
						modelStack.Rotate(_rotate, 0.f, 1.f, 0.f);
						modelStack.Scale(_scale.x, _scale.y, _scale.z);
						RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("Barrel"));
						modelStack.PopMatrix();
						break;
					}
					case WALL:
					{
						MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
						modelStack.PushMatrix();
						modelStack.Translate(_displacement.x, _displacement.y, _displacement.z);
						modelStack.Rotate(_rotate, 0.f, 1.f, 0.f);
						modelStack.Scale(_scale.x, _scale.y, _scale.z);
						RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("Wall"));
						modelStack.PopMatrix();
						break;
					}

				}
				break;
			}
			case ENEMY:
			{
				switch (enemyObject)
				{
					case ROBOT:
					{
						if (!addWaypoint)
						{
							MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
							modelStack.PushMatrix();
							modelStack.Translate(_displacement.x, _displacement.y, _displacement.z);
							modelStack.Rotate(_rotate, 0.f, 1.f, 0.f);
							modelStack.Scale(_scale.x, _scale.y, _scale.z);
							RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("ROBOT"));
							modelStack.PopMatrix();
						}
						else
						{
							MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
							modelStack.PushMatrix();
							modelStack.Translate(_displacement.x, _displacement.y, _displacement.z);
							modelStack.Rotate(_rotate, 0.f, 1.f, 0.f);
							modelStack.Scale(_scale.x, _scale.y, _scale.z);
							RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("cube"));
							modelStack.PopMatrix();
						}
						break;
					}

					case HORDE:
					{
						Vector3 _leftPosition(_displacement.x - (_scale.x * _scale.z * 2.f), _displacement.y, _displacement.z);
						Vector3 _rightPosition(_displacement.x + (_scale.x * _scale.z * 2.f), _displacement.y, _displacement.z);
						Vector3 _upPosition(_displacement.x, _displacement.y, _displacement.z + (_scale.x * _scale.z * 2.f));
						Vector3 _downPosition(_displacement.x, _displacement.y, _displacement.z - (_scale.x * _scale.z * 2.f));

						MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
						modelStack.PushMatrix();
						modelStack.Translate(_displacement.x, _displacement.y, _displacement.z);
						modelStack.Rotate(_rotate, 0.f, 1.f, 0.f);
						modelStack.Scale(_scale.x, _scale.y, _scale.z);
						RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("ROBOT"));
						modelStack.PopMatrix();

						modelStack.PushMatrix();
						modelStack.Translate(_leftPosition.x, _leftPosition.y, _leftPosition.z);
						modelStack.Rotate(_rotate, 0.f, 1.f, 0.f);
						modelStack.Scale(_scale.x, _scale.y, _scale.z);
						RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("ROBOT"));
						modelStack.PopMatrix();

						modelStack.PushMatrix();
						modelStack.Translate(_rightPosition.x, _rightPosition.y, _rightPosition.z);
						modelStack.Rotate(_rotate, 0.f, 1.f, 0.f);
						modelStack.Scale(_scale.x, _scale.y, _scale.z);
						RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("ROBOT"));
						modelStack.PopMatrix();

						modelStack.PushMatrix();
						modelStack.Translate(_upPosition.x, _upPosition.y, _upPosition.z);
						modelStack.Rotate(_rotate, 0.f, 1.f, 0.f);
						modelStack.Scale(_scale.x, _scale.y, _scale.z);
						RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("ROBOT"));
						modelStack.PopMatrix();

						modelStack.PushMatrix();
						modelStack.Translate(_downPosition.x, _downPosition.y, _downPosition.z);
						modelStack.Rotate(_rotate, 0.f, 1.f, 0.f);
						modelStack.Scale(_scale.x, _scale.y, _scale.z);
						RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("ROBOT"));
						modelStack.PopMatrix();


						break;
					}

					case TURRET:
					{
						MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
						modelStack.PushMatrix();
						modelStack.Translate(_displacement.x, _displacement.y, _displacement.z);
						modelStack.Rotate(_rotate, 0.f, 1.f, 0.f);
						modelStack.Scale(_scale.x, _scale.y, _scale.z);
						RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("turret"));
						modelStack.PopMatrix();
						break;
					}

					case TOWER:
					{
						MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
						modelStack.PushMatrix();
						modelStack.Translate(_displacement.x, _displacement.y, _displacement.z);
						modelStack.Rotate(_rotate, 0.f, 1.f, 0.f);
						modelStack.Scale(_scale.x, _scale.y, _scale.z);
						RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("TOWER"));
						modelStack.PopMatrix();
						break;
					}
				}
				break;
			}
		}
	}
}

void Map_Editor::renderOption(void)
{
	//Calculating aspect ratio
	float windowHeight = Application::GetInstance().GetWindowHeight();
	float windowWidth = Application::GetInstance().GetWindowWidth();

	static string s_OptionSelectionLevel = "";
	static string s_ObjectType = "";
	static string s_EnvironmentObject = "";
	static string s_EnemyObject = "";
	static string s_ScaleAxis = "";

	if (optionSelectionLevel == OPTION_SELECT_NONE)
		s_OptionSelectionLevel = "None";

	if (optionSelectionLevel == OBJECT_SELECT)
		s_OptionSelectionLevel = "Object Select";

	if (optionSelectionLevel == ENVIRONMENT_SELECT)
		s_OptionSelectionLevel = "Environment Select";

	if (optionSelectionLevel == ENEMY_SELECT)
		s_OptionSelectionLevel = "Enemy Select";

	if (objectType == OBJECT_TYPE_NONE)
		s_ObjectType = "None";

	if (objectType == ENVIRONMENT)
		s_ObjectType = "Environment";

	if (objectType == ENEMY)
		s_ObjectType = "Enemy";

	if (environmentObject == CRATE)
		s_EnvironmentObject = "Crate";

	if (environmentObject == WATCHTOWER)
		s_EnvironmentObject = "WatchTower";

	if (environmentObject == BARRICADE)
		s_EnvironmentObject = "Statue";

	if (environmentObject == STATUE)
		s_EnvironmentObject = "Statue";

	if (environmentObject == BARREL)
		s_EnvironmentObject = "Barrel";

	if (environmentObject == WALL)
		s_EnvironmentObject = "Wall";

	if (environmentObject == ENVIRONMENT_OBJECT_NONE)
		s_EnvironmentObject = "None";

	if (enemyObject == ROBOT)
		s_EnemyObject = "Robot";

	if (enemyObject == HORDE)
		s_EnemyObject = "Horde";

	if (enemyObject == TURRET)
		s_EnemyObject = "Turret";

	if (enemyObject == TOWER)
		s_EnemyObject = "Tower";

	if (enemyObject == ENEMY_OBJECT_NONE)
		s_EnemyObject = "None";

	if (scaleAxis == SCALE_ALL)
		s_ScaleAxis = "All";

	if (scaleAxis == SCALE_X)
		s_ScaleAxis = "X";

	if (scaleAxis == SCALE_Y)
		s_ScaleAxis = "Y";

	if (scaleAxis == SCALE_Z)
		s_ScaleAxis = "Z";

	ss_OptionSelectLevel.str(s_OptionSelectionLevel);
	ss_ObjectType.str(s_ObjectType);
	ss_EnvironmentObject.str(s_EnvironmentObject);
	ss_EnemyObject.str(s_EnemyObject);
	ss_ScaleAxis.str(s_ScaleAxis);

	std::ostringstream playerPosition("");
	playerPosition << CPlayerInfo::GetInstance()->GetPos();

	std::ostringstream playerTarget("");
	playerTarget << CPlayerInfo::GetInstance()->GetTarget();

	std::ostringstream ss_ObjectRotate("");
	ss_ObjectRotate << _rotate;

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(-windowWidth * 0.48f, windowHeight * 0.4f, 0.f);
	modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.04f, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
	RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), "Option Selection Level:" + ss_OptionSelectLevel.str(), Color(1.f, 0.f, 0.f));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-windowWidth * 0.48f, windowHeight * 0.35f, 0.f);
	modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.04f, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
	RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), "Object Type:" + ss_ObjectType.str(), Color(1.f, 0.f, 0.f));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-windowWidth * 0.48f, windowHeight * 0.3f, 0.f);
	modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.04f, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
	RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), "Environment Object:" + ss_EnvironmentObject.str(), Color(1.f, 0.f, 0.f));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-windowWidth * 0.48f, windowHeight * 0.25f, 0.f);
	modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.04f, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
	RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), "Enemy Object:" + ss_EnemyObject.str(), Color(1.f, 0.f, 0.f));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-windowWidth * 0.48f, windowHeight * 0.20f, 0.f);
	modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.04f, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
	RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), "Scale Axis:" + ss_ScaleAxis.str(), Color(1.f, 0.f, 0.f));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-windowWidth * 0.48f, windowHeight * 0.15f, 0.f);
	modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.04f, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
	RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), "Rotate:" + ss_ObjectRotate.str(), Color(1.f, 0.f, 0.f));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-windowWidth * 0.48f, windowHeight * 0.10f, 0.f);
	modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.04f, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
	RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), "Player Position:" + playerPosition.str(), Color(1.f, 0.f, 0.f));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-windowWidth * 0.48f, windowHeight * 0.05f, 0.f);
	modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.04f, Application::GetInstance().GetWindowWidth() * 0.04f, 1.f);
	RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), "Player Target:" + playerTarget.str(), Color(1.f, 0.f, 0.f));
	modelStack.PopMatrix();
}

void Map_Editor::updateOption(double dt)
{
	static CPlayerInfo* _player = CPlayerInfo::GetInstance();

	_displacement.Set(_player->GetTarget().x - _player->GetPos().x, -10.f, _player->GetTarget().z - _player->GetPos().z);
	_displacement.Set(static_cast<float>((_displacement.x * 50.f) + _player->GetPos().x), -10.f, static_cast<float>((_displacement.z * 50.f) + _player->GetPos().z));

	//cout << "Displacement: " << _displacement << endl;

	//cout << "Option Selection " << optionSelectionLevel << endl;
	//cout << "Object Type " << objectType << endl;
	//cout << "Environment Object " << environmentObject << endl;
	//cout << "Enemy Object " << enemyObject << endl;
	//cout << "Update Displacement: " << _displacement << endl;

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD4))
	{
		if (optionSelectionLevel == OBJECT_SELECT && optionSelectionLevel != ENVIRONMENT_SELECT && optionSelectionLevel != ENEMY_SELECT)
		{
			optionSelectionLevel = ENEMY_SELECT;
		}

		if (optionSelectionLevel == ENVIRONMENT_SELECT)
		{
			objectType = ENVIRONMENT;

			if (objectType == ENVIRONMENT)
			{
				if (environmentObject == ENVIRONMENT_OBJECT_NONE)
					environmentObject = WATCHTOWER;

				else if (environmentObject == WATCHTOWER)
					environmentObject = BARRICADE;

				else if (environmentObject == BARRICADE)
					environmentObject = STATUE;

				else if (environmentObject == STATUE)
					environmentObject = BARREL;

				else if (environmentObject == BARREL)
					environmentObject = WALL;

				else if (environmentObject == WALL)
					environmentObject = CRATE;

				else if (environmentObject == CRATE)
					environmentObject = ENVIRONMENT_OBJECT_NONE;
			}
		}
		else if (optionSelectionLevel == ENEMY_SELECT)
		{
			objectType = ENEMY;
			if (objectType == ENEMY)
			{
				cout << "Enemy Object: " << enemyObject << endl;
				if (enemyObject == ENEMY_OBJECT_NONE)
					enemyObject = TOWER;

				else if (enemyObject == TOWER)
					enemyObject = TURRET;

				else if (enemyObject == TURRET)
					enemyObject = HORDE;

				else if (enemyObject == HORDE)
					enemyObject = ROBOT;

				else if (enemyObject == ROBOT)
					enemyObject = ENEMY_OBJECT_NONE;
			}
		}
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD6))
	{
		if (optionSelectionLevel == OBJECT_SELECT && optionSelectionLevel != ENVIRONMENT_SELECT && optionSelectionLevel != ENEMY_SELECT)
		{
			optionSelectionLevel = ENVIRONMENT_SELECT;
		}

		if (optionSelectionLevel == ENVIRONMENT_SELECT)
		{
			objectType = ENVIRONMENT;

			if (objectType == ENVIRONMENT)
			{
				if (environmentObject == ENVIRONMENT_OBJECT_NONE)
					environmentObject = CRATE;

				else if (environmentObject == CRATE)
					environmentObject = BARREL;

				else if (environmentObject == BARREL)
					environmentObject = STATUE;

				else if (environmentObject == STATUE)
					environmentObject = BARRICADE;

				else if(environmentObject == CRATE)
					environmentObject = BARRICADE;

				else if (environmentObject == BARRICADE)
					environmentObject = WALL;

				else if (environmentObject == WALL)
					environmentObject = WATCHTOWER;

				else if (environmentObject == WATCHTOWER)
					environmentObject = ENVIRONMENT_OBJECT_NONE;
			}
		}
		else if (optionSelectionLevel == ENEMY_SELECT)
		{
			objectType = ENEMY;

			if (objectType == ENEMY)
			{
				cout << "Enemy Object: " << enemyObject << endl;
				if (enemyObject == ENEMY_OBJECT_NONE)
					enemyObject = ROBOT;

				else if (enemyObject == ROBOT)
					enemyObject = HORDE;

				else if (enemyObject == HORDE)
					enemyObject = TURRET;

				else if (enemyObject == TURRET)
					enemyObject = TOWER;

				else if (enemyObject == TOWER)
					enemyObject = ENEMY_OBJECT_NONE;
			}
		}
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD9))
	{
		mapEditing = false;
	}

	/*Back to first option.*/
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD7))
	{
		cout << "PRESSED NUMPAD 7 " << optionSelectionLevel << endl;
		if (optionSelectionLevel == ENVIRONMENT_SELECT || optionSelectionLevel == ENEMY_SELECT || optionSelectionLevel == OPTION_SELECT_NONE)
		{
			optionSelectionLevel = OBJECT_SELECT;
			objectType = OBJECT_TYPE_NONE;
			environmentObject = ENVIRONMENT_OBJECT_NONE;
			enemyObject = ENEMY_OBJECT_NONE;
		}
	}

	/*Rotate objects.*/
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD8))
	{
		if (_rotate >= 270.f)
			_rotate = 0.f;
		else if (_rotate < 270.f)
			_rotate += 90.f;
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_LMENU))
	{
		if (scaleAxis == SCALE_ALL)
			scaleAxis = SCALE_X;

		else if (scaleAxis == SCALE_X)
			scaleAxis = SCALE_Y;

		else if (scaleAxis == SCALE_Y)
			scaleAxis = SCALE_Z;

		else if (scaleAxis == SCALE_Z)
			scaleAxis = SCALE_ALL;
	}

	if (KeyboardController::GetInstance()->IsKeyDown(VK_LMENU) && KeyboardController::GetInstance()->IsKeyDown(VK_LSHIFT))
		_scale.Set(1.f, 1.f, 1.f);

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD3))
	{
		if (scaleAxis == SCALE_X)
			_scale.x += 1.f;

		else if (scaleAxis == SCALE_Y)
			_scale.y += 1.f;

		else if (scaleAxis == SCALE_Z)
			_scale.z += 1.f;

		else if (scaleAxis == SCALE_ALL)
		{
			_scale.x += 1.f;
			_scale.y += 1.f;
			_scale.z += 1.f;
		}
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD1))
	{
		if (scaleAxis == SCALE_X)
			_scale.x -= 1.f;

		else if (scaleAxis == SCALE_Y)
			_scale.y -= 1.f;

		else if (scaleAxis == SCALE_Z)
			_scale.z -= 1.f;

		else if (scaleAxis == SCALE_ALL)
		{
			_scale.x -= 1.f;
			_scale.y -= 1.f;
			_scale.z -= 1.f;
		}
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD9))
		addWaypoint = false;

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD0))
		removeLastCreated();
	//if (KeyboardController::GetInstance()->IsKeyDown(VK_NUMPAD8))
	//	_displacementModifier += _player->GetTarget() - _player->GetPos();

	//if (KeyboardController::GetInstance()->IsKeyDown(VK_NUMPAD2))
	//	_displacementModifier -= _player->GetTarget() - _player->GetPos();

	/*Preventing scale to hit less than 0 that results in Divide By Zero Error.*/
	if (_scale.x < 1.f)
		_scale.x = 1.f;

	if (_scale.y < 1.f)
		_scale.y = 1.f;

	if (_scale.z < 1.f)
		_scale.z = 1.f;

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD5))
	{
		if (objectType == ENVIRONMENT)
		{
			if (environmentObject == CRATE)
			{
				cout << "CREATED" << endl;
				cout << "Displacement: " << _displacement << endl;
				Vector3 _minAABB(-_scale.x, 0.f, -_scale.z);
				Vector3 _maxAABB(_scale);
				CFurniture* crate = Create::Furniture("crate", _displacement, _scale, _rotate);
				crate->SetCollider(true);
				crate->SetLight(true);
				crate->SetAABB(_maxAABB, _minAABB);
				//cout << "_minAABB: " << crate->GetMinAABB() << endl;
				//cout << "_maxAABB: " << crate->GetMaxAABB() << endl;
				//crate->SetPosition(Vector3(0.f, -10.f, 0.f));
				lastCreatedType = CREATED_ENVIRONMENT;
			}

			if (environmentObject == WATCHTOWER)
			{
				cout << "CREATED" << endl;
				cout << "Displacement: " << _displacement << endl;
				Vector3 _minAABB(-_scale.x, 0.f, -_scale.z);
				Vector3 _maxAABB(_scale);
				CFurniture* crate = Create::Furniture("WatchTower", _displacement, _scale, _rotate);
				crate->SetCollider(true);
				crate->SetLight(true);
				crate->SetAABB(_maxAABB, _minAABB);
				//cout << "_minAABB: " << crate->GetMinAABB() << endl;
				//cout << "_maxAABB: " << crate->GetMaxAABB() << endl;
				//crate->SetPosition(Vector3(0.f, -10.f, 0.f));
				lastCreatedType = CREATED_ENVIRONMENT;
			}

			if (environmentObject == BARRICADE)
			{
				cout << "CREATED" << endl;
				cout << "Displacement: " << _displacement << endl;
				Vector3 _minAABB(-_scale.x, 0.f, -_scale.z);
				Vector3 _maxAABB(_scale);
				CFurniture* crate = Create::Furniture("Barricade", _displacement, _scale, _rotate);
				crate->SetCollider(true);
				crate->SetLight(true);
				crate->SetAABB(_maxAABB, _minAABB);
				//cout << "_minAABB: " << crate->GetMinAABB() << endl;
				//cout << "_maxAABB: " << crate->GetMaxAABB() << endl;
				//crate->SetPosition(Vector3(0.f, -10.f, 0.f));
				lastCreatedType = CREATED_ENVIRONMENT;
			}

			if (environmentObject == STATUE)
			{
				cout << "CREATED" << endl;
				cout << "Displacement: " << _displacement << endl;
				Vector3 _minAABB(-_scale.x, 0.f, -_scale.z);
				Vector3 _maxAABB(_scale);
				CFurniture* crate = Create::Furniture("Statue", _displacement, _scale, _rotate);
				crate->SetCollider(true);
				crate->SetLight(true);
				crate->SetAABB(_maxAABB, _minAABB);
				//cout << "_minAABB: " << crate->GetMinAABB() << endl;
				//cout << "_maxAABB: " << crate->GetMaxAABB() << endl;
				//crate->SetPosition(Vector3(0.f, -10.f, 0.f));
				lastCreatedType = CREATED_ENVIRONMENT;
			}

			if (environmentObject == BARREL)
			{
				cout << "CREATED" << endl;
				cout << "Displacement: " << _displacement << endl;
				Vector3 _minAABB(-_scale.x, 0.f, -_scale.z);
				Vector3 _maxAABB(_scale);
				CFurniture* crate = Create::Furniture("Barrel", _displacement, _scale, _rotate);
				crate->SetCollider(true);
				crate->SetLight(true);
				crate->SetAABB(_maxAABB, _minAABB);
				//cout << "_minAABB: " << crate->GetMinAABB() << endl;
				//cout << "_maxAABB: " << crate->GetMaxAABB() << endl;
				//crate->SetPosition(Vector3(0.f, -10.f, 0.f));
				lastCreatedType = CREATED_ENVIRONMENT;
			}

			if (environmentObject == WALL)
			{
				cout << "CREATED" << endl;
				cout << "Displacement: " << _displacement << endl;
				Vector3 _minAABB(0.f, 0.f, 0.f);
				Vector3 _maxAABB(0.f, 0.f, 0.f);
				if (_rotate == 0 || _rotate == 180)
				{
					_minAABB.Set(-_scale.x * 8.f, -10.f, -_scale.z);
					_maxAABB.Set(_scale.x * 8.f, 100.f, _scale.z);
				}
				else
				{
					_minAABB.Set(-_scale.x, 10.f, -_scale.z * 8.f);
					_maxAABB.Set(_scale.x, 100.f, _scale.z * 8.f);
				}
				CFurniture* crate = Create::Furniture("Wall", _displacement, _scale, _rotate);
				crate->SetCollider(true);
				crate->SetLight(true);
				crate->SetAABB(_maxAABB, _minAABB);
				//cout << "_minAABB: " << crate->GetMinAABB() << endl;
				//cout << "_maxAABB: " << crate->GetMaxAABB() << endl;
				//crate->SetPosition(Vector3(0.f, -10.f, 0.f));
				lastCreatedType = CREATED_ENVIRONMENT;
			}
		}
		else if (objectType == ENEMY)
		{
			if (enemyObject == ROBOT)
			{
				if (!addWaypoint)
				{
					enemyNo = 1;
					_enemy = Create::AnimatedEnemy("ROBOT_CORE", "ROBOT_LeftArm", "ROBOT_RightArm", "ROBOT_LeftLeg", "ROBOT_RightLeg", "ROBOT_Head", _displacement, _scale);
					_enemy->SetLight(true);
					addWaypoint = true;
					lastCreatedType = CREATED_ENEMY;
				}
				//else
					//_enemy->addWaypoint(_displacement);

				if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD9))
					addWaypoint = false;

			}
			else if (enemyObject == HORDE)
			{
				enemyNo = 2;

				cout << "Create Displacement: " << _displacement << endl;
				_horde = Create::Horde("ROBOT", _displacement, _scale);
				lastCreatedType = CREATED_ENEMY_HORDE;
			}
			else if (enemyObject == TURRET)
			{
				enemyNo = 3;

				cout << "Create Displacement: " << _displacement << endl;
				turret = Create::Enemy3D("turret", _displacement, _scale);
				lastCreatedType = CREATED_ENEMY;
			}
			else if (enemyObject == TOWER)
			{
				enemyNo = 4;

				cout << "Create Displacement: " << _displacement << endl;
				_tower = Create::Tower("TOWER", _displacement, _rotate, _scale);
				lastCreatedType = CREATED_ENEMY;
			}

		}
		if (!addWaypoint)
			resetOption();
	}
}

void Map_Editor::resetOption(void)
{
	optionSelectionLevel = OBJECT_SELECT;
	objectType = OBJECT_TYPE_NONE;
	environmentObject = ENVIRONMENT_OBJECT_NONE;
	enemyObject = ENEMY_OBJECT_NONE;
}

void Map_Editor::removeLastCreated(void)
{
	if (lastCreatedType == CREATED_ENVIRONMENT)
	{
		CFurniture* object = EntityManager::GetInstance()->returnFixed().back();
		delete object;
		object = nullptr;

		EntityManager::GetInstance()->returnFixed().pop_back();
	}
	else if (lastCreatedType == CREATED_ENEMY)
	{
		CEnemy3D* object = EntityManager::GetInstance()->returnEnemy().back();
		delete object;
		object = nullptr;

		EntityManager::GetInstance()->returnEnemy().pop_back();
	}
	else if (lastCreatedType == CREATED_ENEMY_HORDE)
	{
		for (int i = 0; i < 5; ++i)
		{
			CEnemy3D* object = EntityManager::GetInstance()->returnEnemy().back();
			delete object;
			object = nullptr;

			EntityManager::GetInstance()->returnEnemy().pop_back();
		}
	}

	lastCreatedType = CREATED_NONE;
	addWaypoint = false;
}
