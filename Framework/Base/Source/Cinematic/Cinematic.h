#ifndef CINEMATIC_H
#define CINEMATIC_H

#include "CameraBase.h"

class CCinematic : public CameraBase, public std::exception
{
public:
	CCinematic();
	virtual ~CCinematic();

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

	void moveCamera(Vector3 _position, Vector3 _destination, float _speed, double dt);

	int numberOfPositions;

private:
	Vector3 position;
	Vector3 target;
	Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;
};

#endif