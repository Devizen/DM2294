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
	, ss_OptionSelectLevel("")
	, ss_ObjectType("")
	, ss_EnvironmentObject("")
	, ss_EnemyObject("")
	, addWaypoint(false)
	, _enemy(nullptr)
	, turret(nullptr)
{
}

Map_Editor::~Map_Editor()
{
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
						modelStack.Scale(_scale.x, _scale.y, _scale.z);
						RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("crate"));
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
							modelStack.Scale(_scale.x, _scale.y, _scale.z);
							RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("ROBOT"));
							modelStack.PopMatrix();
						}
						else
						{
							MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
							modelStack.PushMatrix();
							modelStack.Translate(_displacement.x, _displacement.y, _displacement.z);
							modelStack.Scale(_scale.x, _scale.y, _scale.z);
							RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("cube"));
							modelStack.PopMatrix();
						}
						break;
					}
					case TOWER:
					{
						MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
						modelStack.PushMatrix();
						modelStack.Translate(_displacement.x, _displacement.y, _displacement.z);
						modelStack.Scale(_scale.x, _scale.y, _scale.z);
						RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("turret"));
						modelStack.PopMatrix();
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
						modelStack.Scale(_scale.x, _scale.y, _scale.z);
						RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("ROBOT"));
						modelStack.PopMatrix();

						modelStack.PushMatrix();
						modelStack.Translate(_leftPosition.x, _leftPosition.y, _leftPosition.z);
						modelStack.Scale(_scale.x, _scale.y, _scale.z);
						RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("ROBOT"));
						modelStack.PopMatrix();

						modelStack.PushMatrix();
						modelStack.Translate(_rightPosition.x, _rightPosition.y, _rightPosition.z);
						modelStack.Scale(_scale.x, _scale.y, _scale.z);
						RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("ROBOT"));
						modelStack.PopMatrix();

						modelStack.PushMatrix();
						modelStack.Translate(_upPosition.x, _upPosition.y, _upPosition.z);
						modelStack.Scale(_scale.x, _scale.y, _scale.z);
						RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("ROBOT"));
						modelStack.PopMatrix();

						modelStack.PushMatrix();
						modelStack.Translate(_downPosition.x, _downPosition.y, _downPosition.z);
						modelStack.Scale(_scale.x, _scale.y, _scale.z);
						RenderHelper::RenderMeshWithLight(MeshBuilder::GetInstance()->GetMesh("ROBOT"));
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
	static string s_OptionSelectionLevel = "";
	static string s_ObjectType = "";
	static string s_EnvironmentObject = "";
	static string s_EnemyObject = "";

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

	if (environmentObject == ENVIRONMENT_OBJECT_NONE)
		s_EnvironmentObject = "None";

	if (enemyObject == ROBOT)
		s_EnemyObject = "Robot";

	if (enemyObject == TOWER)
		s_EnemyObject = "Tower";

	if (enemyObject == HORDE)
		s_EnemyObject = "Horde";

	if (enemyObject == ENEMY_OBJECT_NONE)
		s_EnemyObject = "None";

	ss_OptionSelectLevel.str(s_OptionSelectionLevel);
	ss_ObjectType.str(s_ObjectType);
	ss_EnvironmentObject.str(s_EnvironmentObject);
	ss_EnemyObject.str(s_EnemyObject);

	Mtx44 ortho;
	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	Mtx44& projectionStack = GraphicsManager::GetInstance()->GetProjectionMatrix();
	projectionStack.SetToOrtho(-80, 80, -60, 60, -10, 10);
	Mtx44& viewStack = GraphicsManager::GetInstance()->GetViewMatrix();
	viewStack.SetToIdentity();
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(-75.f, 50.f, 0.f);
	modelStack.Scale(Application::GetInstance().getAspectRatioWidth() * 0.05f, Application::GetInstance().getAspectRatioWidth() * 0.05f, 1.f);
	RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), "Option Selection Level:" + ss_OptionSelectLevel.str(), Color(1.f, 0.f, 0.f));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-75.f, 45.f, 0.f);
	modelStack.Scale(Application::GetInstance().getAspectRatioWidth() * 0.05f, Application::GetInstance().getAspectRatioWidth() * 0.05f, 1.f);
	RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), "Object Type:" + ss_ObjectType.str(), Color(1.f, 0.f, 0.f));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-75.f, 40.f, 0.f);
	modelStack.Scale(Application::GetInstance().getAspectRatioWidth() * 0.05f, Application::GetInstance().getAspectRatioWidth() * 0.05f, 1.f);
	RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), "Environment Object:" + ss_EnvironmentObject.str(), Color(1.f, 0.f, 0.f));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-75.f, 35.f, 0.f);
	modelStack.Scale(Application::GetInstance().getAspectRatioWidth() * 0.05f, Application::GetInstance().getAspectRatioWidth() * 0.05f, 1.f);
	RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), "Enemy Object:" + ss_EnemyObject.str(), Color(1.f, 0.f, 0.f));
	modelStack.PopMatrix();
}

void Map_Editor::updateOption(double dt)
{
	static CPlayerInfo* _player = CPlayerInfo::GetInstance();

	_displacement.Set(_player->GetTarget().x - _player->GetPos().x, -10.f, _player->GetTarget().z - _player->GetPos().z);
	_displacement.Set((_displacement.x * 50.f) + _player->GetPos().x, -10.f, (_displacement.z * 50.f) + _player->GetPos().z);

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
					enemyObject = HORDE;

				else if (enemyObject == HORDE)
					enemyObject = TOWER;

				else if (enemyObject == TOWER)
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
				if (environmentObject == CRATE)
					environmentObject = ENVIRONMENT_OBJECT_NONE;

				else if (environmentObject == ENVIRONMENT_OBJECT_NONE)
					environmentObject = CRATE;
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
					enemyObject = TOWER;

				else if (enemyObject == TOWER)
					enemyObject = HORDE;

				else if (enemyObject == HORDE)
					enemyObject = ENEMY_OBJECT_NONE;
			}
		}
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD9))
	{
		mapEditing = false;
	}

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

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD3))
	{
		_scale.x += 1.f;
		_scale.y += 1.f;
		_scale.z += 1.f;
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD1))
	{
		_scale.x -= 1.f;
		_scale.y -= 1.f;
		_scale.z -= 1.f;
	}

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

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
	{
		if (objectType == ENVIRONMENT)
		{
			if (environmentObject == CRATE)
			{
				cout << "CREATED" << endl;
				cout << "Displacement: " << _displacement << endl;
				Vector3 _minAABB(-6.f, -6.f, -6.f);
				Vector3 _maxAABB(6.f, 6.f, 6.f);

				CFurniture* crate = Create::Furniture("crate", _displacement, _scale);
				crate->SetCollider(true);
				crate->SetLight(true);
				crate->SetAABB(_scale, -_scale);
			}
		}
		else if (objectType == ENEMY)
		{
			if (enemyObject == ROBOT)
			{
				if (!addWaypoint)
				{
					_enemy = Create::Patrol("ROBOT", _displacement, _scale);
					_enemy->SetAABB(Vector3(_scale.x, _scale.y * 3.f, _scale.z), Vector3(-_scale.x, -_scale.y, -_scale.z));
					_enemy->SetLight(true);
					addWaypoint = true;
				}
				else
					_enemy->addWaypoint(_displacement);

				if (KeyboardController::GetInstance()->IsKeyPressed(VK_NUMPAD9))
					addWaypoint = false;
			}
			else if (enemyObject == TOWER)
			{
				cout << "Create Displacement: " << _displacement << endl;
				turret = Create::Enemy3D("turret", _displacement, _scale);
				turret->setAlertBoundary(Vector3(-150.f, -10.f, -150.f), Vector3(150.f, 10.f, 150.f));
				turret->SetCollider(true);
				turret->SetLight(true);
				turret->SetAABB(_scale, -_scale);
				turret->setMaxHealthTo(10.f);
				turret->setHealthTo(10.f);
				turret->setAttackTo(1.f);
				turret->setDefenseTo(1.f);
				turret->setPlayerProperty(false);
				turret->SetLight(true);
			}

			else if (enemyObject == HORDE)
			{
				cout << "Create Displacement: " << _displacement << endl;
				_horde = Create::Horde("ROBOT", _displacement, _scale);
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
