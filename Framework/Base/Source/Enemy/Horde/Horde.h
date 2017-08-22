/*Horde was done by Lai Wei Jing, Eugene (160540A)*/
/*Tested by Chan Zhi Hao, Lim Guan Sheng, Marcus and Kwong Yu Ming*/
#pragma once
#include "../Enemy3D.h"
class CEnemy3D;

using namespace std;

class Mesh;

class CHorde :
	public CEnemy3D

{
public:
	CHorde(Mesh* _modelMesh);
	virtual ~CHorde();

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

	/*Set Default Position*/
	void setDefaultPosition(Vector3 _defaultPosition);

	/*Get Default Position*/
	const Vector3 getDefaultPosition(void);
private:
	Vector3 minAlertBoundary, maxAlertBoundary, defaultPosition;
	double portableDT;
	float angleToFace;
	float shootDelay;
	float m_fElapsedTimeBeforeUpdate;
	AI_STATE state;
};

namespace Create
{
	CHorde* Horde(const std::string& _meshName,
		const Vector3& _position,
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));
};
