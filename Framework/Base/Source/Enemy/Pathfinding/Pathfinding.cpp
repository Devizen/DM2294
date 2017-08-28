/*Pathfinding logic done by Lai Wei Jing, Eugene (160540A)*/
/*Tested by Chan Zhi Hao, Lim Guan Sheng, Marcus and Kwong Yu Ming*/
#include "Pathfinding.h"
#include "../Patrol/Patrol.h"
#include "../../Object/Furniture.h"
#include "../../EntityManager.h"
#include "../../PlayerInfo/PlayerInfo.h"


Pathfinding::Pathfinding() :
	pathFindingMode(false)
	, path(0)
	, targetObjectPosition(0.f, 0.f, 0.f)
	, nearestPosition(0.f, 0.f, 0.f)
	, positionWithoutY(0.f, 0.f, 0.f)
	, directionToGo(0.f, 0.f, 0.f)
	, scanned(false)
{
}

Pathfinding::~Pathfinding()
{
}

void Pathfinding::updatePathfinding(Vector3 _position, Vector3 _scale, double dt)
{
	/*Creating a circle of positions around object.*/
	const float angleToTurn = 10.f;
	float rotate = 0;

	for (int i = 0; i < 360 / angleToTurn; ++i)
	{
		path.push_back(Vector3(((_scale.LengthSquared() / 4.f) * cos(Math::DegreeToRadian(rotate)) + _position.x), _position.y, ((_scale.LengthSquared() / 4.f) * sin(Math::DegreeToRadian(rotate)) + _position.z)));
		rotate += 10.f;
	}

	/*Removing positions that are inside the AABB of other objects.*/
	checkPathCollision(_scale);
}

void Pathfinding::checkPathCollision(Vector3 _scale)
{
	list<CEnemy3D*>enemyList = EntityManager::GetInstance()->returnEnemy();
	list<CFurniture*>fixedList = EntityManager::GetInstance()->returnFixed();
	static bool erased = false;

	for (vector<Vector3>::iterator it = path.begin(); it != path.end();)
	{
		Vector3 pathToGo = (Vector3)*it;

		for (list<CFurniture*>::iterator objIt = fixedList.begin(); objIt != fixedList.end(); ++objIt)
		{
			CFurniture* object = (CFurniture*)*objIt;

			//cout << "Object to Path Distance " << (pathToGo - object->GetPosition()).LengthSquared() << endl;
			if ((pathToGo - object->GetPosition()).LengthSquared() < 200.f)
			{
				it = path.erase(it);
				erased = true;
				break;
			}
		}

		if (!erased)
			++it;

		erased = false;
	}

	/*Reset variable just in case.*/
	erased = false;
	for (vector<Vector3>::iterator it = path.begin(); it != path.end();)
	{
		Vector3 pathToGo = (Vector3)*it;

		for (list<CEnemy3D*>::iterator enIt = enemyList.begin(); enIt != enemyList.end(); ++enIt)
		{
			CEnemy3D* enemy = (CEnemy3D*)*enIt;

			if (enemy == this)
				continue;

			//cout << "Displacement: " << (pathToGo - Vector3(enemy->GetPos().x, -10.f, enemy->GetPos().z)).LengthSquared() << endl;

			if ((pathToGo - Vector3(enemy->GetPos().x, -10.f, enemy->GetPos().z)).LengthSquared() < 200.f)
			{
				it = path.erase(it);
				erased = true;
				break;;
			}
			else
				continue;;
		}

		if (!erased)
			++it;

		erased = false;
	}
}

Vector3 Pathfinding::nearestPath(void)
{
	Vector3 nearest(_enemy->GetPos());

	if (path.size() > 0)
		nearest = path.back();

	//nearest.y = _enemy->GetPos().y;
	float currentRelativeDistance = 0.f;
	float nextRelativeDistance = 0.f;
	targetObjectPosition.y = -10.f;
	currentRelativeDistance = (targetObjectPosition - nearest).LengthSquared();

	for (vector<Vector3>::iterator it = path.begin(); it != path.end(); ++it)
	{
		Vector3 nextPath = (Vector3)*it;

		//cout << "Object to Path Distance " << (targetObjectPosition - nextPath).LengthSquared() << endl;

		if (nextPath == nearest)
			continue;

		//nextPath.y = _enemy->GetPos().y;
		nextRelativeDistance = (targetObjectPosition - nextPath).LengthSquared();

		if (nextRelativeDistance < currentRelativeDistance)
		{
			nearest = nextPath;
			currentRelativeDistance = nextRelativeDistance;
		}
	}
	/*Debug use.*/
	//nearest.y = _enemy->GetPos().y;
	nearestPosition = nearest;

	//cout << "Object to Nearest Distance " << (targetObjectPosition - nearest).LengthSquared() << endl;
	return nearest;
}
