#ifndef PATHFINDING_H
#define PATHFINDING_H

/*Pathfinding Theory*/
/*
Enemy will use normal AABB collision at first.
When it hits the wall or any objects, it will return to it's previous position and set a boolean to true called startPathfinding.
Upon startPathfinding being set to true, the Pathfinding Singleton will be called.
It will then iterate through the list of nodes based on the enemy type.
It will keep check based on two condition.
The distance between the enemy and target, and, the distance between nodes to enemy.
It will go with something like
if ((nodePosition - enemyPos).LengthSquared() < f_nearestDistance)
	{
	nodeToCheck = nodePosition;
	f_nearestDistance = (nodePosition - enemyPos).LengthSquared();
	}

	if ((nodeToCheck - nodeToMove).LengthSquared() < f_nearestNode)

*/
///*Making Pathfinding a Singleton because you do not need to creates nodes individually for every enemy object.*/
//#include "SingletonTemplate.h"
/*Vector3 to store X, Y and Z position.*/
#include "Vector3.h"
/*Vector to store Vector3 positions.*/
#include <vector>

using std::vector;

class CEnemy3D;
class CPlayerInfo;

class Pathfinding/* : public Singleton<Pathfinding>*/
{
public:
	Pathfinding();
	~Pathfinding();
	/*Switching to path finding mode from normal movement mode.*/
	bool pathFindingMode;
	/*Move the object according to path finding positions.*/
	void updatePathfinding(Vector3 _position, Vector3 _scale, double dt);
	/*Check if path finding positions collides with enemy and object.*/
	void checkPathCollision(Vector3 _scale);
	/*Return nearest path to object.*/
	Vector3 nearestPath(void);
	/*Vector3 vector to store all the positions near object.*/
	vector<Vector3>path;
	/*A target object position.*/
	Vector3 targetObjectPosition;
	/*Position for checking if the object is close to the target.*/
	Vector3 nearestPosition;
	/*Position to ignore the Y value for checking because some object Y-axis is not at -10.f.*/
	Vector3 positionWithoutY;
	/*Fixed direction to go to target normalized.*/
	Vector3 directionToGo;
	CEnemy3D* _enemy;
	CPlayerInfo* _player;

	bool scanned;
private:
protected:
};

#endif