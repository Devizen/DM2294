#ifndef ENEMY_H
#define ENEMY_H

#include "../Attributes.h"
#include "../Animation/Animation.h"
#include "Vector3.h"
#include <string>
#include <vector>
#include <map>

class Mesh;

class CEnemy : public CAttributes, public CAnimation
{
public:
	//enum TYPE
	//{
	//	MONSTER = 0,
	//	HUMAN,
	//	NONE
	//};
	//enum TRANSLATEAXIS
	//{
	//	X = 0,
	//	Y,
	//	Z
	//};

	//struct TRANSFORM
	//{
	//	/*To offset vector together with animation count.*/
	//	unsigned count = 0;
	//	std::string transform = "";
	//	std::string axis = "";
	//	std::string value = "";
	//	std::string speed = "";
	//	TRANSLATEAXIS eAxis;
	//	float amount = 0.f;
	//	bool check = false;
	//	//bool checkHead = false;
	//	//bool checkBody = false;
	//	//bool checkLeftArm = false;
	//	//bool checkRightArm = false;
	//	//bool checkLeftLeg = false;
	//	//bool checkRightLeg = false;
	//	//bool checkAccessoryOne = false;
	//	//bool checkAccessoryTwo = false;
	//	//bool checkAccessoryThree = false;
	//};

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
	void SetName(std::string _name);
	/*Set position for enemy.*/
	void SetPosition(Vector3 _position);
	/*Set default position for enemy.*/
	void SetDefaultPosition(Vector3 _defaultPosition);
	/*Set model mesh for enemy.*/
	void SetModelMesh(std::string _meshName);
	/*Set activeBattle to trigger battle event.*/
	void SetBattleMode(bool _activeBattle);
	/*Set enemy type.*/
	void SetEnemyType(TYPE _type);
	/*Set health bar position for battle scene.*/
	void SetHealthBarPosition(float _healthBarPosition);

	/*Get the name.*/
	std::string GetName(void) const;
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
	///*Get human model mesh.*/
	//std::map<std::string, Mesh*>& GetHumanModelMesh(void);

	///*Update animation based on body parts.*/
	//void UpdateAnimation(std::string _bodyPart, double dt);
	///*For translating model based on different body parts.*/
	//bool TranslateModel(TYPE _type, std::string _bodyPart, TRANSLATEAXIS _axis, float _value, float _speed, double _dt);
	///*Reset the values in TRASFORM struct.*/
	//void ResetCheckTransform(std::string _bodyPart);
	/*Render enemy.*/
	void Render(void);
	/*Render individual body parts to prevent delay.*/
	void Render(std::string _bodyPart);

	/*Update enemy.*/
	void Update(double dt);

	/*Load animations from text files.*/
	void Load(CEnemy* _enemy);
private:
	std::string name;
	Vector3 position;
	Vector3 defaultPosition;
	Vector3 minAABB;
	Vector3 maxAABB;
	float rotate;
	Mesh* modelMesh;
	//std::map<std::string, Mesh*>humanModelMesh;
	//std::map<std::string, Vector3>translateHumanModel;
	//std::map<std::string, std::pair<TRANSFORM, std::vector<std::string>>>translate;
	//std::map<std::string, float>rotateHumanModel;
	//std::map<std::string, unsigned>translateHumanModelState;
	//std::map<std::string, unsigned>rotateHumanModelState;
	bool activateBattle;

	TRANSFORM checkTransform;

	///*To check what type of enemy.*/
	//TYPE type;

	/*Placing the health bar in battle scene.*/
	float healthBarPosition;
protected:
};

namespace Create
{
	CEnemy* Enemy(const std::string& _meshName,
		const Vector3& _position = Vector3(0.f, 0.f, 0.f));
}

namespace Create
{
	CEnemy* Human(const std::string& _name,
		const std::string& _head,
		const std::string& _body,
		const std::string& _leftArm,
		const std::string& _rightArm,
		const std::string& _leftLeg,
		const std::string& _rightLeg,
		const Vector3& _position = Vector3(0.f, 0.f, 0.f),
		const std::string& _accessoryOne = "",
		const std::string& _accessoryTwo = "",
		const std::string& _accessoryThree = "");
}

#endif