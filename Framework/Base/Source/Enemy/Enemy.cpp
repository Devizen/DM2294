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
	translateHumanModel["HEAD"] = Vector3(0.f, 0.f, 0.f);
	translateHumanModel["BODY"] = Vector3(0.f, 0.f, 0.f);
	translateHumanModel["LEFTARM"] = Vector3(0.f, 0.f, 0.f);
	translateHumanModel["RIGHTARM"] = Vector3(0.f, 0.f, 0.f);
	translateHumanModel["LEFTLEG"] = Vector3(0.f, 0.f, 0.f);
	translateHumanModel["RIGHTLEG"] = Vector3(0.f, 0.f, 0.f);
	if (_accessoryOne != nullptr)
		translateHumanModel["ACCESSORYONE"] = Vector3(0.f, 0.f, 0.f);
	if (_accessoryTwo != nullptr)
		translateHumanModel["ACCESSORYTWO"] = Vector3(0.f, 0.f, 0.f);
	if (_accessoryThree != nullptr)
		translateHumanModel["ACCESSORYTHREE"] = Vector3(0.f, 0.f, 0.f);

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

std::map<string, Mesh*>& CEnemy::GetHumanModelMesh(void)
{
	return humanModelMesh;
}

void CEnemy::UpdateAnimation(string _bodyPart, double dt)
{
	/*Real time loop control. Only check when the values differ.*/
	if (!translate[_bodyPart].first.check)
	{
		/*Used for going to next word.*/
		unsigned wordCount = 0;

		for (unsigned i = 0; i < translate[_bodyPart].second[translate[_bodyPart].first.count].size(); ++i)
		{
			/*Skip to next word when , is found.*/
			if (translate[_bodyPart].second[translate[_bodyPart].first.count][i] == ',')
			{
				++wordCount;
				continue;
			}

			/*First word is for checking whether the transformation is translate or rotate.*/
			if (wordCount == 0)
				translate[_bodyPart].first.transform += translate[_bodyPart].second[translate[_bodyPart].first.count][i];

			/*Second word is for checking which axis to transform.*/
			else if (wordCount == 1)
				translate[_bodyPart].first.axis += translate[_bodyPart].second[translate[_bodyPart].first.count][i];

			/*Third word is for transforming to a certain value.*/
			else if (wordCount == 2)
				translate[_bodyPart].first.value += translate[_bodyPart].second[translate[_bodyPart].first.count][i];

			/*Fourth word is for the speed of transformation.*/
			else if (wordCount == 3)
				translate[_bodyPart].first.speed += translate[_bodyPart].second[translate[_bodyPart].first.count][i];
		}
		/*Conclude that all the values have been checked.*/
		translate[_bodyPart].first.check = true;
	}
	/*Assign a TRANSLATEAXIS value to use for TranslateModel function.*/
	TRANSLATEAXIS axis;
	if (translate[_bodyPart].first.axis == "x")
		axis = X;
	else if (translate[_bodyPart].first.axis == "y")
		axis = Y;
	else
		axis = Z;

	/*Keep translating the model until the value is met.*/
	if (TranslateModel(type, _bodyPart, axis, std::stof(translate[_bodyPart].first.value), std::stof(translate[_bodyPart].first.speed), dt))
		/*Revert back to first animation if the animation count is at the last animation.*/
		if (translate[_bodyPart].second.size() == translate[_bodyPart].first.count + 1)
		{
			translate[_bodyPart].first.count = 0;
			/*Start checking again.*/
			ResetCheckTransform(_bodyPart);
		}
	/*Go to next animation if the animation count is not the last animation.*/
		else
		{
			++translate[_bodyPart].first.count;
			/*Start checking again.*/
			ResetCheckTransform(_bodyPart);
		}
}

bool CEnemy::TranslateModel(TYPE _type, string _bodyPart, TRANSLATEAXIS _axis, float _value, float _speed, double _dt)
{
	float dt = static_cast<float>(_dt);
	if (_type == HUMAN)
	{
		if (_axis == X)
			if (_value > 0.f)
			{
				translateHumanModel[_bodyPart].x += dt * _speed;
				if (translateHumanModel[_bodyPart].x >= _value)
					return true;
				return false;
			}
			else
			{
				translateHumanModel[_bodyPart].x -= dt * _speed;
				if (translateHumanModel[_bodyPart].x <= _value)
					return true;
				return false;
			}
		else if (_axis == Y)
			if (_value > 0.f)
			{
				translateHumanModel[_bodyPart].y += dt * _speed;
				if (translateHumanModel[_bodyPart].y >= _value)
					return true;
				return false;
			}
			else
			{
				translateHumanModel[_bodyPart].y -= dt * _speed;
				if (translateHumanModel[_bodyPart].y <= _value)
					return true;
				return false;
			}
		else if (_axis == Z)
			if (_value > 0.f)
			{
				translateHumanModel[_bodyPart].z += dt * _speed;
				if (translateHumanModel[_bodyPart].z >= _value)
					return true;
				return false;
			}
			else
			{
				translateHumanModel[_bodyPart].z -= dt * _speed;
				if (translateHumanModel[_bodyPart].z <= _value)
					return true;
				return false;
			}
	}
	return true;
}

void CEnemy::ResetCheckTransform(std::string _bodyPart)
{
	translate[_bodyPart].first.transform = "";
	translate[_bodyPart].first.axis = "";
	translate[_bodyPart].first.value = "";
	translate[_bodyPart].first.speed = "";
	translate[_bodyPart].first.check = false;
}

void CEnemy::Render(void)
{
	if (type == CEnemy::HUMAN)
	{
		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

		int offsetWithMesh = 0;
		for (std::map<string, Mesh*>::iterator it = humanModelMesh.begin(); it != humanModelMesh.end(); ++it)
		{
			float offsetLeg = 0.f;
			if (it->first == "LEFTLEG" || it->first == "RIGHTLEG")
				offsetLeg = 12.3f;

			modelStack.PushMatrix();
			modelStack.Translate(position.x + translateHumanModel[it->first].x, 
				position.y + translateHumanModel[it->first].y + offsetLeg, 
				position.z + translateHumanModel[it->first].z);

			modelStack.Rotate(rotateHumanModel[it->first], 1.f, 0.f, 0.f);
			if (it->first == "LEFTLEG" || it->first == "RIGHTLEG")
				modelStack.Translate(0.f, -offsetLeg, 0.f);
			modelStack.Scale(10.f, 10.f, 10.f);
			RenderHelper::GetInstance()->RenderMeshWithLight(it->second);
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
	if (humanModelMesh.size() > 0)
	{
		for (std::map<string, Mesh*>::iterator it = humanModelMesh.begin(); it != humanModelMesh.end(); ++it)
		{
			/*
			Draft Theory on how to make animation soft coded.
			File load HEAD
			Translate Animation got 4.
			0 translate z to 40
			1 translate x to 50
			2 translate z to 60
			3 translate y to 70

			if (type == CEnemy::HUMAN)
			Load from to_string(name).txt folder
			HEAD
			BODY
			LEFTARM
			RIGHTARM
			LEFTLEG
			RIGHTLEG
			ACCESSORYONE
			ACCESSORYTWO
			ACCESSORYTHREE

			*/
			if (it->first == "HEAD")
				UpdateAnimation("HEAD", dt);
			else if (it->first == "BODY")
				UpdateAnimation("BODY", dt);
			else if (it->first == "LEFTARM")
				UpdateAnimation("LEFTARM", dt);
			else if (it->first == "RIGHTARM")
				UpdateAnimation("RIGHTARM", dt);
			else if (it->first == "LEFTLEG")
			{
				UpdateAnimation("LEFTLEG", dt);

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
			}
			else if (it->first == "RIGHTLEG")
			{
				UpdateAnimation("RIGHTLEG", dt);
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
			}
			else if (it->first == "ACCESSORYONE")
				UpdateAnimation("ACCESSORYONE", dt);
			else if (it->first == "ACCESSORYTWO")
			{
				UpdateAnimation("ACCESSORYTWO", dt);
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
			}
			else if (it->first == "ACCESSORYTHREE")
				UpdateAnimation("ACCESSORYTHREE", dt);
		}
	}
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
