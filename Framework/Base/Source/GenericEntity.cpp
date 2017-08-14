#include "GenericEntity.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
/*FPS Camera*/
#include "FPSCamera.h"
/*Debugging*/
#include <iostream>

using std::cout;
using std::endl;

GenericEntity::GenericEntity(Mesh* _modelMesh)
	: modelMesh(_modelMesh)
{
}

GenericEntity::~GenericEntity()
{
}

void GenericEntity::Update(double _dt)
{
	// Does nothing here, can inherit & override or create your own version of this class :D
}

void GenericEntity::Render()
{
	float x = rotationAxis.x;
	float y = rotationAxis.y;
	float z = rotationAxis.z;

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Rotate(rotate, x, y, z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (!light)
		RenderHelper::RenderMesh(modelMesh);
	else
		RenderHelper::RenderMeshWithLight(modelMesh);
	modelStack.PopMatrix();
}

void GenericEntity::RenderToScreen(Mesh* _mesh)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	//// Push the current transformation into the modelStack
	//modelStack.PushMatrix();
	//// Translate the current transformation
	//modelStack.Translate(position.x, position.y, position.z);
	// Scale the current transformation
	modelStack.Scale(scale.x, scale.y, scale.z);

	// Push the current transformation into the modelStack
	modelStack.PushMatrix();
	// Display the Avatar
	RenderHelper::RenderMesh(_mesh);

	modelStack.PopMatrix();

	//modelStack.PopMatrix();

}

// Set the maxAABB and minAABB
void GenericEntity::SetAABB(Vector3 maxAABB, Vector3 minAABB)
{
	this->maxAABB = maxAABB;
	this->minAABB = minAABB;
}

GenericEntity* Create::Entity(const std::string& _meshName,
								const Vector3& _position,
								const Vector3& _scale,
								const float& _rotate,
								const Vector3& _rotationAxis)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	GenericEntity* result = new GenericEntity(modelMesh);
	result->SetPosition(_position);
	result->SetRotate(_rotate);
	result->SetRotationAxis(_rotationAxis);
	result->SetScale(_scale);
	result->SetCollider(false);
	EntityManager::GetInstance()->AddEntity(result);
	return result;
}
