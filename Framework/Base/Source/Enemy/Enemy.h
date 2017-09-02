#ifndef ENEMY_H
#define ENEMY_H

#include "../Attributes.h"
#include "Vector3.h"
#include <string>

using std::string;

class Mesh;

class CEnemy : public CAttributes
{
public:
	CEnemy();
	CEnemy(Mesh* _modelMesh);
	CEnemy(Mesh* _modelMesh, Vector3 _position);
	~CEnemy();

	/*Set name for enemy.*/
	void SetName(string _name);
	/*Set position for enemy.*/
	void SetPosition(Vector3 _position);
	/*Set default position for enemy.*/
	void SetDefaultPosition(Vector3 _defaultPosition);
	/*Set model mesh for enemy.*/
	void SetModelMesh(string _meshName);

	/*Get the name.*/
	string GetName(void) const;
	/*Get the position.*/
	Vector3 GetPosition(void) const;
	/*Get the default position.*/
	Vector3 GetDefaultPosition(void) const;
	/*Get modelMesh.*/
	Mesh* GetModelMesh(void);

	/*Render enemy.*/
	void Render(void);

	/*Update enemy.*/
	void Update(double dt);
private:
	string name;
	Vector3 position;
	Vector3 defaultPosition;
	float rotate;
	Mesh* modelMesh;
protected:
};

namespace Create
{
	CEnemy* Enemy(const string& _meshName,
		const Vector3& _position = Vector3(0.f, 0.f, 0.f));
}

#endif