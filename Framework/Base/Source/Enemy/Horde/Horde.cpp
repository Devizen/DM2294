/*Horde was done by Lai Wei Jing, Eugene (160540A)*/
/*Tested by Chan Zhi Hao, Lim Guan Sheng, Marcus and Kwong Yu Ming*/
#include "Horde.h"
#include "../../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"
#include "../../Application.h"
#include "../AnimatedEnemy/AnimatedEnemy.h"

CHorde::CHorde(Mesh * _modelMesh)
	: CEnemy3D(NULL)
	, minAlertBoundary(Vector3(-100.f, -50.f, -100.f))
	, maxAlertBoundary(Vector3(100.f, 50.f, 100.f))
	, m_fElapsedTimeBeforeUpdate(3.0f)
	, state(PATROL)
{
	this->modelMesh = _modelMesh;
}

CHorde::~CHorde()
{
}

void CHorde::Init(void)
{
}

// Reset this player instance to default
void CHorde::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

// Update
void CHorde::Update(double dt)
{
	//cout << "Displacement: " << (CPlayerInfo::GetInstance()->GetPos() - this->GetPos()).LengthSquared() << endl;
	if (checkInsideBoundary(minAlertBoundary, maxAlertBoundary))
		state = ALERT;
	else
		state = IDLE;

	switch (state)
	{
		case IDLE:
		{
			Vector3 displacement(defaultPosition - this->GetPos());

			if (displacement.LengthSquared() > 0.f)
			{
				try
				{
					position += displacement.Normalized() * (float)dt * 20.f;
				}

				catch (exception e)
				{

				}
			}

			break;
		}
		case ALERT:
		{
			Vector3 positionWithoutY(CPlayerInfo::GetInstance()->GetPos().x, -10.f, CPlayerInfo::GetInstance()->GetPos().z);
			Vector3 displacement(positionWithoutY - this->GetPos());

			if (displacement.LengthSquared() > scale.LengthSquared() * 5.f)
				position += displacement.Normalized() * (float)dt * 20.f;

			break;
		}
	}
}

// Render
void CHorde::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);

	if (state == CEnemy3D::AI_STATE::IDLE)
	{
		try {
			angleToFace = Math::RadianToDegree(atan2(defaultPosition.x - this->position.x, defaultPosition.z - this->position.z));
		}

		catch (string Error)
		{
			cout << "Divide by Zero" << endl;
		}
	}
	else if (state == CEnemy3D::AI_STATE::ALERT)
	{
		try {
			angleToFace = Math::RadianToDegree(atan2(CPlayerInfo::GetInstance()->GetPos().x - this->position.x, CPlayerInfo::GetInstance()->GetPos().z - this->position.z));
		}

		catch (string Error)
		{
			cout << "Divide by Zero" << endl;
		}
	}
	modelStack.Rotate(angleToFace, 0.f, 1.f, 0.f);
	modelStack.Scale(scale.x, scale.y, scale.z);

	if (!light)
		RenderHelper::RenderMesh(modelMesh);
	else
		RenderHelper::RenderMeshWithLight(modelMesh);
	modelStack.PopMatrix();

	if (GetAttribute(CAttributes::TYPE_HEALTH) > 0.f)
		renderHealthBar();
}


bool CHorde::checkInsideBoundary(Vector3 minBoundary, Vector3 maxBoundary)
{
	Vector3 boundaryMin = minBoundary + defaultPosition;
	Vector3 boundaryMax = maxBoundary + defaultPosition;

	Vector3 playerMin = CPlayerInfo::GetInstance()->GetMinAABB() + Vector3(CPlayerInfo::GetInstance()->GetPos().x, -5.f, CPlayerInfo::GetInstance()->GetPos().z);
	Vector3 playerMax = CPlayerInfo::GetInstance()->GetMaxAABB() + Vector3(CPlayerInfo::GetInstance()->GetPos().x, -5.f, CPlayerInfo::GetInstance()->GetPos().z);

	if ((boundaryMin.x < playerMax.x && boundaryMax.x >playerMin.x) &&
		(boundaryMin.y < playerMax.y && boundaryMax.y >playerMin.y) &&
		(boundaryMin.z < playerMax.z && boundaryMax.z >playerMin.z))
		return true;
	else
		return false;
}


void CHorde::setShootDelay(float _shootDelay)
{
	shootDelay = _shootDelay;
}

float CHorde::getShootDelay(void)
{
	return shootDelay;
}

void CHorde::setDefaultPosition(Vector3 _defaultPosition)
{
	defaultPosition = _defaultPosition;
}

const Vector3 CHorde::getDefaultPosition(void)
{
	return defaultPosition;
}

//void CHorde::renderHealthBar(void)
//{
//
//}

CHorde* Create::Horde(const std::string& _meshName,
	const Vector3& _position,
	const Vector3& _scale)
{
	Vector3 _leftPosition(_position.x - (_scale.x * _scale.z * 2.f), _position.y, _position.z);
	Vector3 _rightPosition(_position.x + (_scale.x * _scale.z * 2.f), _position.y, _position.z);
	Vector3 _upPosition(_position.x, _position.y, _position.z + (_scale.x * _scale.z * 2.f));
	Vector3 _downPosition(_position.x, _position.y, _position.z - (_scale.x * _scale.z * 2.f));
	CAnimatedEnemy* _center = Create::AnimatedEnemy("ROBOT_CORE", "ROBOT_LeftArm", "ROBOT_RightArm", "ROBOT_LeftLeg", "ROBOT_RightLeg", "ROBOT_Head", _position, _scale);
	CAnimatedEnemy* _left =Create::AnimatedEnemy("ROBOT_CORE", "ROBOT_LeftArm", "ROBOT_RightArm", "ROBOT_LeftLeg", "ROBOT_RightLeg", "ROBOT_Head", _leftPosition, _scale);
	CAnimatedEnemy* _right =Create::AnimatedEnemy("ROBOT_CORE", "ROBOT_LeftArm", "ROBOT_RightArm", "ROBOT_LeftLeg", "ROBOT_RightLeg", "ROBOT_Head", _rightPosition, _scale);
	CAnimatedEnemy* _up =Create::AnimatedEnemy("ROBOT_CORE", "ROBOT_LeftArm", "ROBOT_RightArm", "ROBOT_LeftLeg", "ROBOT_RightLeg", "ROBOT_Head", _upPosition, _scale);
	CAnimatedEnemy* _down =Create::AnimatedEnemy("ROBOT_CORE", "ROBOT_LeftArm", "ROBOT_RightArm", "ROBOT_LeftLeg", "ROBOT_RightLeg", "ROBOT_Head", _downPosition, _scale);

	_center->SetAABB(Vector3(_scale.x, _scale.y * 3.f, _scale.z), Vector3(-_scale.x, -_scale.y, -_scale.z));
	_center->SetLight(true);
	_left->SetAABB(Vector3(_scale.x, _scale.y * 3.f, _scale.z), Vector3(-_scale.x, -_scale.y, -_scale.z));
	_left->SetLight(true);
	_right->SetAABB(Vector3(_scale.x, _scale.y * 3.f, _scale.z), Vector3(-_scale.x, -_scale.y, -_scale.z));
	_right->SetLight(true);
	_up->SetAABB(Vector3(_scale.x, _scale.y * 3.f, _scale.z), Vector3(-_scale.x, -_scale.y, -_scale.z));
	_up->SetLight(true);
	_down->SetAABB(Vector3(_scale.x, _scale.y * 3.f, _scale.z), Vector3(-_scale.x, -_scale.y, -_scale.z));
	_down->SetLight(true);

	return nullptr;

	//Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	//if (modelMesh == nullptr)
	//	return nullptr;

	//CHorde* center = new CHorde(modelMesh);
	//center->SetPosition(_position);
	//center->SetScale(_scale);
	//center->setDefaultPosition(_position);
	//center->SetAABB(Vector3(_scale.x, _scale.y * 3.f, _scale.z), Vector3(-_scale.x, -_scale.y, -_scale.z));
	//center->SetCollider(true);
	//EntityManager::GetInstance()->AddEnemy(center);

	//CHorde* left = new CHorde(modelMesh);
	//Vector3 _leftPosition(_position.x - (_scale.x * _scale.z * 2.f), _position.y, _position.z);
	//left->SetPosition(_leftPosition);
	//left->SetScale(_scale);
	//left->setDefaultPosition(_leftPosition);
	//left->SetAABB(Vector3(_scale.x, _scale.y * 3.f, _scale.z), Vector3(-_scale.x, -_scale.y, -_scale.z));
	//left->SetCollider(true);
	//EntityManager::GetInstance()->AddEnemy(left);

	//CHorde* right = new CHorde(modelMesh);
	//Vector3 _rightPosition(_position.x + (_scale.x * _scale.z * 2.f), _position.y, _position.z);
	//right->SetPosition(_rightPosition);
	//right->SetScale(_scale);
	//right->setDefaultPosition(_rightPosition);
	//right->SetAABB(Vector3(_scale.x, _scale.y * 3.f, _scale.z), Vector3(-_scale.x, -_scale.y, -_scale.z));
	//right->SetCollider(true);
	//EntityManager::GetInstance()->AddEnemy(right);

	//CHorde* up = new CHorde(modelMesh);
	//Vector3 _upPosition(_position.x, _position.y, _position.z + (_scale.x * _scale.z * 2.f));
	//up->SetPosition(_upPosition);
	//up->SetScale(_scale);
	//up->setDefaultPosition(_upPosition);
	//up->SetAABB(Vector3(_scale.x, _scale.y * 3.f, _scale.z), Vector3(-_scale.x, -_scale.y, -_scale.z));
	//up->SetCollider(true);
	//EntityManager::GetInstance()->AddEnemy(up);

	//CHorde* down = new CHorde(modelMesh);
	//Vector3 _downPosition(_position.x, _position.y, _position.z - (_scale.x * _scale.z * 2.f));
	//down->SetPosition(_downPosition);
	//down->SetScale(_scale);
	//down->setDefaultPosition(_downPosition);
	//down->SetAABB(Vector3(_scale.x, _scale.y * 3.f, _scale.z), Vector3(-_scale.x, -_scale.y, -_scale.z));
	//down->SetCollider(true);
	//EntityManager::GetInstance()->AddEnemy(down);

	//center->setAlertBoundary(Vector3(-150.f, -10.f, -150.f), Vector3(150.f, 10.f, 150.f));
	//center->SetCollider(true);
	//center->SetLight(true);;
	//center->setMaxHealthTo(10.f);
	//center->setHealthTo(10.f);
	//center->setAttackTo(1.f);
	//center->setDefenseTo(1.f);
	//center->setPlayerProperty(false);
	//center->SetLight(true);

	//left->setAlertBoundary(Vector3(-150.f, -10.f, -150.f), Vector3(150.f, 10.f, 150.f));
	//left->SetCollider(true);
	//left->SetLight(true);
	//left->setMaxHealthTo(10.f);
	//left->setHealthTo(10.f);
	//left->setAttackTo(1.f);
	//left->setDefenseTo(1.f);
	//left->setPlayerProperty(false);
	//left->SetLight(true);

	//right->setAlertBoundary(Vector3(-150.f, -10.f, -150.f), Vector3(150.f, 10.f, 150.f));
	//right->SetCollider(true);
	//right->SetLight(true);
	//right->setMaxHealthTo(10.f);
	//right->setHealthTo(10.f);
	//right->setAttackTo(1.f);
	//right->setDefenseTo(1.f);
	//right->setPlayerProperty(false);
	//right->SetLight(true);

	//up->setAlertBoundary(Vector3(-150.f, -10.f, -150.f), Vector3(150.f, 10.f, 150.f));
	//up->SetCollider(true);
	//up->SetLight(true);
	//up->setMaxHealthTo(10.f);
	//up->setHealthTo(10.f);
	//up->setAttackTo(1.f);
	//up->setDefenseTo(1.f);
	//up->setPlayerProperty(false);
	//up->SetLight(true);

	//down->setAlertBoundary(Vector3(-150.f, -10.f, -150.f), Vector3(150.f, 10.f, 150.f));
	//down->SetCollider(true);
	//down->SetLight(true);
	//down->setMaxHealthTo(10.f);
	//down->setHealthTo(10.f);
	//down->setAttackTo(1.f);
	//down->setDefenseTo(1.f);
	//down->setPlayerProperty(false);
	//down->SetLight(true);

	//return center;
}