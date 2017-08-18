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

class Pathfinding/* : public Singleton<Pathfinding>*/
{
public:
	Pathfinding();
	~Pathfinding();
	/*Switching to path finding mode from normal movement mode.*/
	bool pathFindingMode;
	/*Move the object according to path finding positions.*/
	void updatePathfinding(double dt);
	/*Check if path finding positions collides with enemy and object.*/
	void checkPathCollision(void);
	/*Vector3 vector to store all the positions near object.*/
	
private:
protected:
};

#endif
