#include "Patrol.h"
#include "../../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"
#include "../../Application.h"

CPatrol::CPatrol(Mesh * _modelMesh) 
	: CEnemy3D(NULL)
	, minAlertBoundary(Vector3(-100.f, -50.f, -100.f))
	, maxAlertBoundary(Vector3(100.f, 50.f, 100.f))
	, m_fElapsedTimeBeforeUpdate(3.0f)
	, state(PATROL)
{
	this->modelMesh = _modelMesh;
}

CPatrol::~CPatrol()
{
}

void CPatrol::Init(void)
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

	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this);

}

// Reset this player instance to default
void CPatrol::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

// Update
void CPatrol::Update(double dt)
{
	cout << "Displacement: " << (CPlayerInfo::GetInstance()->GetPos() - this->GetPos()).LengthSquared() << endl;
	if ((CPlayerInfo::GetInstance()->GetPos() - this->GetPos()).LengthSquared() < 100.f * 100.f && checkInsideBoundary(minAlertBoundary, maxAlertBoundary))
		state = ALERT;
	else
		state = PATROL;

	switch (state)
	{
		case PATROL:
		{
			Vector3 displacement(waypoint[waypointToGo] - this->GetPos());
		
			position += displacement.Normalized() * (float)dt * 50.f;

			if (displacement.LengthSquared() <= 20.f)
				waypointToGo = ((waypointToGo == waypoint.size() - 1) ? 0 : ++waypointToGo);

			break;
		}
		case ALERT:
		{
			Vector3 displacement(CPlayerInfo::GetInstance()->GetPos() - this->GetPos());

			if (displacement.LengthSquared() > scale.LengthSquared())
				position += displacement.Normalized() * (float)dt * 50.f;
		}
	}
}

// Render
void CPatrol::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);

	if (!light)
		RenderHelper::RenderMesh(modelMesh);
	else
		RenderHelper::RenderMeshWithLight(modelMesh);
	modelStack.PopMatrix();

	//if (attributes.HEALTH > 0.f)
	//	renderHealthBar();
}


bool CPatrol::checkInsideBoundary(Vector3 minBoundary, Vector3 maxBoundary)
{
	Vector3 boundaryMin = minBoundary + waypoint[0];
	Vector3 boundaryMax = maxBoundary + waypoint[0];

	Vector3 playerMin = CPlayerInfo::GetInstance()->GetMinAABB() + Vector3(CPlayerInfo::GetInstance()->GetPos().x, -5.f, CPlayerInfo::GetInstance()->GetPos().z);
	Vector3 playerMax = CPlayerInfo::GetInstance()->GetMaxAABB() + Vector3(CPlayerInfo::GetInstance()->GetPos().x, -5.f, CPlayerInfo::GetInstance()->GetPos().z);

	if ((boundaryMin.x < playerMax.x && boundaryMax.x >playerMin.x) &&
		(boundaryMin.y < playerMax.y && boundaryMax.y >playerMin.y) &&
		(boundaryMin.z < playerMax.z && boundaryMax.z >playerMin.z))
		return true;
	else
		return false;
}


void CPatrol::setShootDelay(float _shootDelay)
{
	shootDelay = _shootDelay;
}

float CPatrol::getShootDelay(void)
{
	return shootDelay;
}

void CPatrol::renderHealthBar(void)
{

}


CPatrol* Create::Patrol(const std::string& _meshName,
	const Vector3& _position,
	const Vector3& _scale)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	CPatrol* result = new CPatrol(modelMesh);
	Vector3 newPosition(_position.x, _position.y - _scale.y, _position.z);
	result->SetPosition(newPosition);
	result->SetScale(_scale);
	result->SetCollider(true);
	result->setPlayerProperty(false);
	result->waypointToGo = 0;
	result->waypoint.push_back(newPosition);
	EntityManager::GetInstance()->AddEnemy(result);
	return result;
}