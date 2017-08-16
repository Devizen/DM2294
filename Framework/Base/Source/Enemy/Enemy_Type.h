#ifndef ENEMY_TYPE_H
#define ENEMY_TYPE_H

#include <vector>
using std::vector;

class Vector3;

struct Enemy_Type
{
	enum ENEMY_BEHAVIOR
	{
		PATROL = 0,
		TOWER
	};

	void setEnemyType(ENEMY_BEHAVIOR _enemyBehavior);
	ENEMY_BEHAVIOR enemyBehavior;

	/*Patrol Enemy*/

	/*Increase waypointToGo*/
	void increaseWaypointToGo(void);
	/*Set waypointToGo to Zero*/
	void setToZeroWaypointToGo(void);

	void addWaypoint(Vector3 _waypointToGo);
	int waypointToGo;
	vector<Vector3>waypoint;
};

#endif