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
	, m_fElapsedTimeBeforeUpdate(0.0f)
	, health(3)
	, state(IDLE)
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

			/*Offset the y to make it seem as if bullet is hitting face on.*/
			Vector3 targetLower(CPlayerInfo::GetInstance()->GetPos().x, CPlayerInfo::GetInstance()->GetPos().y, CPlayerInfo::GetInstance()->GetPos().z);
			Vector3 viewVector = (targetLower - position).Normalized();
			Vector3 distanceBetween = CPlayerInfo::GetInstance()->GetPos() - GetPos();

			static float shootDelay = 0.f;
			shootDelay += static_cast<float>(dt);

			/*Change Y value to spawn bullet at correct position.*/
			Vector3 newPosition((distanceBetween.x / 2.f) + this->position.x, (distanceBetween.y / 2.f) + this->position.y, (distanceBetween.z / 2.f) + this->position.z);

			//if (KeyboardController::GetInstance()->IsKeyPressed('M'))
			//{
			//	cout << "Target: " << target << endl;
			//	cout << "Enemy Position: " << position << endl;
			//	cout << "Distance Between: " << distanceBetween << endl;
			//	cout << "New Position: " << newPosition << endl;
			//	cout << "Player Position: " << CPlayerInfo::GetInstance()->GetPos() << endl;
			//}

			//if (shootDelay > 0.2f && health > 0)
			//{
			//	/*Randomise X value by 0.1f so that the trajectory will not be always fixed.*/
			//	Create::Projectile("sphere", newPosition, Vector3(Math::RandFloatMinMax(viewVector.x - 0.1f, viewVector.x + 0.1f), viewVector.y, viewVector.z), 2.f, 100.f, NULL);
			//	CSoundEngine::GetInstance()->PlayASound("ASSAULT");
			//	shootDelay = 0.f;
			//}

			// Constrain the position
			Constrain();

			break;
		}
		case DEAD:
		{
			CPlayerInfo::GetInstance()->setScore(CPlayerInfo::GetInstance()->getScore() + 100);
			CSoundEngine::GetInstance()->PlayASound("EXPLODE");
			state = RECOVERY;
			m_fElapsedTimeBeforeUpdate = 0.f;
			angleToFace = 0.f;
			break;
		}
		case RECOVERY:
		{
			m_fElapsedTimeBeforeUpdate += dt;
			if (m_fElapsedTimeBeforeUpdate > 10.0f)
			{
				health = 3;
				state = IDLE;
				m_fElapsedTimeBeforeUpdate = 0.f;
			}
			break;
		}
	}
	if (health <= 0 && state != RECOVERY)
		state = DEAD;
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
		angleToFace = Math::RadianToDegree(atan2(CPlayerInfo::GetInstance()->GetPos().x - this->position.x, CPlayerInfo::GetInstance()->GetPos().z - this->position.z));
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

	modelStack.PushMatrix();
	cout << "Position Y: " << position.y << endl;
	cout << "Max AABB.Y: " << maxAABB.y << endl;
	modelStack.Translate(position.x + minAABB.x + ((attributes.HEALTH / attributes.MAX_HEALTH) * MAX_HEALTH_SCALE) * 0.5f, position.y + (maxAABB.y - position.y), position.z);
	Vector3 displacement(CPlayerInfo::GetInstance()->GetPos() - this->GetPos());
	modelStack.Rotate(Math::RadianToDegree(atan2f(displacement.x, displacement.z)), 0.f, 1.f, 0.f);
	modelStack.Scale((attributes.HEALTH / attributes.MAX_HEALTH) * MAX_HEALTH_SCALE, Application::GetInstance().GetWindowHeight() * 0.005f, 1.f);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("cube"));
	modelStack.PopMatrix();
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

	Vector3 playerMin = CPlayerInfo::GetInstance()->GetMinAABB() + Vector3(CPlayerInfo::GetInstance()->GetPos().x, -5.f, CPlayerInfo::GetInstance()->GetPos().z);
	Vector3 playerMax = CPlayerInfo::GetInstance()->GetMaxAABB() + Vector3(CPlayerInfo::GetInstance()->GetPos().x, -5.f, CPlayerInfo::GetInstance()->GetPos().z);

	if ((boundaryMin.x < playerMax.x && boundaryMax.x >playerMin.x) &&
		(boundaryMin.y < playerMax.y && boundaryMax.y >playerMin.y) &&
		(boundaryMin.z < playerMax.z && boundaryMax.z >playerMin.z))
		return true;
	else
		return false;
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
	EntityManager::GetInstance()->AddEnemy(result);
	return result;
}