#ifndef HEALTH_H
#define HEALTH_H

#include "Collider\Collider.h"
#include "EntityBase.h"
#include "Vector3.h"

class Mesh;

class CHealth : public EntityBase/*,  public CCollider*/
{
public:
	CHealth();
	CHealth(Mesh* _modelMesh);
	~CHealth();

	virtual void Render();

	// Set the position and scale of the furniture.
	void Set(Vector3 theNewPosition, Vector3 theNewScale);
	// Activate the projectile. true == active, false == inactive
	void SetStatus(const bool m_bStatus);

private:
	float angleToFace;

protected:
	// The model mesh for this furniture
	Mesh* modelMesh;
	// Boolean flag to indicate if this furniture is active. If not active, then do not compute/update
	bool m_bStatus;
};

namespace Create
{
	CHealth* Health(const std::string& _meshName,
		const Vector3& _position,
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));
};

#endif