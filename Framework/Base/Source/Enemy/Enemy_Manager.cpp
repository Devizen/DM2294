#include "Enemy_Manager.h"
#include "Enemy.h"
#include "../Player/Player.h"
#include "../Collision/Collision.h"
#include <utility>

#include <string>
using std::string;

/*Debugging purpose.*/
#include <iostream>
using std::cout;
using std::endl;

using std::pair;

CEnemy_Manager* CEnemy_Manager::s_instance = 0;

CEnemy_Manager::CEnemy_Manager() :
	createEnemy(false)
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
	string key = _enemy->GetName();
	key += std::to_string(enemyList->size());
	enemyList->insert(pair<string, CEnemy*>(key, _enemy));
}

map<string, CEnemy*>* CEnemy_Manager::GetEnemyList(void)
{
	return enemyList;
}

CPlayer * CEnemy_Manager::GetPlayer(void)
{
	return player;
}

void CEnemy_Manager::SetCreateEnemy(bool _createEnemy)
{
	createEnemy = _createEnemy;
}

void CEnemy_Manager::SetPlayer(CPlayer * _player)
{
	player = _player;
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

	if (createEnemy)
	{
		srand(static_cast<unsigned>(time(NULL)));
		int x = rand() % 171 + (-85);
		int z = rand() % 131 + (-65);
		Vector3 randomPosition(static_cast<float>(x), 0.f, static_cast<float>(z));
		static bool noCollision = false;
		while (!noCollision)
		{
			randomPosition.Set(static_cast<float>(x), 0.f, static_cast<float>(z));
			for (map<string, CEnemy*>::iterator thisEnemy = CEnemy_Manager::GetInstance()->GetEnemyList()->begin();
				thisEnemy != CEnemy_Manager::GetInstance()->GetEnemyList()->end();
				++thisEnemy)
			{
				if (!Check::AABB(randomPosition, thisEnemy->second->GetPosition()))
					noCollision = true;
				else
				{
					noCollision = false;
					break;
				}
			}

			if (enemyList->size() == 0)
				noCollision = true;

			if (Check::AABB(randomPosition, player->GetPosition()))
				noCollision = false;

			if (!noCollision)
			{
				x = rand() % 171 + (-85);
				z = rand() % 131 + (-65);
			}
		}
		Create::Enemy("ENEMY", randomPosition);
		noCollision = false;
		createEnemy = false;
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
