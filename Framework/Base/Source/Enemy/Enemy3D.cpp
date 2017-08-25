#include "Enemy3D.h"
#include "../Object/Furniture.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"
#include "../Projectile/Projectile.h"
/*Debugging use; cout when key was pressed.*/
#include "KeyboardController.h"
#include "../SoundEngine.h"

/*Get Application Width and Height*/
#include "../Application.h"


#include "GL\glew.h"

CEnemy3D::CEnemy3D(Mesh* _modelMesh)
	: GenericEntity(NULL)
	, defaultPosition(Vector3(0.0f,0.0f,0.0f))
	, defaultTarget(Vector3(0.0f, 0.0f, 0.0f))
	, defaultUp(Vector3(0.0f, 0.0f, 0.0f))
	, target(Vector3(0.0f, 0.0f, 0.0f))
	, up(Vector3(0.0f, 0.0f, 0.0f))
	, maxBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, minBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, minAlertBoundary(Vector3(0.f, 0.f, 0.f))
	, maxAlertBoundary(Vector3(0.f, 0.f, 0.f))
	, m_pTerrain(NULL)
	, m_fElapsedTimeBeforeUpdate(3.0f)
	, health(3)
	, state(IDLE)
	, playerProperty(false)
	, whoCloser(NONE)
	, previousPosition(0.f, 0.f, 0.f)
{
	this->modelMesh = _modelMesh;
}


CEnemy3D::~CEnemy3D()
{
}

void CEnemy3D::Init(void)
{
	// Set the default values
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set the current values
	//position.Set(10.0f, 0.0f, 0.0f);
	//target.Set(10.0f, 0.0f, 450.0f);
	//up.Set(0.0f, 1.0f, 0.0f);
	position.Set(position.x, position.y, position.z);
	cout << "Init Position : " << position << endl;
	target.Set(10.0f, 0.0f, 450.0f);
	up.Set(0.0f, 1.0f, 0.0f);

	// Set Boundary
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);

	// Set speed
	m_dSpeed = 10.0;

	/*Set State*/
	state = IDLE;

	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this);
	type = 0;
}

// Reset this player instance to default
void CEnemy3D::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

// Set position
void CEnemy3D::SetPos(const Vector3& pos)
{
	position = pos;
}

// Set target
void CEnemy3D::SetTarget(const Vector3& target)
{
	this->target = target;
}
// Set Up
void CEnemy3D::SetUp(const Vector3& up)
{
	this->up = up;
}
// Set the boundary for the player info
void CEnemy3D::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}
// Set the terrain for the player info
void CEnemy3D::SetTerrain(GroundEntity* m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;

		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
	}
}

void CEnemy3D::setDefaultPosition(Vector3 _defaultPosition)
{
	defaultPosition = _defaultPosition;
}

// Get position
Vector3 CEnemy3D::GetPos(void) const
{
	return position;
}

// Get target
Vector3 CEnemy3D::GetTarget(void) const
{
	return target;
}
// Get Up
Vector3 CEnemy3D::GetUp(void) const
{
	return up;
}
// Get the terrain for the player info
GroundEntity* CEnemy3D::GetTerrain(void)
{
	return m_pTerrain;
}

Vector3 CEnemy3D::getDefaultPosition(void)
{
	return defaultPosition;
}

// Update
void CEnemy3D::Update(double dt)
{
	setPortableDT(dt);
	switch (state)
	{
		case IDLE:
		{
			/*Real-time loop control, check if player is within boundary every 1 second.*/
			m_fElapsedTimeBeforeUpdate += dt;
			if (m_fElapsedTimeBeforeUpdate > 1.0f)
				m_fElapsedTimeBeforeUpdate = 0.0f;

			if (m_fElapsedTimeBeforeUpdate == 0.f)
			{
				/*Scan for player, if player is inside the defined boundary, activate the enemy.*/
				if (checkInsideBoundary(minAlertBoundary, maxAlertBoundary))
					state = ALERT;
			}
			break;
		}
		case ALERT:
		{
			if (!checkInsideBoundary(minAlertBoundary, maxAlertBoundary))
				state = IDLE;

			if (this->getPlayerProperty() && returnNearestEnemy() == nullptr)
				state = IDLE;

			/*Offset the y to make it seem as if bullet is hitting face on.*/
			Vector3 targetLower(CPlayerInfo::GetInstance()->GetPos().x, CPlayerInfo::GetInstance()->GetPos().y, CPlayerInfo::GetInstance()->GetPos().z);
			Vector3 viewVector = (targetLower - position).Normalized();
			Vector3 distanceBetween = CPlayerInfo::GetInstance()->GetPos() - GetPos();

			if (this->getPlayerProperty() && returnNearestEnemy() != nullptr)
			{
				/*Offset the y to make it seem as if bullet is hitting face on.*/
				/*Firing at enemies.*/
				targetLower.Set(returnNearestEnemy()->GetPos().x, returnNearestEnemy()->GetPos().y + 5.f, returnNearestEnemy()->GetPos().z);
				viewVector = (targetLower - position).Normalized();
				distanceBetween = returnNearestEnemy()->GetPos() - GetPos();
			}
			else if (!this->getPlayerProperty() && returnNearestEnemy() != nullptr && whoCloser == ENEMY)
			{
				/*Offset the y to make it seem as if bullet is hitting face on.*/
				/*Firing at enemies that converted to player.*/
				targetLower.Set(returnNearestEnemy()->GetPos().x, returnNearestEnemy()->GetPos().y + 5.f, returnNearestEnemy()->GetPos().z);
				viewVector = (targetLower - position).Normalized();
				distanceBetween = returnNearestEnemy()->GetPos() - GetPos();
			}

			shootDelay += static_cast<float>(dt);


			/*Change Y value to spawn bullet at correct position.*/
			Vector3 newPosition((distanceBetween.x / 2.f) + this->position.x, (distanceBetween.y / 2.f) + this->position.y, (distanceBetween.z / 2.f) + this->position.z);
			/*Aim the player.*/
			if (this->getPlayerProperty() || !this->getPlayerProperty() && whoCloser == ENEMY)
				newPosition.Set((distanceBetween.x / 2.f) + this->position.x, (distanceBetween.y / 2.f) + this->position.y + 2.5f, (distanceBetween.z / 2.f) + this->position.z);
			//else if (!this->getPlayerProperty() && whoCloser == ENEMY)
			//	newPosition.Set((distanceBetween.x / 2.f) + this->position.x, (distanceBetween.y / 2.f) + this->position.y + 2.5f, (distanceBetween.z / 2.f) + this->position.z);
			//if (KeyboardController::GetInstance()->IsKeyPressed('M'))
			//{
			//	cout << "Target: " << target << endl;
			//	cout << "Enemy Position: " << position << endl;
			//	cout << "Distance Between: " << distanceBetween << endl;
			//	cout << "New Position: " << newPosition << endl;
			//	cout << "Player Position: " << CPlayerInfo::GetInstance()->GetPos() << endl;
			//}

			if (this->getShootDelay() > 0.2f && GetAttribute(CAttributes::TYPE_HEALTH)> 0)
			{
				/*Randomise X value by 0.1f so that the trajectory will not be always fixed.*/
				CProjectile* _bullet = Create::Projectile("sphere", newPosition, Vector3(Math::RandFloatMinMax(viewVector.x - 0.1f, viewVector.x + 0.1f), viewVector.y, viewVector.z), 2.f, 100.f, NULL);
				_bullet->bulletOriginated = CProjectile::FROM_ENEMY;
				CSoundEngine::GetInstance()->PlayASound("ASSAULT");
				shootDelay = 0.f;
			}

			// Constrain the position
			//Constrain();

			break;
		}
		case DEAD:
		{
			CPlayerInfo::GetInstance()->setScore(CPlayerInfo::GetInstance()->getScore() + 100);
			int _KO_Count = CPlayerInfo::GetInstance()->getKO_Count() + 1;
			CPlayerInfo::GetInstance()->setKO_Count(_KO_Count);

			CSoundEngine::GetInstance()->PlayASound("EXPLODE");

			/*Converting Enemy to Player vice versa.*/
			if (!getPlayerProperty())
				setPlayerProperty(true);
			else
				setPlayerProperty(false);

			state = RECOVERY;
			m_fElapsedTimeBeforeUpdate = 0.f;
			angleToFace = 0.f;
			break;
		}
		case RECOVERY:
		{
			m_fElapsedTimeBeforeUpdate += dt;
			if (m_fElapsedTimeBeforeUpdate > 1.0f)
			{
				health = 3;
				setHealthTo(10.f);
				state = IDLE;
				m_fElapsedTimeBeforeUpdate = 0.f;
			}
			break;
		}
	}
	if (GetAttribute(CAttributes::TYPE_HEALTH) <= 0 && state != RECOVERY)
		state = DEAD;
	//if (enemyBehavior == ENEMY_BEHAVIOR::TOWER)
	//	updateTower(dt);
	//else if (enemyBehavior == ENEMY_BEHAVIOR::PATROL)
	//	updatePatrol(dt);
}

// Constrain the position within the borders
void CEnemy3D::Constrain(void)
{
	// Constrain player within the boundary
	if (position.x > maxBoundary.x - 1.0f)
		position.x = maxBoundary.x - 1.0f;
	if (position.z > maxBoundary.z - 1.0f)
		position.z = maxBoundary.z - 1.0f;
	if (position.x < minBoundary.x + 1.0f)
		position.x = minBoundary.x + 1.0f;
	if (position.z < minBoundary.z + 1.0f)
		position.z = minBoundary.z + 1.0f;

	/*If Y-position is not equal to terrain height at position, 
	update Y-position to the terrain height*/
	/*Commented out to prevent the Y from changing.*/
	//if (position.y != m_pTerrain->GetTerrainHeight(position))
	//	position.y = m_pTerrain->GetTerrainHeight(position);
}

// Render
void CEnemy3D::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);


	if (state == ALERT)
	{
		if (!getPlayerProperty())
		{
			if (whoCloser == PLAYER)
				angleToFace = Math::RadianToDegree(atan2(CPlayerInfo::GetInstance()->GetPos().x - this->position.x, CPlayerInfo::GetInstance()->GetPos().z - this->position.z));
			else if (whoCloser == ENEMY && returnNearestEnemy() != nullptr)
				angleToFace = Math::RadianToDegree(atan2(returnNearestEnemy()->GetPos().x - this->position.x, returnNearestEnemy()->GetPos().z - this->position.z));
		}
		else if (returnNearestEnemy() != nullptr)
			angleToFace = Math::RadianToDegree(atan2(returnNearestEnemy()->GetPos().x - this->position.x, returnNearestEnemy()->GetPos().z - this->position.z));
		modelStack.Rotate(angleToFace, 0.f, 1.f, 0.f);
	}
	if (state == RECOVERY)
	{
		angleToFace += static_cast<float>(getPortableDT() * 20.f);
		if (angleToFace >= 90.f)
			angleToFace = 90.f;
		modelStack.Rotate(-angleToFace, 1.f, 0.f, 0.f);
	}
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (!light)
		RenderHelper::RenderMesh(modelMesh);
	else
		RenderHelper::RenderMeshWithLight(modelMesh);
	modelStack.PopMatrix();

	if (GetAttribute(CAttributes::TYPE_HEALTH) > 0.f)
		renderHealthBar();
}

void CEnemy3D::setHealth(int _health)
{
	health = _health;
}

int CEnemy3D::getHealth(void)
{
	return health;
}

void CEnemy3D::setState(AI_STATE _state)
{
	state = _state;
}

int CEnemy3D::getState(void)
{
	return state;
}

bool CEnemy3D::checkInsideBoundary(Vector3 minBoundary, Vector3 maxBoundary)
{
	Vector3 thisWithoutY(defaultPosition.x, -10.f, defaultPosition.z);
	Vector3 playerWithoutY(CPlayerInfo::GetInstance()->GetPos().x, -10.f, CPlayerInfo::GetInstance()->GetPos().z);
	Vector3 enemyWithoutY(0.f, 0.f, 0.f);
	if (returnNearestEnemy() != nullptr)
		enemyWithoutY = returnNearestEnemy()->GetPos();


	Vector3 boundaryMin = Vector3(minBoundary.x + thisWithoutY.x, -10.f, minBoundary.z + thisWithoutY.z);
	Vector3 boundaryMax = Vector3(maxBoundary.x + thisWithoutY.x, -10.f, maxBoundary.z + thisWithoutY.z);

	if (!this->getPlayerProperty())
	{
		// << "Enemy to Enemy Player Distance: " << Vector3(enemyWithoutY.x - thisWithoutY.x, -10.f, enemyWithoutY.z - thisWithoutY.z).LengthSquared() << endl;
		// << "Enemy to Player Distance: " << Vector3(playerWithoutY.x - thisWithoutY.x, -10.f, playerWithoutY.z - thisWithoutY.z).LengthSquared() << endl;

		//cout << "Enemy Address: " << this << endl;
		if (EntityManager::GetInstance()->returnEnemy().size() < 2)
		{
			Vector3 playerMin = Vector3(CPlayerInfo::GetInstance()->GetMinAABB().x + playerWithoutY.x, -10.f, CPlayerInfo::GetInstance()->GetMinAABB().z + playerWithoutY.z);
			Vector3 playerMax = Vector3(CPlayerInfo::GetInstance()->GetMaxAABB().x + playerWithoutY.x, -10.f, CPlayerInfo::GetInstance()->GetMaxAABB().z + playerWithoutY.z);

			//cout << "Player Closer than Enemy with size < 2" << this << endl;
			if (boundaryMin <= playerMax && boundaryMax >= playerMin)
			{
				whoCloser = PLAYER;
				return true;
			}
			else
				return false;
		}
		else if (returnNearestEnemy() != nullptr)
		{
			Vector3 displacementEnemy(returnNearestEnemy()->GetPos().x - position.x, -10.f, returnNearestEnemy()->GetPos().z - position.z);
			Vector3 displacementPlayer(CPlayerInfo::GetInstance()->GetPos().x - position.x, -10.f, CPlayerInfo::GetInstance()->GetPos().z - position.z);

			if (displacementEnemy.LengthSquared() < displacementPlayer.LengthSquared() && returnNearestEnemy()->getPlayerProperty())
			{
				//cout << "Enemy Closer Than Player: " << this << endl;
				//cout << "Nearest Enemy: " << (returnNearestEnemy()->GetPos() - this->GetPos()).LengthSquared() << " Player: " << (CPlayerInfo::GetInstance()->GetPos() - this->GetPos()).LengthSquared() << " " << returnNearestEnemy()->getPlayerProperty()  <<endl;
				Vector3 enemyMin = Vector3(returnNearestEnemy()->GetMinAABB().x + returnNearestEnemy()->GetPos().x, -10.f, returnNearestEnemy()->GetMinAABB().z + returnNearestEnemy()->GetPos().z);
				Vector3 enemyMax = Vector3(returnNearestEnemy()->GetMaxAABB().x + returnNearestEnemy()->GetPos().x, -10.f, returnNearestEnemy()->GetMaxAABB().z + returnNearestEnemy()->GetPos().z);


				//cout << "Enemy Without Y: " << enemyWithoutY << endl;
				//cout << boundaryMin << " <= " << enemyMax << " && " << boundaryMax << " >= " << enemyMin << endl;
				if (boundaryMin <= enemyMax && boundaryMax >= enemyMin)
				{
					//cout << "Set Who Closer to Enemy" << endl;
					whoCloser = ENEMY;
					return true;
				}
				else
					return false;
			}
		}
		else
		{
			//cout << "Player Closer than Enemy" << this << endl;
			//cout << "Nearest Enemy: " << (returnNearestEnemy()->GetPos() - this->GetPos()).LengthSquared() << " Player: " << (CPlayerInfo::GetInstance()->GetPos() - this->GetPos()).LengthSquared() << " " << returnNearestEnemy()->getPlayerProperty()<<  endl;
			Vector3 playerMin = Vector3(CPlayerInfo::GetInstance()->GetMinAABB().x + playerWithoutY.x, -10.f, CPlayerInfo::GetInstance()->GetMinAABB().z + playerWithoutY.z);
			Vector3 playerMax = Vector3(CPlayerInfo::GetInstance()->GetMaxAABB().x + playerWithoutY.x, -10.f, CPlayerInfo::GetInstance()->GetMaxAABB().z + playerWithoutY.z);

			if (boundaryMin <= playerMax && boundaryMax >= playerMin)
			{
				//cout << "Set Who Closer to Player" << endl;
				whoCloser = PLAYER;
				return true;
			}
			else
				return false;
		}
	}
	else
	{
		if (returnNearestEnemy() == nullptr)
			return false;

		whoCloser = ENEMY;
		Vector3 enemyMin = Vector3(returnNearestEnemy()->GetMinAABB().x + enemyWithoutY.x, -10.f, returnNearestEnemy()->GetMinAABB().z + enemyWithoutY.z);
		Vector3 enemyMax = Vector3(returnNearestEnemy()->GetMaxAABB().x + enemyWithoutY.x, -10.f, returnNearestEnemy()->GetMaxAABB().z + enemyWithoutY.z);

		if (boundaryMin <= enemyMax && boundaryMax >= enemyMin)
			return true;

		else
			return false;
	}
}

CEnemy3D * CEnemy3D::returnNearestEnemy(void)
{
	CEnemy3D* enemy = nullptr;
	float nearestDistance = 0.f;
	Vector3 thisWithoutY(position.x, -10.f, position.z);

	if (this->getPlayerProperty())
	{
		for (list<CEnemy3D*>::iterator it = EntityManager::GetInstance()->returnEnemy().begin(); it != EntityManager::GetInstance()->returnEnemy().end(); ++it)
		{
			if (this == (*it))
				continue;

			if ((*it)->getPlayerProperty())
				continue;

			Vector3 enemyWithoutY((*it)->GetPos().x, -10.f, (*it)->GetPos().z);

			/*cout << "From " << this << " aim " << (CEnemy3D*)*it << endl;*/
			nearestDistance = (Vector3(enemyWithoutY.x - thisWithoutY.x, -10.f, enemyWithoutY.z - thisWithoutY.z)).LengthSquared();
			enemy = (CEnemy3D*)*it;
			break;
		}

		for (list<CEnemy3D*>::iterator it = EntityManager::GetInstance()->returnEnemy().begin(); it != EntityManager::GetInstance()->returnEnemy().end(); ++it)
		{
			if (this == (*it))
				continue;

			if ((*it)->getPlayerProperty())
				continue;

			Vector3 enemyWithoutY((*it)->GetPos().x, -10.f, (*it)->GetPos().z);

			if ((Vector3(enemyWithoutY.x - thisWithoutY.x, -10.f, enemyWithoutY.z - thisWithoutY.z)).LengthSquared() < nearestDistance)
			{
				enemy = (CEnemy3D*)*it;
				nearestDistance = (enemyWithoutY - thisWithoutY).LengthSquared();
			}
			else
				continue;
		}
	}
	else
	{
		for (list<CEnemy3D*>::iterator it = EntityManager::GetInstance()->returnEnemy().begin(); it != EntityManager::GetInstance()->returnEnemy().end(); ++it)
		{
			if (this == (*it))
				continue;

			if (!(*it)->getPlayerProperty())
				continue;

			Vector3 enemyWithoutY((*it)->GetPos().x, -10.f, (*it)->GetPos().z);

			/*cout << "From " << this << " aim " << (CEnemy3D*)*it << endl;*/
			nearestDistance = (Vector3(enemyWithoutY.x - thisWithoutY.x, -10.f, enemyWithoutY.z - thisWithoutY.z)).LengthSquared();
			enemy = (CEnemy3D*)*it;
			break;
		}

		for (list<CEnemy3D*>::iterator it = EntityManager::GetInstance()->returnEnemy().begin(); it != EntityManager::GetInstance()->returnEnemy().end(); ++it)
		{
			if (this == (*it))
				continue;

			if (!(*it)->getPlayerProperty())
				continue;

			Vector3 enemyWithoutY((*it)->GetPos().x, -10.f, (*it)->GetPos().z);

			if ((Vector3(enemyWithoutY.x - thisWithoutY.x, -10.f, enemyWithoutY.z - thisWithoutY.z)).LengthSquared() < nearestDistance)
			{
				enemy = (CEnemy3D*)*it;
				nearestDistance = (enemyWithoutY - thisWithoutY).LengthSquared();
			}
			else
				continue;
		}
	}
	return enemy;
}

void CEnemy3D::setAlertBoundary(Vector3 _minAlertBoundary, Vector3 _maxAlertBoundary)
{
	minAlertBoundary = _minAlertBoundary;
	maxAlertBoundary = _maxAlertBoundary;
}

Vector3 CEnemy3D::getMinAlertBoundary(void)
{
	return minAlertBoundary;
}

Vector3 CEnemy3D::getMaxAlertBoundary(void)
{
	return maxAlertBoundary;
}

void CEnemy3D::setPortableDT(double _portableDT)
{
	portableDT = _portableDT;
}

double CEnemy3D::getPortableDT(void)
{
	return portableDT;
}

//void CEnemy3D::setAttributes(ATTRIBUTES _attributes)
//{
//	attributes = _attributes;
//}
//
//float CEnemy3D::getAttributes(ATTRIBUTE_TYPES type)
//{
//	if (type == ENEMY_HEALTH)
//		return attributes.HEALTH;
//
//	else if (type == ENEMY_ATTACK)
//		return attributes.ATTACK;
//
//	else if (type == ENEMY_DEFENSE)
//		return attributes.DEFENSE;
//}

void CEnemy3D::setPlayerProperty(bool _playerProperty)
{
	playerProperty = _playerProperty;
}

bool CEnemy3D::getPlayerProperty(void)
{
	return playerProperty;
}

void CEnemy3D::setShootDelay(float _shootDelay)
{
	shootDelay = _shootDelay;
}

float CEnemy3D::getShootDelay(void)
{
	return shootDelay;
}

void CEnemy3D::renderHealthBar(void)
{
	/*Calculate the displacement from enemy to player.*/
	Vector3 displacement(CPlayerInfo::GetInstance()->GetPos() - this->GetPos());
	/*Calculate the displacement from player to enemy.*/
	Vector3 furtherDisplacement(this->GetPos() - CPlayerInfo::GetInstance()->GetPos());
	/*Move the position slightly further away to prevent Z-Axis fighting.*/
	furtherDisplacement *= 1.01f;
	/*Get the player position.*/
	Vector3 playerPosition(CPlayerInfo::GetInstance()->GetPos());
	/*Add the displacement with player position to bring the further than displacement Vector3 to prevent Z-Axis fighting.*/
	furtherDisplacement = furtherDisplacement + playerPosition;

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	/*Black health bar that depicts the total health of enemy.*/
	modelStack.PushMatrix();
	/*Keep the health bar fixed to the left of the enemy.*/
	modelStack.Translate(furtherDisplacement.x, furtherDisplacement.y + (maxAABB.y - furtherDisplacement.y), furtherDisplacement.z);
	modelStack.Rotate(Math::RadianToDegree(atan2f(displacement.x, displacement.z)), 0.f, 1.f, 0.f);
	/*Scale it according to the health left.*/
	modelStack.Scale(MAX_HEALTH_SCALE, Application::GetInstance().GetWindowHeight() * 0.005f, 0.000001f);
	MeshBuilder::GetInstance()->GenerateCube("cube", Color(0.f, 0.f, 0.0f), 1.f);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("cube"));
	modelStack.PopMatrix();

	/*Health Bar above enemy head*/
	modelStack.PushMatrix();
	/*Keep the health bar fixed to the left of the enemy.*/
	modelStack.Translate(position.x/*+ minAABB.x + ((attributes.HEALTH / attributes.MAX_HEALTH) * MAX_HEALTH_SCALE) * 0.5f*/, position.y + (maxAABB.y - position.y), position.z);
	/*Vector3 displacement(CPlayerInfo::GetInstance()->GetPos() - this->GetPos());*/
	modelStack.Rotate(Math::RadianToDegree(atan2f(displacement.x, displacement.z)), 0.f, 1.f, 0.f);
	/*Scale it according to the health left.*/
	modelStack.Scale((GetAttribute(CAttributes::TYPE_HEALTH) / GetAttribute(CAttributes::TYPE_MAXHEALTH)) * MAX_HEALTH_SCALE, Application::GetInstance().GetWindowHeight() * 0.005f, 0.000001f);

	/*Set health bar to green colour before damage.*/
	if (GetAttribute(CAttributes::TYPE_HEALTH) / GetAttribute(CAttributes::TYPE_MAXHEALTH) == 1)
		MeshBuilder::GetInstance()->GenerateCube("cube", Color(0.f, 1.f, 0.0f), 1.f);

	/*Set health bar to yellow color when taken damage and is above 20%.*/
	if (GetAttribute(CAttributes::TYPE_HEALTH) / GetAttribute(CAttributes::TYPE_MAXHEALTH) > 0.2f && GetAttribute(CAttributes::TYPE_HEALTH) / GetAttribute(CAttributes::TYPE_MAXHEALTH) < 1.f)
		MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.f, 1.f, 0.0f), 1.f);

	/*Set health bar to red color when health is 20% and below.*/
	if (GetAttribute(CAttributes::TYPE_HEALTH) / GetAttribute(CAttributes::TYPE_MAXHEALTH) <= 0.2f)
		MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.f, 0.f, 0.0f), 1.f);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("cube"));

	modelStack.PopMatrix();
}

bool CEnemy3D::checkCollision(void)
{
	/*Boundary was multiplied by 2.f in order to factor in the boundary that the point does not have.*/
	list<CEnemy3D*>enemyList = EntityManager::GetInstance()->returnEnemy();
	list<CFurniture*>fixedList = EntityManager::GetInstance()->returnFixed();
	Vector3 positionWithoutYMin(position.x + GetMinAABB().x, -10.f, position.z + GetMinAABB().z);
	Vector3 positionWithoutYMax(position.x + GetMaxAABB().x, -10.f, position.z + GetMaxAABB().z);

	for (list<CEnemy3D*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
	{
		CEnemy3D* enemy = (CEnemy3D*)*it;

		if (this == enemy)
			continue;

		//Vector3 minBoundary = enemy->GetPos() + (enemy->GetMinAABB() * 2.f);
		//Vector3 maxBoundary = enemy->GetPos() + (enemy->GetMaxAABB() * 2.f);

		Vector3 minBoundary(enemy->GetPos().x + enemy->GetMinAABB().x, -10.f, enemy->GetPos().z + enemy->GetMinAABB().z);
		Vector3 maxBoundary(enemy->GetPos().x + enemy->GetMaxAABB().x, -10.f, enemy->GetPos().z + enemy->GetMaxAABB().z);
																			
		//Vector3 positionWithoutYMin(position.x + GetMinAABB().x, -10.f, position.z + GetMinAABB().z);
		//Vector3 positionWithoutYMax(position.x + GetMaxAABB().x, -10.f, position.z + GetMaxAABB().z);

		if (positionWithoutYMax >= minBoundary && positionWithoutYMin <= maxBoundary)
			return true;
		else
			continue;
	}

	for (list<CFurniture*>::iterator it = fixedList.begin(); it != fixedList.end(); ++it)
	{
		CFurniture* object = (CFurniture*)*it;
	
		Vector3 minBoundary(object->GetPosition().x + object->GetMinAABB().x, -10.f, object->GetPosition().z + object->GetMinAABB().z);
		Vector3 maxBoundary(object->GetPosition().x + object->GetMaxAABB().x, -10.f, object->GetPosition().z + object->GetMaxAABB().z);

		//Vector3 minBoundary = object->GetPosition() + (object->GetMinAABB() * 2.f);
		//Vector3 maxBoundary = object->GetPosition() + (object->GetMaxAABB() * 2.f);

		if (positionWithoutYMax >= minBoundary && positionWithoutYMin <= maxBoundary)
			return true;
		else
			continue;
	}

	Vector3 playerMin(CPlayerInfo::GetInstance()->GetPos().x + CPlayerInfo::GetInstance()->GetMinAABB().x, -10.f, CPlayerInfo::GetInstance()->GetPos().z + CPlayerInfo::GetInstance()->GetMinAABB().z);
	Vector3 playerMax(CPlayerInfo::GetInstance()->GetPos().x + CPlayerInfo::GetInstance()->GetMaxAABB().x, -10.f, CPlayerInfo::GetInstance()->GetPos().z + CPlayerInfo::GetInstance()->GetMaxAABB().z);
	//Vector3 playerMin = CPlayerInfo::GetInstance()->GetMinAABB() + CPlayerInfo::GetInstance()->GetPos();
	//Vector3 playerMax = CPlayerInfo::GetInstance()->GetMaxAABB() + CPlayerInfo::GetInstance()->GetPos();

	//cout << position << " >= " << playerMin << " && " << position << " <= " << playerMax << endl;
	if (positionWithoutYMax >= playerMin && positionWithoutYMin <= playerMax)
		return true;

	return false;
}

void CEnemy3D::SetWhoCloser(WHO_CLOSER _whoCloser)
{
	whoCloser = _whoCloser;
}

CEnemy3D::WHO_CLOSER CEnemy3D::GetWhoCloser(void)
{
	return whoCloser;
}

void CEnemy3D::SetElapsedTime(float _m_fElapsedTimeBeforeUpdate)
{
	m_fElapsedTimeBeforeUpdate = _m_fElapsedTimeBeforeUpdate;
}

float CEnemy3D::GetElapsedTime(void)
{
	return m_fElapsedTimeBeforeUpdate;
}

void CEnemy3D::SetPreviousPosition(Vector3 _previousPosition)
{
	previousPosition = _previousPosition;
}

Vector3 CEnemy3D::GetPreviousPosition(void)
{
	return previousPosition;
}

CEnemy3D* Create::Enemy3D(const std::string& _meshName,
						const Vector3& _position,
						const Vector3& _scale,
						const bool& _playerProperty)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	CEnemy3D* result = new CEnemy3D(modelMesh);

	//cout << "Position in Create: " << _position << endl;
	result->SetPosition(_position);
	result->setDefaultPosition(_position);
	result->SetPos(_position);
	result->SetScale(_scale);
	result->SetCollider(true);
	result->setPlayerProperty(_playerProperty);
	result->SetLight(true);
	result->SetAABB(_scale * 2.f, -_scale * 2.f);
	result->SetMinAABB(-_scale * 2.f);
	result->SetMaxAABB(_scale * 2.f);
	result->setAlertBoundary(Vector3(-150.f, -10.f, -150.f), Vector3(150.f, 10.f, 150.f));
	result->setMaxHealthTo(10.f);
	result->setHealthTo(10.f);
	result->setAttackTo(1.f);
	result->setDefenseTo(1.f);
	result->setType(3);
	EntityManager::GetInstance()->AddEnemy(result);
	return result;
}