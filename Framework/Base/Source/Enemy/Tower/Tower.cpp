#include "Tower.h"
#include "../../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"

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
{
	this->modelMesh = _modelMesh;
	setAlertBoundary(Vector3(0.f, 0.f, 0.f), Vector3(0.f, 0.f, 0.f));
	SetElapsedTime(0.f);
	setHealthTo(10.f);
	setState(AI_STATE::IDLE);
	setPlayerProperty(false);
	SetWhoCloser(CEnemy3D::NONE);
	SetPreviousPosition(Vector3(0.f, 0.f, 0.f));
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
	setState(IDLE);

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
	setPortableDT(dt);
	switch (getState())
	{
	case IDLE:
	{
		
	}
	case ALERT:
	{
		
	}
	case DEAD:
	{
		
	}
	case RECOVERY:
	{
		
	}
	}
}

// Render
void CTower::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Rotate(rotate, 1.f, 0.f, 0.f);
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (!light)
		RenderHelper::RenderMesh(modelMesh);
	else
		RenderHelper::RenderMeshWithLight(modelMesh);
	modelStack.PopMatrix();

	if (GetAttribute(CAttributes::TYPE_HEALTH) > 0.f)
		renderHealthBar();
}

CTower* Create::Tower(const std::string& _meshName,
	const Vector3& _position,
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