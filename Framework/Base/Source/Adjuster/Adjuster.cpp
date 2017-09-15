#include "Adjuster.h"
#include "KeyboardController.h"

/*Debugging purpose.*/
#include <iostream>
using std::cout;
using std::endl;

CAdjuster* CAdjuster::s_instance = 0;

CAdjuster::CAdjuster() :
	position(0.f, 0.f, 0.f)
	, scale(0.f, 0.f, 0.f)
	, mode(TRANSLATE_MODE)
	, speed(0.1f)
{
}

CAdjuster::~CAdjuster()
{
}

void CAdjuster::Init(void)
{
}

void CAdjuster::Update(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_LCONTROL))
	{
		if (mode == TRANSLATE_MODE)
			mode = SCALE_MODE;
		else if (mode == SCALE_MODE)
			mode = TRANSLATE_MODE;
	}
	if (mode == TRANSLATE_MODE)
	{
		if (KeyboardController::GetInstance()->IsKeyDown(VK_LEFT))
			position.x += (static_cast<float>(dt) * speed);

		if (KeyboardController::GetInstance()->IsKeyDown(VK_RIGHT))
			position.x -= (static_cast<float>(dt) * speed);

		if (KeyboardController::GetInstance()->IsKeyDown(VK_UP))
			position.y -= (static_cast<float>(dt) * speed);

		if (KeyboardController::GetInstance()->IsKeyDown(VK_DOWN))
			position.y += (static_cast<float>(dt) * speed);

		cout << "Translate X: " << position.x << endl;
		cout << "Translate Y: " << position.y << endl;
	}
	else if (mode == SCALE_MODE)
	{
		if (KeyboardController::GetInstance()->IsKeyDown(VK_LEFT))
			scale.x -= (static_cast<float>(dt) * speed);

		if (KeyboardController::GetInstance()->IsKeyDown(VK_RIGHT))
			scale.x += (static_cast<float>(dt) * speed);

		if (KeyboardController::GetInstance()->IsKeyDown(VK_UP))
			scale.y += (static_cast<float>(dt) * speed);

		if (KeyboardController::GetInstance()->IsKeyDown(VK_DOWN))
			scale.y -= (static_cast<float>(dt) * speed);

		cout << "Scale X: " << scale.x << endl;
		cout << "Scale Y: " << scale.y << endl;
	}
}

void CAdjuster::SetSpeed(float _speed)
{
	speed = _speed;
}

Vector3 CAdjuster::GetPosition(void)
{
	return position;
}

Vector3 CAdjuster::GetScale(void)
{
	return scale;
}

CAdjuster * CAdjuster::GetInstance(void)
{
	if (!s_instance)
		s_instance = new CAdjuster();

	return s_instance;
}

void CAdjuster::DestroyAll(void)
{
	if (s_instance)
	{
		delete s_instance;
		s_instance = nullptr;
	}
}
