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
	, type(NONE)
{
}

CEnemy::CEnemy(Mesh * _modelMesh) :
	modelMesh(_modelMesh)
	, name("")
	, position(0.f, 0.f, 0.f)
	, defaultPosition(0.f, 0.f, 0.f)
	, rotate(0.f)
	, type(NONE)
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

CEnemy::CEnemy(std::string _name,
	Mesh * _head, 
	Mesh * _body,
	Mesh * _leftArm, 
	Mesh * _rightArm, 
	Mesh * _leftLeg, 
	Mesh * _rightLeg, 
	Mesh * _accessoryOne, 
	Mesh * _accessoryTwo,
	Mesh * _accessoryThree, 
	Vector3 _position)
{
	name = _name;
	humanModelMesh["HEAD"] = _head;
	humanModelMesh["BODY"] = _body;
	humanModelMesh["LEFTARM"] = _leftArm;
	humanModelMesh["RIGHTARM"] = _rightArm;
	humanModelMesh["LEFTLEG"] = _leftLeg;
	humanModelMesh["RIGHTLEG"] = _rightLeg;
	if (_accessoryOne != nullptr)
		humanModelMesh["ACCESSORYONE"] = _accessoryOne;
	if (_accessoryTwo != nullptr)
		humanModelMesh["ACCESSORYTWO"] = _accessoryTwo;
	if (_accessoryThree != nullptr)
		humanModelMesh["ACCESSORYTHREE"] = _accessoryThree;

	position = defaultPosition =_position;
	rotate = 0.f;
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

void CEnemy::SetBattleMode(bool _activeBattle)
{
	activateBattle = _activeBattle;
}

void CEnemy::SetEnemyType(TYPE _type)
{
	type = _type;
}

void CEnemy::SetHealthBarPosition(float _healthBarPosition)
{
	healthBarPosition = _healthBarPosition;
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

bool CEnemy::GetActiveBattle(void)
{
	return activateBattle;
}

CEnemy::TYPE CEnemy::GetEnemyType(void)
{
	return type;
}

float CEnemy::GetHealthBarPosition(void)
{
	return healthBarPosition;
}

std::map<string, Mesh*>& CEnemy::GetHumanModelMesh(void)
{
	return humanModelMesh;
}

void CEnemy::Render(void)
{
	if (type == CEnemy::HUMAN)
	{
		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

		for (std::map<string, Mesh*>::iterator it = humanModelMesh.begin(); it != humanModelMesh.end(); ++it)
		{
			modelStack.PushMatrix();
			modelStack.Translate(position.x, position.y, position.z);
			modelStack.Rotate(rotate, 0.f, 1.f, 0.f);
			modelStack.Scale(10.f, 10.f, 10.f);
			RenderHelper::GetInstance()->RenderMesh(it->second);
			modelStack.PopMatrix();
		}
	}
	else
	{
		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		modelStack.PushMatrix();
		modelStack.Translate(position.x, position.y, position.z);
		modelStack.Rotate(rotate, 0.f, 1.f, 0.f);
		modelStack.Scale(10.f, 10.f, 10.f);
		RenderHelper::GetInstance()->RenderMesh(modelMesh);
		modelStack.PopMatrix();
	}
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
	enemy->SetHealthBarPosition(0.11f);
	return enemy;
}

CEnemy * Create::Human(const string& _name,
	const string & _head, 
	const string & _body, 
	const string & _leftArm, 
	const string & _rightArm, 
	const string & _leftLeg, 
	const string & _rightLeg, 
	const Vector3 & _position,
	const string& _accessoryOne,
	const string& _accessoryTwo,
	const string& _accessoryThree)
{
	/*Check head mesh.*/
	Mesh* head = MeshBuilder::GetInstance()->GetMesh(_head);
	if (head == nullptr)
		return nullptr;

	/*Check body mesh.*/
	Mesh* body = MeshBuilder::GetInstance()->GetMesh(_body);
	if (body == nullptr)
		return nullptr;

	/*Check left arm mesh.*/
	Mesh* leftArm = MeshBuilder::GetInstance()->GetMesh(_leftArm);
	if (leftArm == nullptr)
		return nullptr;

	/*Check right arm mesh.*/
	Mesh* rightArm = MeshBuilder::GetInstance()->GetMesh(_rightArm);
	if (rightArm == nullptr)
		return nullptr;

	/*Check left leg mesh.*/
	Mesh* leftLeg = MeshBuilder::GetInstance()->GetMesh(_leftLeg);
	if (leftLeg == nullptr)
		return nullptr;

	/*Check right leg mesh.*/
	Mesh* rightLeg = MeshBuilder::GetInstance()->GetMesh(_rightLeg);
	if (rightLeg == nullptr)
		return nullptr;

	/*Check accessory one mesh.*/
	Mesh* accessoryOne = MeshBuilder::GetInstance()->GetMesh(_accessoryOne);

	/*Check accessory two mesh.*/
	Mesh* accessoryTwo = MeshBuilder::GetInstance()->GetMesh(_accessoryTwo);

	/*Check accessory three mesh.*/
	Mesh* accessoryThree = MeshBuilder::GetInstance()->GetMesh(_accessoryThree);

	CEnemy* enemy = new CEnemy(_name, head, body, leftArm, rightArm, leftLeg, rightLeg, accessoryOne, accessoryTwo, accessoryThree, _position);
	CEnemy_Manager::GetInstance()->AddEnemy(enemy);
	enemy->SetEnemyType(CEnemy::HUMAN);

	if (enemy->GetName() == "Monk")
		enemy->SetHealthBarPosition(0.25f);
	return enemy;
}
