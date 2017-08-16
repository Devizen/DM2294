#include "Enemy3D.h"
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

// Update
void CEnemy3D::Update(double dt)
{
	setPortableDT(dt);

	if (enemyBehavior == ENEMY_BEHAVIOR::TOWER)
		updateTower(dt);
	else if (enemyBehavior == ENEMY_BEHAVIOR::PATROL)
		updatePatrol(dt);
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

	if (attributes.HEALTH > 0.f)
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
	Vector3 boundaryMin = minBoundary + GetPos();
	Vector3 boundaryMax = maxBoundary + GetPos();

	if (!this->getPlayerProperty())
	{
		cout << "Enemy Address: " << this << endl;
		if (EntityManager::GetInstance()->returnEnemy().size() < 2)
		{
			cout << "Lesser than Two: " << this << endl;
			Vector3 playerMin = CPlayerInfo::GetInstance()->GetMinAABB() + Vector3(CPlayerInfo::GetInstance()->GetPos().x, -5.f, CPlayerInfo::GetInstance()->GetPos().z);
			Vector3 playerMax = CPlayerInfo::GetInstance()->GetMaxAABB() + Vector3(CPlayerInfo::GetInstance()->GetPos().x, -5.f, CPlayerInfo::GetInstance()->GetPos().z);

			if ((boundaryMin.x < playerMax.x && boundaryMax.x >playerMin.x) &&
				(boundaryMin.y < playerMax.y && boundaryMax.y >playerMin.y) &&
				(boundaryMin.z < playerMax.z && boundaryMax.z >playerMin.z))
				return true;
			else
				return false;
		}
	
		else if ((returnNearestEnemy()->GetPos() - this->GetPos()).LengthSquared() < (CPlayerInfo::GetInstance()->GetPos() - this->GetPos()).LengthSquared() && returnNearestEnemy()->getPlayerProperty())
		{
			cout << "Enemy Closer Than Player: " << this << endl;
			whoCloser = ENEMY;
			if (returnNearestEnemy() == nullptr)
				return false;
			cout << "Nearest Enemy: " << (returnNearestEnemy()->GetPos() - this->GetPos()).LengthSquared() << " Player: " << (CPlayerInfo::GetInstance()->GetPos() - this->GetPos()).LengthSquared() << " " << returnNearestEnemy()->getPlayerProperty()  <<endl;
			cout << "I AM HERE" << endl;
			Vector3 objectMin = returnNearestEnemy()->GetMinAABB() + Vector3(returnNearestEnemy()->GetPos().x, -5.f, returnNearestEnemy()->GetPos().z);
			Vector3 objectMax = returnNearestEnemy()->GetMaxAABB() + Vector3(returnNearestEnemy()->GetPos().x, -5.f, returnNearestEnemy()->GetPos().z);

			if ((boundaryMin.x < objectMax.x && boundaryMax.x >objectMin.x) &&
				(boundaryMin.y < objectMax.y && boundaryMax.y >objectMin.y) &&
				(boundaryMin.z < objectMax.z && boundaryMax.z >objectMin.z))

			{
				cout << "ENEMY INSIDE BOUNDARY" << endl;
				return true;
			}

			else
				return false;
		}
		else
		{
			cout << "Player Closer than Enemy" << this << endl;
			cout << "Nearest Enemy: " << (returnNearestEnemy()->GetPos() - this->GetPos()).LengthSquared() << " Player: " << (CPlayerInfo::GetInstance()->GetPos() - this->GetPos()).LengthSquared() << " " << returnNearestEnemy()->getPlayerProperty()<<  endl;
			Vector3 playerMin = CPlayerInfo::GetInstance()->GetMinAABB() + Vector3(CPlayerInfo::GetInstance()->GetPos().x, -5.f, CPlayerInfo::GetInstance()->GetPos().z);
			Vector3 playerMax = CPlayerInfo::GetInstance()->GetMaxAABB() + Vector3(CPlayerInfo::GetInstance()->GetPos().x, -5.f, CPlayerInfo::GetInstance()->GetPos().z);

			whoCloser = PLAYER;
			if ((boundaryMin.x < playerMax.x && boundaryMax.x >playerMin.x) &&
				(boundaryMin.y < playerMax.y && boundaryMax.y >playerMin.y) &&
				(boundaryMin.z < playerMax.z && boundaryMax.z >playerMin.z))
				return true;
			else
				return false;
		}
	}
	else
	{
		if (returnNearestEnemy() == nullptr)
			return false;

		whoCloser = NONE;
		Vector3 objectMin = returnNearestEnemy()->GetMinAABB() + Vector3( returnNearestEnemy()->GetPos().x, -5.f,  returnNearestEnemy()->GetPos().z);
		Vector3 objectMax = returnNearestEnemy()->GetMaxAABB() + Vector3( returnNearestEnemy()->GetPos().x, -5.f,  returnNearestEnemy()->GetPos().z);

		if ((boundaryMin.x < objectMax.x && boundaryMax.x >objectMin.x) &&
			(boundaryMin.y < objectMax.y && boundaryMax.y >objectMin.y) &&
			(boundaryMin.z < objectMax.z && boundaryMax.z >objectMin.z))

			return true;

		else
			return false;
	}
}

CEnemy3D * CEnemy3D::returnNearestEnemy(void)
{
	CEnemy3D* enemy = nullptr;
	float nearestDistance = 0.f;
	for (list<CEnemy3D*>::iterator it = EntityManager::GetInstance()->returnEnemy().begin(); it != EntityManager::GetInstance()->returnEnemy().end(); ++it)
	{
		if (this == (*it))
			continue;

		if ((*it)->getPlayerProperty() && this->getPlayerProperty())
			continue;

		/*cout << "From " << this << " aim " << (CEnemy3D*)*it << endl;*/
		nearestDistance = ((*it)->GetPos() - this->GetPos()).LengthSquared();
		enemy = (CEnemy3D*)*it;
		break;
	}

	if (enemy == nullptr)
		return enemy;

	for (list<CEnemy3D*>::iterator it = EntityManager::GetInstance()->returnEnemy().begin(); it != EntityManager::GetInstance()->returnEnemy().end(); ++it)
	{
		/*enemy = (CEnemy3D*)*it;*/
		/*cout << "Next From " << this << " aim " << (CEnemy3D*)*it << " PLAYER? " << (*it)->getPlayerProperty() << endl;*/
		if ((*it)->getPlayerProperty() && this->getPlayerProperty())
			continue;

		if (((*it)->GetPos() - this->GetPos()).LengthSquared() < nearestDistance && this != (*it))
		{
			enemy = (CEnemy3D*)*it;
			nearestDistance = ((*it)->GetPos() - this->GetPos()).LengthSquared();
		}
		else
			continue;
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

void CEnemy3D::setAttributes(ATTRIBUTES _attributes)
{
	attributes = _attributes;
}

float CEnemy3D::getAttributes(ATTRIBUTE_TYPES type)
{
	if (type == ENEMY_HEALTH)
		return attributes.HEALTH;

	else if (type == ENEMY_ATTACK)
		return attributes.ATTACK;

	else if (type == ENEMY_DEFENSE)
		return attributes.DEFENSE;
}

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
	modelStack.Scale(MAX_HEALTH_SCALE, Application::GetInstance().GetWindowHeight() * 0.005f, 1.f);
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
	modelStack.Scale((attributes.HEALTH / attributes.MAX_HEALTH) * MAX_HEALTH_SCALE, Application::GetInstance().GetWindowHeight() * 0.005f, 1.f);

	/*Set health bar to green colour before damage.*/
	if (attributes.HEALTH / attributes.MAX_HEALTH == 1)
		MeshBuilder::GetInstance()->GenerateCube("cube", Color(0.f, 1.f, 0.0f), 1.f);

	/*Set health bar to yellow color when taken damage and is above 20%.*/
	if (attributes.HEALTH / attributes.MAX_HEALTH > 0.2f && attributes.HEALTH / attributes.MAX_HEALTH < 1.f)
		MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.f, 1.f, 0.0f), 1.f);

	/*Set health bar to red color when health is 20% and below.*/
	if (attributes.HEALTH / attributes.MAX_HEALTH <= 0.2f)
		MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.f, 0.f, 0.0f), 1.f);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("cube"));

	modelStack.PopMatrix();
}

void CEnemy3D::updatePatrol(double dt)
{
	//Vector3 displacement(waypoint[waypointToGo] - this->GetPos());
	//this->SetPos(this->GetPos() +  (displacement * (float)dt * 100.f));

	//for (vector<Vector3>::iterator it = waypoint.begin(); it != waypoint.end(); ++it)
	//{
	//	Vector3 waypointPosition = (Vector3)*it;

	//	cout << "Iterator: " << waypointPosition << endl;
	//}
	//for (int i = 0; i < waypoint.size(); ++i)
	//{
	//	cout << "For Loop " << to_string(i) << ": " << waypoint[i] << endl;
	//}

	//cout << waypointToGo << endl;
	//if (position == waypoint[waypointToGo])
	//	waypointToGo = waypointToGo == waypoint.size() ? 0 : ++waypointToGo;
	////}
	//cout << waypointToGo << endl;
}

void CEnemy3D::updateTower(double dt)
{
	switch (state)
	{
	case IDLE:
	{
		/*Real-time loop control, check if player is within boundary every 3 seconds.*/
		m_fElapsedTimeBeforeUpdate += dt;
		if (m_fElapsedTimeBeforeUpdate > 3.0f)
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
			targetLower.Set(returnNearestEnemy()->GetPos().x, returnNearestEnemy()->GetPos().y, returnNearestEnemy()->GetPos().z);
			viewVector = (targetLower - position).Normalized();
			distanceBetween = returnNearestEnemy()->GetPos() - GetPos();
		}
		else if (!this->getPlayerProperty() && returnNearestEnemy() != nullptr && whoCloser == ENEMY)
		{
			/*Offset the y to make it seem as if bullet is hitting face on.*/
			targetLower.Set(returnNearestEnemy()->GetPos().x, returnNearestEnemy()->GetPos().y, returnNearestEnemy()->GetPos().z);
			viewVector = (targetLower - position).Normalized();
			distanceBetween = returnNearestEnemy()->GetPos() - GetPos();
		}

		shootDelay += static_cast<float>(dt);


		/*Change Y value to spawn bullet at correct position.*/
		Vector3 newPosition((distanceBetween.x / 2.f) + this->position.x, (distanceBetween.y / 2.f) + this->position.y, (distanceBetween.z / 2.f) + this->position.z);

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

		if (this->getShootDelay() > 0.2f && this->attributes.HEALTH > 0)
		{
			/*Randomise X value by 0.1f so that the trajectory will not be always fixed.*/
			Create::Projectile("sphere", newPosition, Vector3(Math::RandFloatMinMax(viewVector.x - 0.1f, viewVector.x + 0.1f), viewVector.y, viewVector.z), 2.f, 100.f, NULL);
			CSoundEngine::GetInstance()->PlayASound("ASSAULT");
			shootDelay = 0.f;
		}

		// Constrain the position
		Constrain();

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
			attributes.HEALTH = 10;
			state = IDLE;
			m_fElapsedTimeBeforeUpdate = 0.f;
		}
		break;
	}
	}
	if (attributes.HEALTH <= 0 && state != RECOVERY)
		state = DEAD;
}

CEnemy3D* Create::Enemy3D(const std::string& _meshName,
						const Vector3& _position,
						const Vector3& _scale)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	CEnemy3D* result = new CEnemy3D(modelMesh);
	Vector3 newPosition(_position.x, _position.y - 10.f, _position.z);
	result->SetPosition(newPosition);
	result->SetScale(_scale);
	result->SetCollider(true);
	result->setPlayerProperty(false);
	EntityManager::GetInstance()->AddEnemy(result);
	return result;
}