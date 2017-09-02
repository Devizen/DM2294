#include "Player.h"
#include <utility>
#include <map>
#include "../Enemy/Enemy.h"
#include "../Enemy/Enemy_Manager.h"
#include "../Collision/Collision.h"

//using std::map;
//using std::pair;

CPlayer::CPlayer() :
	position(0.f, 0.f, 0.f)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::SetPosition(Vector3 _position)
{
	position = _position;
}

Vector3 CPlayer::GetPosition(void)
{
	return position;
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
			position = previousPosition;
			break;
		}
	}
	previousPosition = position;
}
