#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "SingletonTemplate.h"
#include <list>
#include <string>
#include "Vector3.h"
#include "PlayerInfo\PlayerInfo.h"

using std::string;

class EntityBase;
class CCollider;
class CProjectile;
class CFurniture;
class CEnemy3D;
class CPlayerInfo;
class Patrol;

class EntityManager : public Singleton<EntityManager>
{
	friend Singleton<EntityManager>;
public:
	void Update(double _dt);
	void Render();
	void RenderUI();

	void AddEntity(EntityBase* _newEntity);
	void AddProjectile(CProjectile* _newEntity);
	void AddFixed(CFurniture* _newEntity);
	void AddPlayer(CPlayerInfo* _newEntity);
	void AddEnemy(CEnemy3D* _newEntity);
	void AddPatrolEnemy(Patrol* _newEntity);
	void AddItem(EntityBase* _newEntity);
	bool RemoveEntity(EntityBase* _existingEntity);
	bool RemoveEnemy(CEnemy3D* _existingEntity);

	bool CheckAABBCollision(Vector3 _minAABB, Vector3 _maxAABB, Vector3 _position);
	bool CheckRayAABB(Vector3 _direction, Vector3 _ray, CFurniture* _furniture);

	list<CFurniture*>&returnFixed(void);
	list<CEnemy3D*>&returnEnemy(void);
	/*Count how many enemies left.*/
	int enemyCount(void);

	/*Check Spawn Position*/
	bool getSpawnPosition(Vector3 _minAABB, Vector3 _maxAABB, Vector3 _position);

	/*Set Hit*/
	void setHit(bool _hit);

	/*Get Hit*/
	bool getHit(void);

	/*Remove all entities.*/
	void removeAllEntities(void);
private:
	EntityManager();
	virtual ~EntityManager();

	// Check for overlap
	bool CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB);
	// Check if this entity's bounding sphere collided with that entity's bounding sphere 

	bool CheckSphereCollision(CProjectile *ThisEntity, CFurniture *ThatEntity);
	// Check if this entity collided with another entity, but both must have collider
	//bool CheckAABBCollision(CPlayerInfo *ThisEntity/*, CFurniture *ThatEntity*/);
	// Check if any Collider is colliding with another Collider
	bool CheckForCollision(CCollider* thisEntity, CCollider* thatEntity);
	bool CheckProjectileCollision(CProjectile* thisEntity, CFurniture* thatEntity);
	bool CheckProjectileToPlayerCollision(CProjectile* thisEntity, CPlayerInfo* thatEntity);
	bool CheckProjectileToEnemyCollision(CProjectile* thisEntity, CEnemy3D* thatEntity);

	std::list<EntityBase*> entityList;
	std::list<CProjectile*> projectileList;
	std::list<CFurniture*> fixedList;
	std::list<CPlayerInfo*> playerList;
	std::list<CEnemy3D*> enemyList;
	std::list<EntityBase*>itemList;
	std::list<Patrol*>patrolList;

	bool hit;
};

#endif // ENTITY_MANAGER_H