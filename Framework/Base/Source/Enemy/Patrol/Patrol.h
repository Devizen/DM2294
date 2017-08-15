#pragma once
#include "../../GenericEntity.h"
#include "../../GroundEntity.h"
#include "../Enemy3D.h"

#include <vector>

#define MAX_HEALTH_SCALE 10.f

using namespace std;

class Mesh;

class Patrol :
	public CEnemy3D
{
	friend class EntityManager;
	friend class SceneText;

	enum AI_STATE
	{
		IDLE = 0,
		ALERT,
		DEAD,
		RECOVERY,
	};

	enum ATTRIBUTE_TYPES
	{
		ENEMY_HEALTH = 0,
		ENEMY_ATTACK,
		ENEMY_DEFENSE,
	};

	struct ATTRIBUTES
	{
		float MAX_HEALTH;
		float HEALTH;
		int ATTACK;
		int DEFENSE;
	};

protected:
	Mesh* modelMesh;

	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 target, up;
	Vector3 maxBoundary, minBoundary;
	GroundEntity* m_pTerrain;

	double m_dSpeed;
	double m_dAcceleration;
	float m_fElapsedTimeBeforeUpdate;

public:
	Patrol(Mesh* _modelMesh);
	virtual ~Patrol();

	void Init(void);
	// Reset this player instance to default
	void Reset(void);

	// Set position
	void SetPos(const Vector3& pos);
	// Set target
	void SetTarget(const Vector3& target);
	// Set Up
	void SetUp(const Vector3& up);
	// Set the boundary for the player info
	void SetBoundary(Vector3 max, Vector3 min);
	// Set the terrain for the player info
	void SetTerrain(GroundEntity* m_pTerrain);

	// Get position
	Vector3 GetPos(void) const;
	// Get target
	Vector3 GetTarget(void) const;
	// Get Up
	Vector3 GetUp(void) const;
	// Get the terrain for the player info
	GroundEntity* GetTerrain(void);

	// Update
	void Update(double dt = 0.0333f);

	// Constrain the position within the borders
	void Constrain(void);
	// Render
	void Render(void);

	/*Set Health*/
	void setHealth(int _health);
	/*Get Health*/
	int getHealth(void);

	/*Set A.I State*/
	void setState(AI_STATE _state);
	/*Get A.I State*/
	int getState(void);

	/*Check if player is inside defined boundary around enemy.*/
	bool checkInsideBoundary(Vector3 minBoundary, Vector3 maxBoundary);

	/*Return nearest enemy*/
	CEnemy3D* returnNearestEnemy(void);

	/*Set Alert Boundary*/
	void setAlertBoundary(Vector3 _minAlertBoundary, Vector3 _maxAlertBoundary);
	/*Get Min Alert Boundary*/
	Vector3 getMinAlertBoundary(void);
	/*Get Max Alert Boundary*/
	Vector3 getMaxAlertBoundary(void);

	/*Set Portable DT*/
	void setPortableDT(double _portableDT);

	/*Get Portable DT*/
	double getPortableDT(void);

	/*Set Attributes*/
	void setAttributes(ATTRIBUTES _attributes);

	/*Get Attributes*/
	float getAttributes(ATTRIBUTE_TYPES type);

	/*Set Player Property*/
	void setPlayerProperty(bool _playerProperty);

	/*Get Player Property*/
	bool getPlayerProperty(void);

	/*Set Shoot Delay*/
	void setShootDelay(float _shootDelay);

	/*Get Player Property*/
	float getShootDelay(void);

	/*Render Health Bar*/
	void renderHealthBar(void);
private:
	ATTRIBUTES attributes;
	int health;
	AI_STATE state;
	Vector3 minAlertBoundary, maxAlertBoundary;
	double portableDT;
	float angleToFace;
	/*Check if enemy belongs to player.*/
	bool playerProperty;
	float shootDelay;
};

namespace Create
{
	CEnemy3D* Patrol(const std::string& _meshName,
		const Vector3& _position,
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));
};
