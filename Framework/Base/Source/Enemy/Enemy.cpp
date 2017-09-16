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

	/*Testing on string.*/
	translate["HEAD"].second.push_back("translate,z,10,50");
	translate["HEAD"].second.push_back("translate,z,-10,50");
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
			{
				/*Real time loop control. Only check when the values differ.*/
				if (!translate["HEAD"].first.check)
				{
					/*Used for going to next word.*/
					unsigned wordCount = 0;

					for (unsigned i = 0; i < translate["HEAD"].second[translate["HEAD"].first.count].size(); ++i)
					{
						/*Skip to next word when , is found.*/
						if (translate["HEAD"].second[translate["HEAD"].first.count][i] == ',')
						{
							++wordCount;
							continue;
						}

						/*First word is for checking whether the transformation is translate or rotate.*/
						if (wordCount == 0)
							translate["HEAD"].first.transform += translate["HEAD"].second[translate["HEAD"].first.count][i];

						/*Second word is for checking which axis to transform.*/
						else if (wordCount == 1)
							translate["HEAD"].first.axis += translate["HEAD"].second[translate["HEAD"].first.count][i];

						/*Third word is for transforming to a certain value.*/
						else if (wordCount == 2)
							translate["HEAD"].first.value += translate["HEAD"].second[translate["HEAD"].first.count][i];

						/*Fourth word is for the speed of transformation.*/
						else if (wordCount == 3)
							translate["HEAD"].first.speed += translate["HEAD"].second[translate["HEAD"].first.count][i];
					}
					/*Conclude that all the values have been checked.*/
					translate["HEAD"].first.check = true;
				}
				/*Assign a TRANSLATEAXIS value to use for TranslateModel function.*/
				TRANSLATEAXIS axis;
				if (translate["HEAD"].first.axis == "x")
					axis = X;
				else if (translate["HEAD"].first.axis == "y")
					axis = Y;
				else
					axis = Z;

				/*Keep translating the model until the value is met.*/
				if (TranslateModel(type, "HEAD", axis, std::stof(translate["HEAD"].first.value), std::stof(translate["HEAD"].first.speed), dt))
					/*Revert back to first animation if the animation count is at the last animation.*/
					if (translate["HEAD"].second.size() == translate["HEAD"].first.count + 1)
					{
						translate["HEAD"].first.count = 0;
						/*Start checking again.*/
						ResetCheckTransform("HEAD");
					}
					/*Go to next animation if the animation count is not the last animation.*/
					else
					{
						++translate["HEAD"].first.count;
						/*Start checking again.*/
						ResetCheckTransform("HEAD");
					}
			}
			else if (it->first == "BODY")
			{
				if (translateHumanModelState["BODY"] == 0)
				{
					translateHumanModel["BODY"].z += static_cast<float>(dt) * 50.f;
					if (translateHumanModel["BODY"].z >= 10.f)
						translateHumanModelState["BODY"] = 1;
				}
				else if (translateHumanModelState["BODY"] == 1)
				{
					translateHumanModel["BODY"].z -= static_cast<float>(dt) * 50.f;
					if (translateHumanModel["BODY"].z <= -10.f)
						translateHumanModelState["BODY"] = 0;
				}
			}
			else if (it->first == "LEFTARM")
			{
				if (translateHumanModelState["LEFTARM"] == 0)
				{
					translateHumanModel["LEFTARM"].z += static_cast<float>(dt) * 50.f;
					if (translateHumanModel["LEFTARM"].z >= 10.f)
						translateHumanModelState["LEFTARM"] = 1;
				}
				else if (translateHumanModelState["LEFTARM"] == 1)
				{
					translateHumanModel["LEFTARM"].z -= static_cast<float>(dt) * 50.f;
					if (translateHumanModel["LEFTARM"].z <= -10.f)
						translateHumanModelState["LEFTARM"] = 0;
				}
			}
			else if (it->first == "RIGHTARM")
			{
				if (translateHumanModelState["RIGHTARM"] == 0)
				{
					translateHumanModel["RIGHTARM"].z += static_cast<float>(dt) * 50.f;
					if (translateHumanModel["RIGHTARM"].z >= 10.f)
						translateHumanModelState["RIGHTARM"] = 1;
				}
				else if (translateHumanModelState["RIGHTARM"] == 1)
				{
					translateHumanModel["RIGHTARM"].z -= static_cast<float>(dt) * 50.f;
					if (translateHumanModel["RIGHTARM"].z <= -10.f)
						translateHumanModelState["RIGHTARM"] = 0;
				}
			}
			else if (it->first == "LEFTLEG")
			{
				if (translateHumanModelState["LEFTLEG"] == 0)
				{
					translateHumanModel["LEFTLEG"].z += static_cast<float>(dt) * 50.f;
					if (translateHumanModel["LEFTLEG"].z >= 10.f)
						translateHumanModelState["LEFTLEG"] = 1;
				}
				else if (translateHumanModelState["LEFTLEG"] == 1)
				{
					translateHumanModel["LEFTLEG"].z -= static_cast<float>(dt) * 50.f;
					if (translateHumanModel["LEFTLEG"].z <= -10.f)
						translateHumanModelState["LEFTLEG"] = 0;
				}

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
				if (translateHumanModelState["RIGHTLEG"] == 0)
				{
					translateHumanModel["RIGHTLEG"].z += static_cast<float>(dt) * 50.f;
					if (translateHumanModel["RIGHTLEG"].z >= 10.f)
						translateHumanModelState["RIGHTLEG"] = 1;
				}
				else if (translateHumanModelState["RIGHTLEG"] == 1)
				{
					translateHumanModel["RIGHTLEG"].z -= static_cast<float>(dt) * 50.f;
					if (translateHumanModel["RIGHTLEG"].z <= -10.f)
						translateHumanModelState["RIGHTLEG"] = 0;
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
			}
			else if (it->first == "ACCESSORYONE")
			{

				if (translateHumanModelState["ACCESSORYONE"] == 0)
				{
					translateHumanModel["ACCESSORYONE"].z += static_cast<float>(dt) * 50.f;
					if (translateHumanModel["ACCESSORYONE"].z >= 10.f)
						translateHumanModelState["ACCESSORYONE"] = 1;
				}
				else if (translateHumanModelState["ACCESSORYONE"] == 1)
				{
					translateHumanModel["ACCESSORYONE"].z -= static_cast<float>(dt) * 50.f;
					if (translateHumanModel["ACCESSORYONE"].z <= -10.f)
						translateHumanModelState["ACCESSORYONE"] = 0;
				}
			}
			else if (it->first == "ACCESSORYTWO")
			{
				if (translateHumanModelState["ACCESSORYTWO"] == 0)
				{
					translateHumanModel["ACCESSORYTWO"].z += static_cast<float>(dt) * 50.f;
					if (translateHumanModel["ACCESSORYTWO"].z >= 10.f)
						translateHumanModelState["ACCESSORYTWO"] = 1;
				}
				else if (translateHumanModelState["ACCESSORYTWO"] == 1)
				{
					translateHumanModel["ACCESSORYTWO"].z -= static_cast<float>(dt) * 50.f;
					if (translateHumanModel["ACCESSORYTWO"].z <= -10.f)
						translateHumanModelState["ACCESSORYTWO"] = 0;
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
			}
			else if (it->first == "ACCESSORYTHREE")
			{
				if (translateHumanModelState["ACCESSORYTHREE"] == 0)
				{
					translateHumanModel["ACCESSORYTHREE"].z += static_cast<float>(dt) * 50.f;
					if (translateHumanModel["ACCESSORYTHREE"].z >= 10.f)
						translateHumanModelState["ACCESSORYTHREE"] = 1;
				}
				else if (translateHumanModelState["ACCESSORYTHREE"] == 1)
				{
					translateHumanModel["ACCESSORYTHREE"].z -= static_cast<float>(dt) * 50.f;
					if (translateHumanModel["ACCESSORYTHREE"].z <= -10.f)
						translateHumanModelState["ACCESSORYTHREE"] = 0;
				}
			}
		}
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
