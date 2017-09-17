#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include <map>
#include <vector>
#include "Vector3.h"

class Mesh;

class CAnimation
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
		std::string transform = "";
		std::string axis = "";
		std::string value = "";
		std::string speed = "";
		TRANSLATEAXIS eAxis;
		float amount = 0.f;
		bool check = false;
	};
	CAnimation();
	~CAnimation();

	/*Get human model mesh.*/
	std::map<std::string, Mesh*>& GetHumanModelMesh(void);

	/*Update animation based on body parts.*/
	void UpdateAnimation(std::string _bodyPart, double dt);
	/*For translating model based on different body parts.*/
	bool TranslateModel(TYPE _type, std::string _bodyPart, TRANSLATEAXIS _axis, float _value, float _speed, double _dt);
	/*Reset the values in TRASFORM struct.*/
	void ResetCheckTransform(std::string _bodyPart);
	std::map<std::string, Mesh*>humanModelMesh;
	std::map<std::string, Vector3>translateHumanModel;
	std::map<std::string, std::pair<TRANSFORM, std::vector<std::string>>>translate;
	std::map<std::string, float>rotateHumanModel;
	std::map<std::string, unsigned>translateHumanModelState;
	std::map<std::string, unsigned>rotateHumanModelState;
	/*To check what type of enemy.*/
	TYPE type;
private:
protected:
};

#endif