#include "Player.h"
#include <utility>
#include <map>
#include "../Enemy/Enemy.h"
#include "../Enemy/Enemy_Manager.h"
#include "../Collision/Collision.h"
#include "../Battle/Battle.h"
/*Terrain.*/
#include "../Terrain/LoadHmap.h"
#include "SceneManager.h"

/*Debugging purpose.*/
#include <iostream>

using std::cout;
using std::endl;

//using std::map;
//using std::pair;

CPlayer::CPlayer() :
	position(0.f, 0.f, 0.f)
	, battle(nullptr)
{
}

CPlayer::~CPlayer()
{
	if (battle)
	{
		delete battle;
		battle = nullptr;
	}
}

void CPlayer::SetPosition(Vector3 _position)
{
	position = _position;
}

Vector3 CPlayer::GetPosition(void)
{
	return position;
}

void CPlayer::SetTarget(Vector3 _target)
{
	target = _target;
}

Vector3 CPlayer::GetTarget(void)
{
	return target;
}

CBattle * CPlayer::GetBattle(void)
{
	if (!battle)
		battle = new CBattle();

	return battle;
}

void CPlayer::MovePlayer(float move, MOVE_PLAYER direction)
{
	/*Z Axis is flipped.*/
	if (direction == MOVE_UP)
	{
		position.z -= move;
		target = Vector3(position.x, position.y, position.z - 1.f);
	}
	else if (direction == MOVE_DOWN)
	{
		position.z += move;
		target = Vector3(position.x, position.y, position.z + 1.f);
	}
	else if (direction == MOVE_LEFT)
	{
		position.x -= move;
		target = Vector3(position.x - 1.f, position.y, position.z);
	}
	else if (direction == MOVE_RIGHT)
	{
		position.x += move;
		target = Vector3(position.x + 1.f, position.y, position.z);
	}
}

void CPlayer::Update(double dt)
{
	static Vector3 previousPosition(0.f, 0.f, 0.f);

	//target =  Vector3(0.f * position.x, 0.f * position.y, 1.f * position.z);

	map<string, CEnemy*>* enemyList = CEnemy_Manager::GetInstance()->GetEnemyList();

	for (map<string, CEnemy*>::iterator it = enemyList->begin(); it != enemyList->end(); ++it)
	{
		if (Check::AABB(position, it->second->GetPosition()))
		{
			if (!this->GetBattle()->GetBattleState())
			{
				it->second->SetBattleMode(true);
				it->second->setHealthTo(3);
				it->second->setMaxHealthTo(3);
				it->second->setAttackTo(1);
				it->second->setSpeed(1);
				it->second->setEXP(1);
				Activate::BattleScene(this->battle, it->second, this);
			}
			position = previousPosition;
			break;
		}
	}
	previousPosition = position;

	/*Keep updating position.y to terrain height.*/
	position.y = (350.f * ReadHeightMap(SceneManager::GetInstance()->GetHeightMap(), position.x / 4000.f, position.z / 4000.f)) + 5.f;
	//cout << "Position: " << position << endl;
	//cout << "Target: " << target << endl;
}
