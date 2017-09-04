#include "Enemy_Manager.h"
#include "Enemy.h"
#include <utility>

using std::pair;

CEnemy_Manager* CEnemy_Manager::s_instance = 0;

CEnemy_Manager::CEnemy_Manager()
{
}

CEnemy_Manager::~CEnemy_Manager()
{
}

CEnemy_Manager * CEnemy_Manager::GetInstance(void)
{
	if (!s_instance)
	{
		s_instance = new CEnemy_Manager();
		s_instance->enemyList = new map<string, CEnemy*>;
	}

	return s_instance;
}

void CEnemy_Manager::AddEnemy(CEnemy * _enemy)
{
	enemyList->insert(pair<string, CEnemy*>(_enemy->GetName(), _enemy));
}

map<string, CEnemy*>* CEnemy_Manager::GetEnemyList(void)
{
	return enemyList;
}

void CEnemy_Manager::Render(void)
{
	if (enemyList->size() <= 0)
		return;

	for (map<string, CEnemy*>::iterator it = enemyList->begin(); it != enemyList->end(); ++it)
	{
		if (it->second == nullptr)
			continue;

		it->second->Render();
	}
}

void CEnemy_Manager::Update(double dt)
{
	if (!enemyList)
		enemyList = new map<string, CEnemy*>;

	for (map<string, CEnemy*>::iterator it = enemyList->begin(); it != enemyList->end(); ++it)
	{
		if (it->second == nullptr)
			continue;

		it->second->Update(dt);
	}
}

void CEnemy_Manager::Destroy(void)
{
	for (map<string, CEnemy*>::iterator it = enemyList->begin(); it != enemyList->end();)
	{
		CEnemy* enemy = it->second;
		delete enemy;
		enemy = nullptr;
		it = enemyList->erase(it);
	}

	enemyList->clear();
	delete enemyList;
	enemyList = nullptr;
}
