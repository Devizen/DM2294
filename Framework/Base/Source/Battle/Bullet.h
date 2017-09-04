#ifndef BULLET_H
#define BULLET_H

#include "Vector3.h"
#include <string>

using std::string;
class Mesh;

class CBullet
{
public:
	CBullet();
	~CBullet();

	enum ORIGINATED
	{
		ENEMY_ORIGINATED = 0,
		PLAYER_ORIGINATED,
		NONE_ORIGINATED
	};

	/*Get Mesh Name.*/
	string GetMeshName(void);
	/*Get Model Mesh.*/
	Mesh* GetModelMesh(void);
	/*Get Position.*/
	Vector3 GetPosition(void);
	/*Get Damage.*/
	int GetDamage(void);
	/*Get Speed.*/
	int GetSpeed(void);
	/*Get Active.*/
	int GetActive(void);
	/*Get Originated.*/
	ORIGINATED GetOriginated(void);
	/*Set Mesh Name.*/
	void SetMeshName(string _meshName);
	/*Set Model Mesh.*/
	void SetModelMesh(Mesh* _modelMesh);
	/*Set Position.*/
	void SetPosition(Vector3 _position);
	/*Set Damage.*/
	void SetDamage(int _damage);
	/*Set Speed.*/
	void SetSpeed(float _speed);
	/*Set Active.*/
	void SetActive(bool _active);
	/*Set Originated.*/
	void SetOriginated(ORIGINATED _originated);

	void Render(void);
	void Reset(void);
	void Update(double dt);
private:
	string meshName;
	Mesh* modelMesh;
	Vector3 position;
	int damage;
	float speed;
	bool active;
	ORIGINATED originated;
protected:
};

namespace Create
{
	void Bullet(string _meshName = "BULLET"
		, Vector3 _position = Vector3(0.f,0.f, 0.f)
		, CBullet::ORIGINATED _originated = CBullet::NONE_ORIGINATED);
}

#endif