#ifndef FPS_CAMERA_H
#define FPS_CAMERA_H

#include "CameraBase.h"

class FPSCamera : public CameraBase
{
public:
	enum MOVE_CAMERA
	{
		MOVE_UP = 0,
		MOVE_DOWN,
		MOVE_LEFT,
		MOVE_RIGHT,
		MOVE_FORWARD,
		MOVE_BACKWARD
	};

	enum LOOK_CAMERA
	{
		LOOK_UP = 0,
		LOOK_DOWN,
		LOOK_LEFT,
		LOOK_RIGHT
	};
	FPSCamera();
	virtual ~FPSCamera();

	virtual Mtx44 GetViewMatrix();

	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();

	virtual Vector3 GetCameraPos() const;
	virtual void SetCameraPos(Vector3 pos);
	virtual Vector3 GetCameraTarget() const;
	virtual void SetCameraTarget(Vector3 pos);
	virtual Vector3 GetCameraUp() const;
	virtual void SetCameraUp(Vector3 pos);

	virtual void MoveCamera(float move, MOVE_CAMERA direction);
	virtual void LookCamera(float look, LOOK_CAMERA direction);

	virtual void SetOffsetToPlayer(Vector3 _playerPos);
	virtual Vector3 GetOffsetToPlayer(void);
private:
	Vector3 position;
	Vector3 target;
	Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	Vector3 offsetToPlayer;
};

#endif