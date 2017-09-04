#include "Bullet.h"
#include "Bullet_Manager.h"
/*To get mesh.*/
#include "MeshBuilder.h"
/*To get modelStack.*/
#include "GraphicsManager.h"
/*To render mesh.*/
#include "RenderHelper.h"

CBullet::CBullet() :
	position(0.f, 0.f, 0.f)
	, damage(0)
	, speed(100.f)
	, active(false)
	, originated(NONE_ORIGINATED)
{
}

CBullet::~CBullet()
{
}

string CBullet::GetMeshName(void)
{
	return meshName;
}

Mesh * CBullet::GetModelMesh(void)
{
	return modelMesh;
}

Vector3 CBullet::GetPosition(void)
{
	return position;
}

int CBullet::GetDamage(void)
{
	return damage;
}

int CBullet::GetSpeed(void)
{
	return speed;
}

int CBullet::GetActive(void)
{
	return active;
}

CBullet::ORIGINATED CBullet::GetOriginated(void)
{
	return originated;
}

void CBullet::SetMeshName(string _meshName)
{
	meshName = _meshName;
	if (MeshBuilder::GetInstance()->GetMesh(meshName) == nullptr)
		modelMesh = MeshBuilder::GetInstance()->GetMesh("sphere");
	else
		modelMesh = MeshBuilder::GetInstance()->GetMesh(meshName);
}

void CBullet::SetModelMesh(Mesh * _modelMesh)
{
	modelMesh = _modelMesh;
}

void CBullet::SetPosition(Vector3 _position)
{
	position = _position;
}

void CBullet::SetDamage(int _damage)
{
	damage = _damage;
}

void CBullet::SetSpeed(float _speed)
{
	speed = _speed;
}

void CBullet::SetActive(bool _active)
{
	active = _active;
}

void CBullet::SetOriginated(ORIGINATED _originated)
{
	originated = _originated;
}

void CBullet::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	RenderHelper::GetInstance()->RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

void CBullet::Reset(void)
{
	meshName = "";
	position = Vector3(0.f, 0.f, 0.f);
	damage = 0;
	speed = 0;
	active = false;
	originated = NONE_ORIGINATED;
}

void CBullet::Update(double dt)
{
}

void Create::Bullet(string _meshName, Vector3 _position, CBullet::ORIGINATED _originated)
{
	for (vector<CBullet*>::iterator it = CBullet_Manager::GetInstance()->GetBulletList()->begin();
		it != CBullet_Manager::GetInstance()->GetBulletList()->end();
		++it)
	{
		CBullet* bullet = (CBullet*)*it;

		if (!bullet->GetActive())
		{
			bullet->SetMeshName(_meshName);
			bullet->SetPosition(_position);
			bullet->SetOriginated(_originated);
			bullet->SetActive(true);
			break;
		}
	}
}
