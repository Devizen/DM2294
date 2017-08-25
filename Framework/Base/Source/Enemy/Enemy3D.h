#pragma once
#include "../GenericEntity.h"
#include "../GroundEntity.h"
#include "Enemy_Type.h"
#include "../Attributes.h"
#include "Pathfinding\Pathfinding.h"
#include <vector>

#define MAX_HEALTH_SCALE 10.f

using namespace std;

class Mesh;

class CEnemy3D :
	public GenericEntity,
	public CAttributes,
	public Pathfinding
{
	friend class EntityManager;
	friend class SceneText;

	enum ATTRIBUTE_TYPES
	{
		ENEMY_HEALTH = 0,
		ENEMY_ATTACK,
		ENEMY_DEFENSE,
	};

	//struct ATTRIBUTES
	//{
	//	float MAX_HEALTH;
	//	float HEALTH;
	//	int ATTACK;
	//	int DEFENSE;
	//};
protected:
	Mesh* modelMesh;

	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 target, up;
	Vector3 maxBoundary, minBoundary;
	GroundEntity* m_pTerrain;
	
	double m_dSpeed;
	double m_dAcceleration;
	//float m_fElapsedTimeBeforeUpdate;

public:

	CEnemy3D(Mesh* _modelMesh);
	virtual ~CEnemy3D();

	enum WHO_CLOSER
	{
		PLAYER = 0,
		ENEMY,
		NONE
	};

	enum AI_STATE
	{
		IDLE = 0,
		ALERT,
		ATTACK,
		DEAD,
		RECOVERY,
		PATROL,
		NO_AI_STATE
	};


	virtual void Init(void);
	// Reset this player instance to default
	virtual void Reset(void);

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
	/*Set Default Position*/
	void setDefaultPosition(Vector3 _defaultPosition);

	// Get position
	Vector3 GetPos(void) const;
	// Get target
	Vector3 GetTarget(void) const;
	// Get Up
	Vector3 GetUp(void) const;
	// Get the terrain for the player info
	GroundEntity* GetTerrain(void);

	/*Get Default Position*/
	Vector3 getDefaultPosition(void);

	// Update
	virtual void Update(double dt = 0.0333f);

	// Constrain the position within the borders
	void Constrain(void);
	// Render
	virtual void Render(void);

	/*Set Health*/
	void setHealth(int _health);
	/*Get Health*/
	int getHealth(void);

	/*Set A.I State*/
	void setState(AI_STATE _state);
	/*Get A.I State*/
	int getState(void);

	/*Check if player is inside defined boundary around enemy.*/
	virtual bool checkInsideBoundary(Vector3 minBoundary, Vector3 maxBoundary);

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

	///*Set Attributes*/
	//void setAttributes(ATTRIBUTES _attributes);

	///*Get Attributes*/
	//float getAttributes(ATTRIBUTE_TYPES type);

	/*Set Player Property*/
	void setPlayerProperty(bool _playerProperty);

	/*Get Player Property*/
	bool getPlayerProperty(void);

	/*Set Shoot Delay*/
	virtual void setShootDelay(float _shootDelay);

	/*Get Player Property*/
	virtual float getShootDelay(void);

	/*Render Health Bar*/
	virtual void renderHealthBar(void);
	///*Update for Patrol Behavior*/
	//void updatePatrol(double dt);
	///*Update for Tower Behavior*/
	//void updateTower(double dt);

	/*Checking collision with enemy and object.*/
	bool checkCollision(void);

	/*Set who is closer, enemy (player) or player.*/
	void SetWhoCloser(WHO_CLOSER _whoCloser);
	/*Get who is closer, enemy (player) or player.*/
	WHO_CLOSER GetWhoCloser(void);

	Vector3 previousPosition;
	/*Set the Elapsed Time*/
	void SetElapsedTime(float _m_fElapsedTimeBeforeUpdate);
	/*Get the Elapsed Time*/
	float GetElapsedTime(void);

	/*Set Previous Position.*/
	void SetPreviousPosition(Vector3 _previousPosition);
	/*Get Previous Position.*/
	Vector3 GetPreviousPosition(void);

	//set type 
	void setType(int newType) { this->type = newType; }
	int getType() { return type; }

private:
	//ATTRIBUTES attributes;
	int health;
	AI_STATE state;
	Vector3 minAlertBoundary, maxAlertBoundary;
	double portableDT;
	float angleToFace;
	/*Check if enemy belongs to player.*/
	bool playerProperty;
	float shootDelay;
	float m_fElapsedTimeBeforeUpdate;
	WHO_CLOSER whoCloser;
	//type of enemy

	int type;

};

namespace Create
{
	CEnemy3D* Enemy3D(const std::string& _meshName,
						const Vector3& _position,
						const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
						const bool& _playerProperty = false);
};
