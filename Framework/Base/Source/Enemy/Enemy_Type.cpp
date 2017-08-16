#include "Enemy_Type.h"
#include "Vector3.h"

void Enemy_Type::setEnemyType(ENEMY_BEHAVIOR _enemyBehavior)
{
	enemyBehavior = _enemyBehavior;
}

void Enemy_Type::increaseWaypointToGo(void)
{
	++waypointToGo;
}

void Enemy_Type::setToZeroWaypointToGo(void)
{
	waypointToGo = 0;
}

void Enemy_Type::addWaypoint(Vector3 _waypointToGo)
{
	waypoint.push_back(_waypointToGo);
}
