#include "Player.h"
#include <utility>
#include <map>
#include "../Enemy/Enemy.h"
#include "../Enemy/Enemy_Manager.h"
#include "../Collision/Collision.h"
#include "../Battle/Battle.h"

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
		position.z -= move;
	else if (direction == MOVE_DOWN)
		position.z += move;
	else if (direction == MOVE_LEFT)
		position.x -= move;
	else if (direction == MOVE_RIGHT)
		position.x += move;
}

void CPlayer::Update(double dt)
{
	static Vector3 previousPosition(0.f, 0.f, 0.f);

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
				Activate::BattleScene(this->battle, it->second, this);
			}
			position = previousPosition;
			break;
		}
	}
	previousPosition = position;
}
