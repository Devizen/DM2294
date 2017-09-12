#include "Bullet_Manager.h"
#include "Bullet.h"
#include "../Application.h"
/*Debugging purpose.*/
#include <iostream>

using std::cout;
using std::endl;

CBullet_Manager* CBullet_Manager::s_instance = 0;

CBullet_Manager * CBullet_Manager::GetInstance(void)
{
	if (!s_instance)
		s_instance = new CBullet_Manager();

	return s_instance;
}

void CBullet_Manager::AddBullet(int _amount)
{
	if (bulletList == nullptr)
		bulletList = new vector<CBullet*>;

	for (int i = 0; i < _amount; ++i)
	{
		CBullet* bullet = new CBullet();
		bulletList->push_back(bullet);
	}
}

void CBullet_Manager::Render(void)
{
	for (vector<CBullet*>::iterator it = bulletList->begin(); it != bulletList->end(); ++it)
	{
		if (!(*it)->GetActive())
			break;

		(*it)->Render();
	}
}

void CBullet_Manager::Update(double dt)
{
	for (vector<CBullet*>::iterator it = bulletList->begin(); it != bulletList->end(); ++it)
	{
		if (!(*it)->GetActive())
			break;

		if ((*it)->GetOriginated() == CBullet::ORIGINATED::ENEMY_ORIGINATED)
			(*it)->SetPosition(Vector3((*it)->GetPosition().x, (*it)->GetPosition().y, (*it)->GetPosition().z + (static_cast<float>(dt) * (*it)->GetSpeed())));
		if ((*it)->GetOriginated() == CBullet::ORIGINATED::PLAYER_ORIGINATED)
			(*it)->SetPosition(Vector3((*it)->GetPosition().x, (*it)->GetPosition().y, (*it)->GetPosition().z - (static_cast<float>(dt) * (*it)->GetSpeed())));

		if ((*it)->GetPosition().z > Application::GetInstance().GetWindowHeight())
			(*it)->Reset();
	}
}

vector<CBullet*>* CBullet_Manager::GetBulletList(void)
{
	return bulletList;
}

void CBullet_Manager::DestroyAll(void)
{
	while (bulletList->size() > 0)
	{
		CBullet* bullet = bulletList->back();
		delete bullet;
		bullet = nullptr;
		bulletList->pop_back();
	}

	if (bulletList)
	{
		delete bulletList;
		bulletList = nullptr;
	}

	if (s_instance)
	{
		delete s_instance;
		s_instance = nullptr;
	}
}
