#pragma once
#include "../Enemy3D.h"

class CEnemy3D;

using namespace std;

class Mesh;

class CPatrol :
	public CEnemy3D

{
public:
	CPatrol(Mesh* _modelMesh);
	virtual ~CPatrol();

	virtual void Init(void);
	// Reset this player instance to default
	virtual void Reset(void);

	// Update
	virtual void Update(double dt = 0.0333f);

	// Render
	virtual void Render(void);

	/*Check if player is inside defined boundary around enemy.*/
	virtual bool checkInsideBoundary(Vector3 minBoundary, Vector3 maxBoundary);

	/*Set Shoot Delay*/
	virtual void setShootDelay(float _shootDelay);

	/*Get Player Property*/
	virtual float getShootDelay(void);

	/*Render Health Bar*/
	//virtual void renderHealthBar(void);

	/*Increase waypointToGo*/
	void increaseWaypointToGo(void);
	/*Set waypointToGo to Zero*/
	void setToZeroWaypointToGo(void);

	void addWaypoint(Vector3 _waypointToGo);

	int waypointToGo;
	vector<Vector3>waypoint;
private:
	Vector3 minAlertBoundary, maxAlertBoundary;
	double portableDT;
	float angleToFace;
	/*Check if enemy belongs to player.*/
	bool playerProperty;
	float shootDelay;
	float m_fElapsedTimeBeforeUpdate;
	AI_STATE state;
};

namespace Create
{
	CPatrol* Patrol(const std::string& _meshName,
		const Vector3& _position,
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));
};
