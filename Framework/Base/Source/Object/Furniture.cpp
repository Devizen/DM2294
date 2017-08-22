#include "Furniture.h"

#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

#include <string>
using std::string;

CFurniture::CFurniture()
{
}

CFurniture::CFurniture(Mesh * _modelMesh) :
	modelMesh(_modelMesh)
	, m_bStatus(false)
{
}

CFurniture::~CFurniture()
{
}

void CFurniture::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	//modelStack.Rotate(37.f, 0.f, 1.f, 0.f);
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (!light)
		RenderHelper::RenderMesh(modelMesh);
	else
		RenderHelper::RenderMeshWithLight(modelMesh);
	modelStack.PopMatrix();

}

void CFurniture::Set(Vector3 theNewPosition, Vector3 theNewScale)
{
	position = theNewPosition;
	scale = theNewScale;
}

void CFurniture::SetStatus(const bool m_bStatus)
{
	if (m_bStatus == false)

	this->m_bStatus = m_bStatus;
}

void CFurniture::SetName(const string newName)
{
	name.assign(newName);
}

string CFurniture::GetName()
{
	return name;
}

CFurniture* Create::Furniture(const std::string& _meshName, const Vector3& _position, const Vector3& _scale)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	CFurniture* result = new CFurniture(modelMesh);
	/*Align to ground*/
	//Vector3 newPosition = _position;
	//newPosition.y -= 10.f;
	result->SetName(_meshName);
	result->SetPosition(_position);
	result->SetRotate(0.f);
	result->SetScale(_scale);
	result->SetCollider(true);
	result->SetLight(true);
	EntityManager::GetInstance()->AddFixed(result);

	return result;
}
