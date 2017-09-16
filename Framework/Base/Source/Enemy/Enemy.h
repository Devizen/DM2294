#ifndef ENEMY_H
#define ENEMY_H

#include "../Attributes.h"
#include "Vector3.h"
#include <string>
#include <vector>
#include <map>

using std::string;

class Mesh;

class CEnemy : public CAttributes
{
public:
	enum TYPE
	{
		MONSTER = 0,
		HUMAN,
		NONE
	};
	enum TRANSLATEAXIS
	{
		X = 0,
		Y,
		Z
	};

	struct TRANSFORM
	{
		/*To offset vector together with animation count.*/
		unsigned count = 0;
		string transform = "";
		string axis = "";
		string value = "";
		string speed = "";
		bool check = false;
		//bool checkHead = false;
		//bool checkBody = false;
		//bool checkLeftArm = false;
		//bool checkRightArm = false;
		//bool checkLeftLeg = false;
		//bool checkRightLeg = false;
		//bool checkAccessoryOne = false;
		//bool checkAccessoryTwo = false;
		//bool checkAccessoryThree = false;
	};

	CEnemy();
	CEnemy(Mesh* _modelMesh);
	CEnemy(Mesh* _modelMesh, Vector3 _position);
	CEnemy(std::string _name,
		Mesh* _head, 
		Mesh* _body, 
		Mesh* _leftArm, 
		Mesh* _rightArm, 
		Mesh* _leftLeg, 
		Mesh* _rightLeg, 
		Mesh* _accessoryOne, 
		Mesh* _accessoryTwo, 
		Mesh* _accessoryThree, 
		Vector3 _position);
	~CEnemy();

	/*Set name for enemy.*/
	void SetName(string _name);
	/*Set position for enemy.*/
	void SetPosition(Vector3 _position);
	/*Set default position for enemy.*/
	void SetDefaultPosition(Vector3 _defaultPosition);
	/*Set model mesh for enemy.*/
	void SetModelMesh(string _meshName);
	/*Set activeBattle to trigger battle event.*/
	void SetBattleMode(bool _activeBattle);
	/*Set enemy type.*/
	void SetEnemyType(TYPE _type);
	/*Set health bar position for battle scene.*/
	void SetHealthBarPosition(float _healthBarPosition);

	/*Get the name.*/
	string GetName(void) const;
	/*Get the position.*/
	Vector3 GetPosition(void) const;
	/*Get the default position.*/
	Vector3 GetDefaultPosition(void) const;
	/*Get modelMesh.*/
	Mesh* GetModelMesh(void);
	/*Get activeBattle to check whether battle should be triggered.*/
	bool GetActiveBattle(void);
	/*Get enemy type.*/
	TYPE GetEnemyType(void);
	/*Get health bar position for battle scene.*/
	float GetHealthBarPosition(void);
	/*Get human model mesh.*/
	std::map<string, Mesh*>& GetHumanModelMesh(void);

	/*Update animation based on body parts.*/
	void UpdateAnimation(string _bodyPart, double dt);
	/*For translating model based on different body parts.*/
	bool TranslateModel(TYPE _type, string _bodyPart, TRANSLATEAXIS _axis, float _value, float _speed, double _dt);
	/*Reset the values in TRASFORM struct.*/
	void ResetCheckTransform(std::string _bodyPart);
	/*Render enemy.*/
	void Render(void);

	/*Update enemy.*/
	void Update(double dt);

	/*Load animations from text files.*/
	void Load(CEnemy* _enemy);
private:
	string name;
	Vector3 position;
	Vector3 defaultPosition;
	float rotate;
	Mesh* modelMesh;
	std::map<string, Mesh*>humanModelMesh;
	std::map<string, Vector3>translateHumanModel;
	std::map<string, std::pair<TRANSFORM, std::vector<string>>>translate;
	std::map<string, float>rotateHumanModel;
	std::map<string, unsigned>translateHumanModelState;
	std::map<string, unsigned>rotateHumanModelState;
	bool activateBattle;

	TRANSFORM checkTransform;

	/*To check what type of enemy.*/
	TYPE type;

	/*Placing the health bar in battle scene.*/
	float healthBarPosition;
protected:
};

namespace Create
{
	CEnemy* Enemy(const string& _meshName,
		const Vector3& _position = Vector3(0.f, 0.f, 0.f));
}

namespace Create
{
	CEnemy* Human(const string& _name,
		const string& _head,
		const string& _body,
		const string& _leftArm,
		const string& _rightArm,
		const string& _leftLeg,
		const string& _rightLeg,
		const Vector3& _position = Vector3(0.f, 0.f, 0.f),
		const string& _accessoryOne = "",
		const string& _accessoryTwo = "",
		const string& _accessoryThree = "");
}

#endif