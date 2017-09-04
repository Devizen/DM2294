#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include <map>
#include <string>

using std::map;
using std::string;

class CEnemy;
class CPlayer;

class CEnemy_Manager
{
	static CEnemy_Manager* s_instance;

public:
	CEnemy_Manager();
	~CEnemy_Manager();

	static CEnemy_Manager* GetInstance(void);

	/*Add an enemy to the enemy list.*/
	void AddEnemy(CEnemy* _enemy);
	/*Get the Enemy List.*/
	map<string, CEnemy*>*GetEnemyList(void);
	/*Get player for checking against enemy purposes.*/
	CPlayer* GetPlayer(void);
	/*Set createEnemy to start creating enemy on map.*/
	void SetCreateEnemy(bool _createEnemy);
	/*Set player to allow enemy to check against player.*/
	void SetPlayer(CPlayer* _player);

	/*Render enemy in list.*/
	void Render(void);

	/*Update enemy in list.*/
	void Update(double dt);

	/*Remove everything from CEnemy_Manager.*/
	void Destroy(void);

private:
	CPlayer* player;
	map<string, CEnemy*>*enemyList;
	bool createEnemy;

protected:

};

#endif