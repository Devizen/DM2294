#include "Cinematic.h"
#include "MouseController.h"
#include "KeyboardController.h"

#include <iostream>

using std::cout;
using std::endl;

CCinematic::CCinematic() :
	defaultPosition(0.f, 0.f, 0.f)
	, defaultTarget(0.f, 0.f, 1.f)
	, defaultUp(0.f, 1.f, 0.f)
	,position(0.f, 0.f, 0.f)
	, target(0.f, 0.f, 1.f)
	, up(0.f, 1.f, 0.f)
	, numberOfPositions(0)
{
}

CCinematic::~CCinematic()
{
}

Mtx44 CCinematic::GetViewMatrix()
{
	Mtx44 result;
	result.SetToLookAt(position.x, position.y, position.z,
		target.x, target.y, target.z,
		up.x, up.y, up.z);
	return result;
}

void CCinematic::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void CCinematic::Update(double dt)
{
	static const float CAMERA_SPEED = 200.f;

	double mouse_diff_x, mouse_diff_y;
	MouseController::GetInstance()->GetMouseDelta(mouse_diff_x, mouse_diff_y);

	double camera_yaw = mouse_diff_x * 0.0174555555555556;		// 3.142 / 180.0
	double camera_pitch = mouse_diff_y * 0.0174555555555556;	// 3.142 / 180.0

	{
		Vector3 view = (target - position).Normalized();
		float yaw = (float)(-CAMERA_SPEED * camera_yaw * (float)dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
	}
	{
		float pitch = (float)(-CAMERA_SPEED * camera_pitch * (float)dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
	}
	if (KeyboardController::GetInstance()->IsKeyDown('R'))
	{
		Reset();
	}
}

Vector3 CCinematic::GetCameraPos() const
{
	return position;
}

void CCinematic::SetCameraPos(Vector3 pos)
{
	position = pos;
}
Vector3 CCinematic::GetCameraTarget() const
{
	return target;
}
void CCinematic::SetCameraTarget(Vector3 target)
{
	this->target = target;
}
Vector3 CCinematic::GetCameraUp() const
{
	return up;
}
void CCinematic::SetCameraUp(Vector3 up)
{
	this->up = up;
}

void CCinematic::moveCamera(Vector3 _position, Vector3 _destination, float _speed, double dt)
{
	Vector3 _target = _destination - _position;
	cout << "Position: " << _position << endl;
	cout << "Destination: " << _destination << endl;
	target = Vector3(0.f, 0.f, 0.f);
	try
	{
		cout << "Displacement: " << (_destination - _position).Length() << endl;
		if ((_destination - _position).Length() >= 50.f)
			position += _target.Normalized() * _speed * (float)dt;
		else 
			++numberOfPositions;
	}
	catch (exception e)
	{

	}
}

void CCinematic::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}