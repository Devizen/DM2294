#ifndef GENERIC_ENTITY_H
#define GENERIC_ENTITY_H

#include "EntityBase.h"
#include <string>
#include "Collider/Collider.h"

class Mesh;
class FPSCamera;

class GenericEntity : public EntityBase/*, public CCollider*/
{
public:
	GenericEntity(Mesh* _modelMesh);
	virtual ~GenericEntity();

	virtual void Update(double _dt);
	virtual void Render();
	virtual void RenderToScreen(Mesh* _mesh);

	// Set the maxAABB and minAABB
	void SetAABB(Vector3 maxAABB, Vector3 minAABB);

private:
	Mesh* modelMesh;
};

namespace Create
{
	GenericEntity* Entity(	const std::string& _meshName, 
							const Vector3& _position, 
							const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
							const float& _rotate = 0.f,
							const Vector3& _rotationAxis = Vector3(0.f, 1.f, 0.f));
};

#endif // GENERIC_ENTITY_H