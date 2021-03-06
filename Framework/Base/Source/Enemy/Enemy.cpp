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

/*Loading text files.*/
#include <fstream>

/*Reading terrain height.*/
#include "../Terrain/LoadHmap.h"
/*To access vector that consists height map.*/
#include "SceneManager.h"

/*To adjust AABB collision box.*/
#include "../Adjuster/Adjuster.h"

using std::cout;
using std::endl;

CEnemy::CEnemy() :
	modelMesh(nullptr)
	, name("")
	, position(0.f, 0.f, 0.f)
	, defaultPosition(0.f, 0.f, 0.f)
	, rotate(0.f)
	/*, type(NONE)*/
{
}

CEnemy::CEnemy(Mesh * _modelMesh) :
	modelMesh(_modelMesh)
	, name("")
	, position(0.f, 0.f, 0.f)
	, defaultPosition(0.f, 0.f, 0.f)
	, rotate(0.f)
	/*, type(NONE)*/
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

	/*Set visual AABB box.*/
	modelMesh = MeshBuilder::GetInstance()->GetMesh("AABB");

	/*Initialise model.*/
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

	/*Initialise translation for body parts.*/
	translateHumanModel["HEAD"] = _position;
	translateHumanModel["BODY"] = _position;
	translateHumanModel["LEFTARM"] = _position;
	translateHumanModel["RIGHTARM"] = _position;
	translateHumanModel["LEFTLEG"] = _position;
	translateHumanModel["RIGHTLEG"] = _position;
	if (_accessoryOne != nullptr)
		translateHumanModel["ACCESSORYONE"] = _position;
	if (_accessoryTwo != nullptr)
		translateHumanModel["ACCESSORYTWO"] = _position;
	if (_accessoryThree != nullptr)
		translateHumanModel["ACCESSORYTHREE"] = _position;
	//translateHumanModel["HEAD"] = Vector3(0.f, 0.f, 0.f);
	//translateHumanModel["BODY"] = Vector3(0.f, 0.f, 0.f);
	//translateHumanModel["LEFTARM"] = Vector3(0.f, 0.f, 0.f);
	//translateHumanModel["RIGHTARM"] = Vector3(0.f, 0.f, 0.f);
	//translateHumanModel["LEFTLEG"] = Vector3(0.f, 0.f, 0.f);
	//translateHumanModel["RIGHTLEG"] = Vector3(0.f, 0.f, 0.f);
	//if (_accessoryOne != nullptr)
	//	translateHumanModel["ACCESSORYONE"] = Vector3(0.f, 0.f, 0.f);
	//if (_accessoryTwo != nullptr)
	//	translateHumanModel["ACCESSORYTWO"] = Vector3(0.f, 0.f, 0.f);
	//if (_accessoryThree != nullptr)
	//	translateHumanModel["ACCESSORYTHREE"] = Vector3(0.f, 0.f, 0.f);

	/*Initialise rotation for body parts.*/
	rotateHumanModel["HEAD"] = 0.f;
	rotateHumanModel["BODY"] = 0.f;
	rotateHumanModel["LEFTARM"] = 0.f;
	rotateHumanModel["RIGHTARM"] = 0.f;
	rotateHumanModel["LEFTLEG"] = 0.f;
	rotateHumanModel["RIGHTLEG"] = 0.f;
	if (_accessoryOne != nullptr)
		rotateHumanModel["ACCESSORYONE"] = 0.f;
	if (_accessoryTwo != nullptr)
		rotateHumanModel["ACCESSORYTWO"] = 0.f;
	if (_accessoryThree != nullptr)
		rotateHumanModel["ACCESSORYTHREE"] = 0.f;

	/*Initialise translate state(s) for body parts.*/
	translateHumanModelState["HEAD"] = 0;
	translateHumanModelState["BODY"] = 0;
	translateHumanModelState["LEFTARM"] = 0;
	translateHumanModelState["RIGHTARM"] = 0;
	translateHumanModelState["LEFTLEG"] = 0;
	translateHumanModelState["RIGHTLEG"] = 0;
	if (_accessoryOne != nullptr)
		translateHumanModelState["ACCESSORYONE"] = 0;
	if (_accessoryTwo != nullptr)
		translateHumanModelState["ACCESSORYTWO"] = 0;
	if (_accessoryThree != nullptr)
		translateHumanModelState["ACCESSORYTHREE"] = 0;

	/*Initialise rotation state(s) for body parts.*/
	rotateHumanModelState["HEAD"] = 0;
	rotateHumanModelState["BODY"] = 0;
	rotateHumanModelState["LEFTARM"] = 0;
	rotateHumanModelState["RIGHTARM"] = 0;
	rotateHumanModelState["LEFTLEG"] = 0;
	rotateHumanModelState["RIGHTLEG"] = 0;
	if (_accessoryOne != nullptr)
		rotateHumanModelState["ACCESSORYONE"] = 0;
	if (_accessoryTwo != nullptr)
		rotateHumanModelState["ACCESSORYTWO"] = 0;
	if (_accessoryThree != nullptr)
		rotateHumanModelState["ACCESSORYTHREE"] = 0;

	rotate = 0.f;

	/*Set the position to place the enemy in the world space.*/
	position = _position;

	/*Set the enemy type to be human so that it will render all meshes.*/
	type = CEnemy::HUMAN;

	/*Positioning of the health bar during battle scene.*/
	if (name == "Monk")
		healthBarPosition = 0.25f;

	/*Add the enemy into the Enemy Manager.*/
	CEnemy_Manager::GetInstance()->AddEnemy(this);

	/*Load animations.*/
	Load(this);
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

void CEnemy::Render(void)
{
	if (type == CEnemy::HUMAN)
	{
		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

		for (std::map<string, Mesh*>::iterator it = humanModelMesh.begin(); it != humanModelMesh.end(); ++it)
		{
			/*Prevent program from crashing when the body part is not used.*/
			if (it->second == nullptr)
				continue;

			Render(it->first);
		}
		//	float offsetLeg = 0.f;
		//	if (it->first == "LEFTLEG" || it->first == "RIGHTLEG")
		//		offsetLeg = 12.3f;

		//	modelStack.PushMatrix();
		//	modelStack.Translate(position.x + translateHumanModel[it->first].x, 
		//		position.y + translateHumanModel[it->first].y + offsetLeg, 
		//		position.z + translateHumanModel[it->first].z);

		//	modelStack.Rotate(rotateHumanModel[it->first], 1.f, 0.f, 0.f);
		//	if (it->first == "LEFTLEG" || it->first == "RIGHTLEG")
		//		modelStack.Translate(0.f, -offsetLeg, 0.f);
		//	modelStack.Scale(10.f, 10.f, 10.f);
		//	RenderHelper::GetInstance()->RenderMeshWithLight(it->second);
		//	modelStack.PopMatrix();
		//}

		if (CAdjuster::GetInstance()->GetState())
		{
			modelStack = GraphicsManager::GetInstance()->GetModelStack();
			modelStack.PushMatrix();
			modelStack.Translate(translateHumanModel["BODY"].x, translateHumanModel["BODY"].y, translateHumanModel["BODY"].z);
			//modelStack.Translate(position.x + translateHumanModel["BODY"].x, position.y + translateHumanModel["BODY"].y, position.z + translateHumanModel["BODY"].z);
			modelStack.Scale(maxAABB.x - minAABB.x, maxAABB.y - minAABB.y, maxAABB.z - minAABB.z);
			//modelStack.Scale(CAdjuster::GetInstance()->GetScale().x, CAdjuster::GetInstance()->GetScale().y, CAdjuster::GetInstance()->GetScale().z);
			RenderHelper::GetInstance()->RenderMesh(modelMesh);
			modelStack.PopMatrix();
			//CAdjuster::GetInstance()->SetSpeed(10.f);
			//CAdjuster::GetInstance()->SetMode(CAdjuster::SCALE_MODE);
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

void CEnemy::Render(std::string _bodyPart)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	/*Prevent program from crashing when the body part is not used.*/
	if (humanModelMesh[_bodyPart] == nullptr)
		return;

	float offsetLeg = 0.f;
	if (_bodyPart == "LEFTLEG" || _bodyPart == "RIGHTLEG")
		offsetLeg = 12.3f;

	modelStack.PushMatrix();
	modelStack.Translate(translateHumanModel[_bodyPart].x,
		translateHumanModel[_bodyPart].y + offsetLeg,
		translateHumanModel[_bodyPart].z);

	modelStack.Rotate(rotateHumanModel[_bodyPart], 1.f, 0.f, 0.f);
	if (_bodyPart == "LEFTLEG" || _bodyPart == "RIGHTLEG")
		modelStack.Translate(0.f, -offsetLeg, 0.f);
	modelStack.Scale(10.f, 10.f, 10.f);
	RenderHelper::GetInstance()->RenderMeshWithLight(humanModelMesh[_bodyPart]);
	modelStack.PopMatrix();
}

void CEnemy::Update(double dt)
{
	/*Keep updating position.y to terrain height.*/
	position.y = (350.f * ReadHeightMap(SceneManager::GetInstance()->GetHeightMap(), position.x / 4000.f, position.z / 4000.f));

	if (humanModelMesh.size() > 0)
	{
		UpdateAnimation("HEAD", dt);
		UpdateAnimation("BODY", dt);
		UpdateAnimation("LEFTARM", dt);
		UpdateAnimation("RIGHTARM", dt);
		UpdateAnimation("LEFTLEG", dt);
		UpdateAnimation("RIGHTLEG", dt);
		UpdateAnimation("ACCESSORYONE", dt);
		UpdateAnimation("ACCESSORYTWO", dt);
		UpdateAnimation("ACCESSORYTHREE", dt);

		if (rotateHumanModelState["LEFTLEG"] == 0)
		{
			rotateHumanModel["LEFTLEG"] += static_cast<float>(dt) * 50.f;
			if (rotateHumanModel["LEFTLEG"] >= 10.f)
				rotateHumanModelState["LEFTLEG"] = 1;
		}
		else if (rotateHumanModelState["LEFTLEG"] == 1)
		{
			rotateHumanModel["LEFTLEG"] -= static_cast<float>(dt) * 50.f;
			if (rotateHumanModel["LEFTLEG"] <= -10.f)
				rotateHumanModelState["LEFTLEG"] = 0;
		}

		if (rotateHumanModelState["RIGHTLEG"] == 0)
		{
			rotateHumanModel["RIGHTLEG"] -= static_cast<float>(dt) * 50.f;
			if (rotateHumanModel["RIGHTLEG"] <= -10.f)
				rotateHumanModelState["RIGHTLEG"] = 1;
		}
		else if (rotateHumanModelState["RIGHTLEG"] == 1)
		{
			rotateHumanModel["RIGHTLEG"] += static_cast<float>(dt) * 50.f;
			if (rotateHumanModel["RIGHTLEG"] >= 10.f)
				rotateHumanModelState["RIGHTLEG"] = 0;
		}

		if (rotateHumanModelState["ACCESSORYTWO"] == 0)
		{
			rotateHumanModel["ACCESSORYTWO"] -= static_cast<float>(dt) * 50.f;
			if (rotateHumanModel["ACCESSORYTWO"] <= -10.f)
				rotateHumanModelState["ACCESSORYTWO"] = 1;
		}
		else if (rotateHumanModelState["ACCESSORYTWO"] == 1)
		{
			rotateHumanModel["ACCESSORYTWO"] += static_cast<float>(dt) * 50.f;
			if (rotateHumanModel["ACCESSORYTWO"] >= 10.f)
				rotateHumanModelState["ACCESSORYTWO"] = 0;
		}
		//std::cout << "AMOUNT: " << translate["BODY"].first.amount << std::endl;
		//std::cout << "COUNT: " << translate["BODY"].first.count << std::endl;
		//std::cout << "Position: " << translateHumanModel["BODY"] << std::endl;
	}
	/*Bind position with animation.*/
	//position = Vector3(translateHumanModel["BODY"].x, position.y, translateHumanModel["BODY"].z);
	/*Keep updating AABB to follow object.*/
	//minAABB += translateHumanModel["BODY"];
	//maxAABB += translateHumanModel["BODY"];
}

void CEnemy::Load(CEnemy * _enemy)
{
	std::string content = "";
	std::string filePath = "Animation//";
	filePath += _enemy->GetName() + "//";

	if (_enemy->GetEnemyType() == HUMAN)
	{
		std::ifstream path(filePath + "HEAD.txt");
		if (path)
			while (std::getline(path, content))
				translate["HEAD"].second.push_back(content);
		path.clear();
		path.close();

		path.open(filePath + "BODY.txt");
		if (path)
			while (std::getline(path, content))
				translate["BODY"].second.push_back(content);
		path.clear();
		path.close();

		path.open(filePath + "LEFTARM.txt");
		if (path)
			while (std::getline(path, content))
				translate["LEFTARM"].second.push_back(content);
		path.clear();
		path.close();

		path.open(filePath + "RIGHTARM.txt");
		if (path)
			while (std::getline(path, content))
				translate["RIGHTARM"].second.push_back(content);
		path.clear();
		path.close();

		path.open(filePath + "LEFTLEG.txt");
		if (path)
			while (std::getline(path, content))
				translate["LEFTLEG"].second.push_back(content);
		path.clear();
		path.close();

		path.open(filePath + "RIGHTLEG.txt");
		if (path)
			while (std::getline(path, content))
				translate["RIGHTLEG"].second.push_back(content);
		path.clear();
		path.close();

		path.open(filePath + "ACCESSORYONE.txt");
		if (path)
			while (std::getline(path, content))
				translate["ACCESSORYONE"].second.push_back(content);
		path.clear();
		path.close();

		path.open(filePath + "ACCESSORYTWO.txt");
		if (path)
			while (std::getline(path, content))
				translate["ACCESSORYTWO"].second.push_back(content);
		path.clear();
		path.close();

		path.open(filePath + "ACCESSORYTHREE.txt");
		if (path)
			while (std::getline(path, content))
				translate["ACCESSORYTHREE"].second.push_back(content);
		path.clear();
		path.close();
	}

	content = "";
	filePath = "Data//Collision//";
	filePath += _enemy->GetName() + "//AABB.txt";
	std::ifstream path(filePath);
	if (path)
		while (std::getline(path, content))
		{
			std::string x = "";
			std::string y = "";
			std::string z = "";
			unsigned wordCount = 0;
			for (unsigned i = 0; i < content.size(); ++i)
			{
				if (content[i] == ',')
				{
					++wordCount;
					continue;
				}

				if (wordCount == 0)
				{
					x += content[i];
					continue;
				}

				if (wordCount == 1)
				{
					y += content[i];
					continue;
				}

				if (wordCount == 2)
				{
					z += content[i];
					continue;
				}
			}

			minAABB = Vector3(-std::stof(x) * 0.5f,
				-std::stof(y) * 0.5f,
				-std::stof(z) * 0.5f);

			maxAABB = Vector3(std::stof(x) * 0.5f,
				std::stof(y) * 0.5f,
				std::stof(z) * 0.5f);
		}
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
	return enemy;
}
