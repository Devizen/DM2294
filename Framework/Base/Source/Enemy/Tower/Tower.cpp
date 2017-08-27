#include "Tower.h"
#include "../../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"
#include "../../Base/Source/Debugger/Debugger.h"
#include "../../ParticleManager.h"

CTower::CTower(Mesh* _modelMesh)
	: CEnemy3D(NULL)
	, defaultPosition(Vector3(0.0f, 0.0f, 0.0f))
	, defaultTarget(Vector3(0.0f, 0.0f, 0.0f))
	, defaultUp(Vector3(0.0f, 0.0f, 0.0f))
	, target(Vector3(0.0f, 0.0f, 0.0f))
	, up(Vector3(0.0f, 0.0f, 0.0f))
	, maxBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, minBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, m_pTerrain(NULL)
	, demolished(false)
{
	this->modelMesh = _modelMesh;
	SetAlertBoundary(Vector3(0.f, 0.f, 0.f), Vector3(0.f, 0.f, 0.f));
	SetElapsedTime(0.f);
	setHealthTo(10);
	SetState(AI_STATE::IDLE);
	SetPlayerProperty(false);
	SetWhoCloser(CEnemy3D::NONE);
	SetPreviousPosition(Vector3(0.f, 0.f, 0.f));
	gravity.Set(0.f, -9.8f, 0.f);
	initialVelocity.Set(0.f, 0.f, 0.f);
	/*Speed to slow down the demolishing.*/
	speed = 0.1f;
	mass = 100.f;
	force.Set(200.f, 0.f, 0.f);
	SetType(4);
}


CTower::~CTower()
{
}

void CTower::Init(void)
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
	SetState(IDLE);

	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this);

}

// Reset this player instance to default
void CTower::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

// Update
void CTower::Update(double dt)
{
	//static Debug_Message* _debug = new Debug_Message();
	//SetPortableDT(dt);
	if (GetAttribute(CAttributes::ATTRIBUTE_TYPES::TYPE_HEALTH) <= 0)
	{
		if (!demolished)
		{
			finalVelocity += gravity * static_cast<float>(dt) * speed;
			Vector3 shake(Math::RandFloatMinMax(-0.5f, 0.5f), 0.f, Math::RandFloatMinMax(-0.5f, 0.5f));
			position += finalVelocity + shake;
			maxAABB += finalVelocity + shake;

			if (maxAABB.y < -10.f)
			{
				SetState(AI_STATE::DEAD);
				demolished = true;
			}
		}
	}

	switch (GetState())
	{
		case IDLE:
		{
			///*Position*/
			//_debug->SetMessageType(Debug_Message::POSITION);
			//_debug->SetVector3(position);
			//Debugger::GetInstance()->AddMessage(_debug);
			//_debug->SetMessageType(Debug_Message::POSITION);
			//_debug->SetVector3(minAABB);
			//_debug->SetMessage("Min AABB: ");
			//Debugger::GetInstance()->AddMessage(_debug);
			//_debug->SetMessageType(Debug_Message::POSITION);
			//_debug->SetVector3(maxAABB);
			//_debug->SetMessage("Max AABB: ");
			//Debugger::GetInstance()->AddMessage(_debug);

			///*Defined Message*/
			//_debug->SetMessageType(Debug_Message::DEFINED);
			//_debug->SetMessage("I am in Tower.");
			//Debugger::GetInstance()->AddMessage(_debug);
			break;
		}
		case ALERT:
		{
			break;
		}
		case DEAD:
		{
			isTower = false;
			//if (!demolished)
			//{
			//	finalVelocity += gravity * static_cast<float>(dt) * speed;
			//	Vector3 shake(Math::RandFloatMinMax(-1.f, 1.f), 0.f, Math::RandFloatMinMax(-1.f, 1.f));
			//	position += finalVelocity + shake;
			//	maxAABB += finalVelocity + shake;

			//	if (maxAABB.y < -10.f)
			//		demolished = true;
			//}
			break;
		}
		case RECOVERY:
		{
			break;
		}
	}
}

// Render
void CTower::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Rotate(rotate, 0.f, 1.f, 0.f);
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (!light)
		RenderHelper::RenderMesh(modelMesh);
	else
		RenderHelper::RenderMeshWithLight(modelMesh);
	modelStack.PopMatrix();

	if (GetAttribute(CAttributes::TYPE_HEALTH) > 0.f)
		RenderHealthBar();
}

CTower* Create::Tower(const std::string& _meshName,
	const Vector3& _position,
	const float& _rotate,
	const Vector3& _scale,
	const bool& _playerProperty)
{

	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	CTower* result = new CTower(modelMesh);

	//cout << "Position in Create: " << _position << endl;
	result->SetPosition(_position);
	result->setDefaultPosition(_position);
	result->SetPos(_position);
	result->SetRotate(_rotate);
	result->SetScale(_scale);
	result->SetCollider(true);
	result->SetPlayerProperty(_playerProperty);
	result->SetLight(true);
	result->SetMinAABB(Vector3(-_scale.x * 4.f, -10.f, -_scale.z * 4.f));
	result->SetMaxAABB(Vector3(_scale.x * 4.f, _scale.x * _scale.y * _scale.z * 2.5f, _scale.z * 4.f));
	result->SetAlertBoundary(Vector3(-150.f, -10.f, -150.f), Vector3(150.f, 10.f, 150.f));
	result->setMaxHealthTo(10);
	result->setHealthTo(10);
	result->setAttackTo(1);
	result->setDefenseTo(1);
	result->SetType(4);
	result->SetLight(true);
	result->isTower = true;
	EntityManager::GetInstance()->AddEnemy(result);
	return result;
}