#include "Health.h"

#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

CHealth::CHealth()
{
}

CHealth::CHealth(Mesh * _modelMesh) :
	modelMesh(_modelMesh)
	, m_bStatus(false)
	, angleToFace(0.f)
{
}

CHealth::~CHealth()
{
}

void CHealth::Render()
{
	angleToFace = Math::RadianToDegree(atan2(CPlayerInfo::GetInstance()->GetPos().x - this->position.x, CPlayerInfo::GetInstance()->GetPos().z - this->position.z));
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Rotate(angleToFace + 90.f, 0.f, 1.f, 0.f);
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (!light)
		RenderHelper::RenderMesh(modelMesh);
	else
		RenderHelper::RenderMeshWithLight(modelMesh);
	modelStack.PopMatrix();

}

void CHealth::Set(Vector3 theNewPosition, Vector3 theNewScale)
{
	position = theNewPosition;
	scale = theNewScale;
}

void CHealth::SetStatus(const bool m_bStatus)
{
	if (m_bStatus == false)

	this->m_bStatus = m_bStatus;
}

CHealth* Create::Health(const std::string& _meshName, const Vector3& _position, const Vector3& _scale)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	CHealth* result = new CHealth(modelMesh);
	/*Align to ground*/
	Vector3 newPosition = _position;
	newPosition.y -= 10.f;

	result->SetPosition(newPosition);
	result->SetScale(_scale);
	result->SetCollider(true);
	Vector3 _minAABB(-5.f, -5.f, -5.f);
	Vector3 _maxAABB(5.f, 5.f, 5.f);
	result->SetLight(true);
	result->SetAABB(_maxAABB, _minAABB);
	result->SetItem(EntityBase::HEALTH);
	EntityManager::GetInstance()->AddItem(result);

	return result;
}
