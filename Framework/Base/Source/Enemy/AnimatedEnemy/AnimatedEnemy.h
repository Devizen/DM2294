#pragma once
#include "../Enemy3D.h"/*
#include "../../Attributes.h"*/
#include "../../GenericEntity.h"
#include <vector>

#define MAX_HEALTH_SCALE 10.f

using namespace std;

class Mesh;

class CAnimatedEnemy: public CEnemy3D
{
	friend class EntityManager;
	//friend class SceneText;

	enum ATTRIBUTE_TYPES
	{
		ENEMY_HEALTH = 0,
		ENEMY_ATTACK,
		ENEMY_DEFENSE,
	};
	
	enum WHO_CLOSER
	{
		PLAYER = 0,
		ENEMY,
		NONE
	};

	//struct ATTRIBUTES
	//{
	//	float MAX_HEALTH;
	//	float HEALTH;
	//	int ATTACK;
	//	int DEFENSE;
	//};

protected:
	/*
	0 - Torso/Core
	1- Left arm with respect from robot
	2- Right arm with respect from robot
	3- Left leg with respect from robot
	4- Right leg with respect from robot
	5- Head with respect from robot
	*/
	Mesh* modelMesh[6];

	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 target, up;
	Vector3 maxBoundary, minBoundary;
	GroundEntity* m_pTerrain;
	
	double m_dSpeed;
	double m_dAcceleration;
	//float m_fElapsedTimeBeforeUpdate;

public:
	CAnimatedEnemy(Mesh* _core,
					Mesh* _leftArm,
					Mesh* _rightArm,
					Mesh* _leftLeg,
					Mesh* _rightLeg,
					Mesh* _head);
	virtual ~CAnimatedEnemy();

	enum AI_STATE
	{
		IDLE = 0,
		ALERT,
		DEAD,
		RECOVERY,
		PATROL
	};
	virtual void Init(void);
	// Reset this player instance to default
	virtual void Reset(void);
	//// Set position
	//void SetPos(const Vector3& pos);
	//// Set target
	//void SetTarget(const Vector3& target);
	//// Set Up
	//void SetUp(const Vector3& up);
	//// Set the boundary for the player info
	//void SetBoundary(Vector3 max, Vector3 min);
	//// Set the terrain for the player info
	//void SetTerrain(GroundEntity* m_pTerrain);
	//// Get position
	//Vector3 GetPos(void) const;
	//// Get target
	//Vector3 GetTarget(void) const;
	//// Get Up
	//Vector3 GetUp(void) const;
	//// Get the terrain for the player info
	//GroundEntity* GetTerrain(void);

	// Update
	virtual void Update(double dt = 0.0333f);

	//// Constrain the position within the borders
	//void Constrain(void);

	// Render
	virtual void Render(void);

	/////*Set Health*/
	////void setHealth(int _health);
	/////*Get Health*/
	////int getHealth(void);
	/////*Set A.I State*/
	////void setState(AI_STATE _state);
	/////*Get A.I State*/
	////int getState(void);
	/////*Check if player is inside defined boundary around enemy.*/
	////virtual bool checkInsideBoundary(Vector3 minBoundary, Vector3 maxBoundary);
	/////*Return nearest enemy*/
	////CAnimatedEnemy* returnNearestEnemy(void);
	/////*Set Alert Boundary*/
	////void setAlertBoundary(Vector3 _minAlertBoundary, Vector3 _maxAlertBoundary);
	/////*Get Min Alert Boundary*/
	////Vector3 getMinAlertBoundary(void);
	/////*Get Max Alert Boundary*/
	////Vector3 getMaxAlertBoundary(void);
	/////*Set Portable DT*/
	////void setPortableDT(double _portableDT);
	/////*Get Portable DT*/
	//double getPortableDT(void);
	/////*Set Attributes*/
	////void setAttributes(ATTRIBUTES _attributes);
	/////*Get Attributes*/
	////float getAttributes(ATTRIBUTE_TYPES type);
	/////*Set Player Property*/
	////void setPlayerProperty(bool _playerProperty);
	/////*Get Player Property*/
	////bool getPlayerProperty(void);
	/////*Set Shoot Delay*/
	////virtual void setShootDelay(float _shootDelay);
	/////*Get Player Property*/
	////virtual float getShootDelay(void);
	/////*Render Health Bar*/
	//virtual void renderHealthBar(void);
	/////*Update for Patrol Behavior*/
	////void updatePatrol(double dt);
	/////*Update for Tower Behavior*/
	////void updateTower(double dt);
private:
	//ATTRIBUTES attributes;
	int health;
	AI_STATE state;
	Vector3 minAlertBoundary, maxAlertBoundary;
	double portableDT;
	float angleToFace;
	/////*Check if enemy belongs to player.*/
	bool playerProperty;
	float shootDelay;
	float m_fElapsedTimeBeforeUpdate;
	WHO_CLOSER whoCloser;
};

namespace Create
{
	CAnimatedEnemy* AnimatedEnemy(const std::string& _core,
						const std::string& _leftArm,
						const std::string& _rightArm,
						const std::string& _leftLeg,
						const std::string& _rightLeg,
						const std::string& _Head,
						const Vector3& _position,
						const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));
};