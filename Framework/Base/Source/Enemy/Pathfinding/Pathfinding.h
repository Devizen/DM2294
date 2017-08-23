/*Pathfinding logic done by Lai Wei Jing, Eugene (160540A)*/
/*Tested by Chan Zhi Hao, Lim Guan Sheng, Marcus and Kwong Yu Ming*/
#ifndef PATHFINDING_H
#define PATHFINDING_H

/*Pathfinding Theory*/
/*
Object will use normal AABB collision at first.
When it hits the wall or any objects, it will return to it's previous position and set a boolean to true called pathFindingMode.
It will generate 36 points in a circle around the object.
Each points will be checked if it collided with something, if collided, remove the points.
The remaining points will be used to check and compare with the target position.
The closest to the target will be selected.
Object will then move towards this selected position.
*/

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
