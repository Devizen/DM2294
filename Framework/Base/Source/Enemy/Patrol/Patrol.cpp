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
	if (checkCollision())
	{
		position = previousPosition;
		cout << "COLLIDED" << endl;
	}
	else
	{
		previousPosition = position;
		cout << "NO COLLISION" << endl;
	}

	//cout << "Displacement: " << (CPlayerInfo::GetInstance()->GetPos() - this->GetPos()).LengthSquared() << endl;
	if ((CPlayerInfo::GetInstance()->GetPos() - this->GetPos()).LengthSquared() < 100.f * 100.f && checkInsideBoundary(minAlertBoundary, maxAlertBoundary))
		state = ALERT;
	else
		state = PATROL;

	switch (state)
	{
		case PATROL:
		{
			Vector3 displacement(waypoint[waypointToGo] - this->GetPos());
		
			try
			{
				position += displacement.Normalized() * (float)dt * 20.f;
			}
			catch (exception e)
			{
				/*Divide By Zero does no harm to this situation because it will only happen when there is only 1 waypoint and the enemy is on top, thus, can be left unresolved.*/
			}

			if (displacement.LengthSquared() <= 20.f)
				waypointToGo = ((waypointToGo == waypoint.size() - 1) ? 0 : ++waypointToGo);

			break;
		}
		case ALERT:
		{
			Vector3 positionWithoutY(CPlayerInfo::GetInstance()->GetPos().x, -10.f, CPlayerInfo::GetInstance()->GetPos().z);
			Vector3 displacement(positionWithoutY - this->GetPos());

			/*Using comparison of magnitude to mimic the real world environment where if the a person just left you not long ago, you will be more alerted and prepare if the person will return.*/
			if (displacement.LengthSquared() > scale.LengthSquared() * 5.f)
				position += displacement.Normalized() * (float)dt * 20.f;
		}
	}
}

// Render
void CPatrol::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	if (state == CEnemy3D::AI_STATE::ALERT)
	{
		try {
			angleToFace = Math::RadianToDegree(atan2(CPlayerInfo::GetInstance()->GetPos().x - this->position.x, CPlayerInfo::GetInstance()->GetPos().z - this->position.z));
		}

		catch (string Error)
		{
			cout << "Divide by Zero" << endl;
		}
	}
	else
	{
		Vector3 displacement(waypoint[waypointToGo] - this->GetPos());
		angleToFace = Math::RadianToDegree(atan2(displacement.x, displacement.z));
	}
	modelStack.Rotate(angleToFace, 0.f, 1.f, 0.f);
	modelStack.Scale(scale.x, scale.y, scale.z);

	if (!light)
		RenderHelper::RenderMesh(modelMesh);
	else
		RenderHelper::RenderMeshWithLight(modelMesh);
	modelStack.PopMatrix();

	if (getAttribute(CAttributes::TYPE_HEALTH) > 0.f)
		renderHealthBar();
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

//void CPatrol::renderHealthBar(void)
//{
//
//}

void CPatrol::increaseWaypointToGo(void)
{
	++waypointToGo;
}

void CPatrol::setToZeroWaypointToGo(void)
{
	waypointToGo = 0;
}

void CPatrol::addWaypoint(Vector3 _waypointToGo)
{
	waypoint.push_back(_waypointToGo);
}

CPatrol* Create::Patrol(const std::string& _meshName,
	const Vector3& _position,
	const Vector3& _scale)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	CPatrol* result = new CPatrol(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(true);
	result->setPlayerProperty(false);
	result->waypointToGo = 0;
	result->waypoint.push_back(_position);
	EntityManager::GetInstance()->AddEnemy(result);
	return result;
}