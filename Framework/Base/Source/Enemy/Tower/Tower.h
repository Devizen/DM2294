#pragma once
#include "../Enemy3D.h"
#include <vector>

#define MAX_HEALTH_SCALE 10.f

using namespace std;

class Mesh;

class CTower : public CEnemy3D
{
protected:
	Mesh* modelMesh;

	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 target, up;
	Vector3 maxBoundary, minBoundary;
	GroundEntity* m_pTerrain;

	double m_dSpeed;
	double m_dAcceleration;
	//float m_fElapsedTimeBeforeUpdate;

public:

	CTower(Mesh* _modelMesh);
	virtual ~CTower();

	virtual void Init(void);
	// Reset this player instance to default
	virtual void Reset(void);
	// Update
	virtual void Update(double dt = 0.0333f);
	// Render
	virtual void Render(void);
private:

};

namespace Create
{
	CTower* Tower(const std::string& _meshName,
		const Vector3& _position,
		const float& _rotate,
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
		const bool& _playerProperty = false);
};
