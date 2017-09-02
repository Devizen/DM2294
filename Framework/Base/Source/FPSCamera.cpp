#include "FPSCamera.h"
#include "MouseController.h"
#include "KeyboardController.h"

FPSCamera::FPSCamera()
{
}

FPSCamera::~FPSCamera()
{
}

Mtx44 FPSCamera::GetViewMatrix()
{
	Mtx44 result;
	result.SetToLookAt(position.x, position.y, position.z,
		target.x, target.y, target.z,
		up.x, up.y, up.z);
	return result;
}

void FPSCamera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void FPSCamera::Update(double dt)
{
	static const float CAMERA_SPEED = 200.f;

	double mouse_diff_x, mouse_diff_y;
	MouseController::GetInstance()->GetMouseDelta(mouse_diff_x, mouse_diff_y);

	double camera_yaw = mouse_diff_x * 0.0174555555555556;		// 3.142 / 180.0
	double camera_pitch = mouse_diff_y * 0.0174555555555556;	// 3.142 / 180.0

	//Update the camera direction based on mouse move
	// left-right rotate
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

}

Vector3 FPSCamera::GetCameraPos() const
{
	return position;
}

void FPSCamera::SetCameraPos(Vector3 pos)
{
	position = pos;
}
Vector3 FPSCamera::GetCameraTarget() const
{
	return target;
}
void FPSCamera::SetCameraTarget(Vector3 target)
{
	this->target = target;
}
Vector3 FPSCamera::GetCameraUp() const
{
	return up;
}
void FPSCamera::SetCameraUp(Vector3 up)
{
	this->up = up;
}

void FPSCamera::MoveCamera(float move, MOVE_CAMERA direction)
{
	if (direction == MOVE_UP)
	{
		position.y += static_cast<float>(move);
		target.y += static_cast<float>(move);
	}
	else if (direction == MOVE_DOWN)
	{
		position.y -= static_cast<float>(move);
		target.y -= static_cast<float>(move);
	}
	else if (direction == MOVE_LEFT)
	{
		position.x -= static_cast<float>(move);
		target.x -= static_cast<float>(move);
	}
	else if (direction == MOVE_RIGHT)
	{
		position.x += static_cast<float>(move);
		target.x += static_cast<float>(move);
	}
	/*Z Axis is flipped.*/
	else if (direction == MOVE_FORWARD)
	{
		position.z -= static_cast<float>(move);
		target.z -= static_cast<float>(move);
	}
	else if (direction == MOVE_BACKWARD)
	{
		position.z += static_cast<float>(move);
		target.z += static_cast<float>(move);
	}
}

void FPSCamera::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}