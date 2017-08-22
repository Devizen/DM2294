#ifndef FURNITURE_H
#define FURNITURE_H

#include "Collider\Collider.h"
#include "EntityBase.h"
#include "Vector3.h"

#include <string>

using std::string;

class Mesh;

class CFurniture : public EntityBase/*,  public CCollider*/
{
public:
	CFurniture();
	CFurniture(Mesh* _modelMesh);
	~CFurniture();

	virtual void Render();

	// Set the position and scale of the furniture.
	void Set(Vector3 theNewPosition, Vector3 theNewScale);
	// Activate the projectile. true == active, false == inactive
	void SetStatus(const bool m_bStatus);
	
	// setting name
	void SetName(const string newName);
	//getting name
	string GetName();

private:
	//mesh name
	string name;
protected:
	// The model mesh for this furniture
	Mesh* modelMesh;
	// Boolean flag to indicate if this furniture is active. If not active, then do not compute/update
	bool m_bStatus;
	
	
};

namespace Create
{
	CFurniture* Furniture(const std::string& _meshName,
		const Vector3& _position,
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));
};

#endif