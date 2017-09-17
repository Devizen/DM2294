#include "Animation.h"
#include "Mesh.h"

CAnimation::CAnimation()
	: type(NONE)
{
}

CAnimation::~CAnimation()
{
}

std::map<std::string, Mesh*>& CAnimation::GetHumanModelMesh(void)
{
	return humanModelMesh;
}

void CAnimation::UpdateAnimation(std::string _bodyPart, double dt)
{
	/*Prevent program from crashing when the body part is not used.*/
	if (humanModelMesh[_bodyPart] == nullptr)
		return;

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
	if (translate[_bodyPart].first.axis == "x")
		translate[_bodyPart].first.eAxis = X;
	else if (translate[_bodyPart].first.axis == "y")
		translate[_bodyPart].first.eAxis = Y;
	else
		translate[_bodyPart].first.eAxis = Z;

	/*Keep translating the model until the value is met.*/
	if (TranslateModel(type, _bodyPart, translate[_bodyPart].first.eAxis, std::stof(translate[_bodyPart].first.value), std::stof(translate[_bodyPart].first.speed), dt))
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

bool CAnimation::TranslateModel(TYPE _type, std::string _bodyPart, TRANSLATEAXIS _axis, float _value, float _speed, double _dt)
{
	float dt = static_cast<float>(_dt);
	if (_type == HUMAN)
	{
		if (_value > 0.f)
		{
			translate[_bodyPart].first.amount += dt * _speed;
			if (_axis == X)
				translateHumanModel[_bodyPart].x += dt * _speed;
			else if (_axis == Y)
				translateHumanModel[_bodyPart].y += dt * _speed;
			else
				translateHumanModel[_bodyPart].z += dt * _speed;

			if (translate[_bodyPart].first.amount >= _value)
				return true;
			else
				return false;
		}
		else
		{
			translate[_bodyPart].first.amount -= dt * _speed;
			if (_axis == X)
				translateHumanModel[_bodyPart].x -= dt * _speed;
			else if (_axis == Y)
				translateHumanModel[_bodyPart].y -= dt * _speed;
			else
				translateHumanModel[_bodyPart].z -= dt * _speed;

			if (translate[_bodyPart].first.amount <= _value)
				return true;
			else
				return false;
		}
	}
	return true;
}

void CAnimation::ResetCheckTransform(std::string _bodyPart)
{
	translate[_bodyPart].first.transform = "";
	translate[_bodyPart].first.axis = "";
	translate[_bodyPart].first.value = "";
	translate[_bodyPart].first.speed = "";
	translate[_bodyPart].first.amount = 0.f;
	translate[_bodyPart].first.check = false;
}