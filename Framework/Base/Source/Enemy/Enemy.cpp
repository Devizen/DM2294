#include "Enemy.h"
/*To get mesh.*/
#include "MeshBuilder.h"
/*To get modelStack.*/
#include "GraphicsManager.h"
/*To render mesh.*/
#include "RenderHelper.h"
/*To get window width and height.*/
#include "../Application.h"
/*Add enemy to enemy list.*/
#include "Enemy_Manager.h"

/*Debugging purpose.*/
#include <iostream>

using std::cout;
using std::endl;

CEnemy::CEnemy() :
	modelMesh(nullptr)
	, name("")
	, position(0.f, 0.f, 0.f)
	, defaultPosition(0.f, 0.f, 0.f)
	, rotate(0.f)
{
}

CEnemy::CEnemy(Mesh * _modelMesh) :
	modelMesh(_modelMesh)
	, name("")
	, position(0.f, 0.f, 0.f)
	, defaultPosition(0.f, 0.f, 0.f)
	, rotate(0.f)
{
}

CEnemy::CEnemy(Mesh * _modelMesh, Vector3 _position) :
	modelMesh(_modelMesh)
	, name("")
	, position(_position)
	, defaultPosition(_position)
	, rotate(0.f)
{
}

CEnemy::~CEnemy()
{
}

void CEnemy::SetName(string _name)
{
	name = _name;
}

void CEnemy::SetPosition(Vector3 _position)
{
	position = _position;
}

void CEnemy::SetDefaultPosition(Vector3 _defaultPosition)
{
	defaultPosition = _defaultPosition;
}

void CEnemy::SetModelMesh(string _meshName)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return;
	else
		this->modelMesh = modelMesh;
}

string CEnemy::GetName(void) const
{
	return name;
}

Vector3 CEnemy::GetPosition(void) const
{
	return position;
}

Vector3 CEnemy::GetDefaultPosition(void) const
{
	return defaultPosition;
}

Mesh * CEnemy::GetModelMesh(void)
{
	return modelMesh;
}

void CEnemy::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Rotate(rotate, 0.f, 1.f, 0.f);
	modelStack.Scale(10.f, 10.f, 10.f);
	RenderHelper::GetInstance()->RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

void CEnemy::Update(double dt)
{
}

CEnemy * Create::Enemy(const string & _meshName, const Vector3 & _position)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	CEnemy* enemy = new CEnemy(modelMesh, _position);
	enemy->SetName(_meshName);

	cout << "Enemy in Create: " << enemy << endl;
	CEnemy_Manager::GetInstance()->AddEnemy(enemy);
	return enemy;
}
