#include "EntityManager.h"
#include "EntityBase.h"
#include "Collider/Collider.h"
#include "Projectile\Projectile.h"
#include "Object\Furniture.h"
#include "SpriteEntity.h"
#include "Enemy\Enemy3D.h"
#include "PlayerInfo\PlayerInfo.h"
#include "CameraEffects\CameraEffects.h"
#include "SoundEngine.h"

/*Min and Max*/
#include <algorithm>

#include <iostream>
using namespace std;

// Update all entities
void EntityManager::Update(double _dt)
{
	static float displayHit = 0.f;

	/*Checking projectile*/
	for (list<CProjectile*>::iterator thisObj = projectileList.begin(); thisObj != projectileList.end(); ++thisObj)
	{
		CProjectile* bullet = (CProjectile*)*thisObj;
		if (!bullet->GetStatus())
			continue;

		for (list<CFurniture*>::iterator thatObj = fixedList.begin(); thatObj != fixedList.end(); ++thatObj)
		{
			CFurniture* furniture = (CFurniture*)*thatObj;
			if (CheckProjectileCollision(bullet, furniture))
			{
				bullet->SetStatus(false);
				hit = true;
			}
			else
				continue;

		}


		for (list<CPlayerInfo*>::iterator playerObj = playerList.begin(); playerObj != playerList.end(); ++playerObj)
		{
			CPlayerInfo* player = (CPlayerInfo*)*playerObj;
			if (CheckProjectileToPlayerCollision(bullet, player))
			{
				CSoundEngine::GetInstance()->PlayASound("TAKEDAMAGE");
				player->deductHealthBy(1.f);
				player->setTookDamage(true);
				bullet->SetStatus(false);
				player->setTookDamage(false);
				CCameraEffects::GetInstance()->SetStatus_BloodScreen(true);
			}
			else
				continue;

		}

		for (list<CEnemy3D*>::iterator enemyObj = enemyList.begin(); enemyObj != enemyList.end(); ++enemyObj)
		{
			CEnemy3D* enemy = (CEnemy3D*)*enemyObj;
			if (CheckProjectileToEnemyCollision(bullet, enemy))
			{
				enemy->setHealth(enemy->getHealth() - 1);
				--enemy->attributes.HEALTH;
				bullet->SetStatus(false);
				hit = true;
			}
			else
				continue;

		}
	}

	for (list<CEnemy3D*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
	{
		CEnemy3D* enemy = (CEnemy3D*)*it;
		enemy->Update(_dt);
	}

	for (list<CProjectile*>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
	{
		CProjectile* projectile = (CProjectile*)*it;
		projectile->Update(_dt);
	}

	for (list<EntityBase*>::iterator it = itemList.begin(); it != itemList.end(); ++it)
	{
		EntityBase* item = (EntityBase*)*it;

		Vector3 itemMin = item->GetMinAABB() + Vector3(item->GetPosition().x, -5.f, item->GetPosition().z);
		Vector3 itemMax = item->GetMaxAABB() + Vector3(item->GetPosition().x, -5.f, item->GetPosition().z);

		Vector3 playerMin = CPlayerInfo::GetInstance()->GetMinAABB() + Vector3(CPlayerInfo::GetInstance()->GetPos().x, -5.f, CPlayerInfo::GetInstance()->GetPos().z);
		Vector3 playerMax = CPlayerInfo::GetInstance()->GetMaxAABB() + Vector3(CPlayerInfo::GetInstance()->GetPos().x, -5.f, CPlayerInfo::GetInstance()->GetPos().z);

		if ((playerMin.x < itemMax.x && playerMax.x > itemMin.x) &&
			(playerMin.y < itemMax.y && playerMax.y > itemMin.y) &&
			(playerMin.z < itemMax.z && playerMax.z > itemMin.z))
		{
			CWeaponInfo** weaponManager = CPlayerInfo::GetInstance()->getWeaponManager();
			CPlayerInfo* player = CPlayerInfo::GetInstance();

			if (item->GetItem() == EntityBase::HEALTH)
			{
				CSoundEngine::GetInstance()->PlayASound("HEAL");
				if (player->getAttribute(CAttributes::TYPE_HEALTH) + 20 > 100)
					player->setHealthTo(100);
				else
					player->setHealthTo(player->getAttribute(CAttributes::TYPE_HEALTH) + 20);

				item->SetIsDone(true);
			}
			if (item->GetItem() == EntityBase::BULLET)
			{
				/*Player weapon manager*/
				weaponManager[player->GetWeapon()]->SetTotalRound(weaponManager[player->GetWeapon()]->getDefaultMaxTotalRounds());
				CSoundEngine::GetInstance()->PlayASound("RELOAD");
				item->SetIsDone(true);
			}
		}
	}

	//for (auto it = fixedList.begin(); it != fixedList.end(); ++it)
	//{
	//	CFurniture* crate = (CFurniture*)*it;

	//	if (CheckRayAABB(CPlayerInfo::GetInstance()->GetTarget().Normalized(), CPlayerInfo::GetInstance()->GetPos(), crate))
	//	{
	//		hit = true;
	//		break;
	//	}
	//}

	for (list<CProjectile*>::iterator it = projectileList.begin(); it != projectileList.end();)
	{
		if ((*it)->IsDone())
		{
			// Delete if done
			delete *it;
			it = projectileList.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}

	for (list<CEnemy3D*>::iterator it = enemyList.begin(); it != enemyList.end();)
	{
		if ((*it)->IsDone())
		{
			// Delete if done
			delete *it;
			it = enemyList.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}

	for (list<EntityBase*>::iterator it = itemList.begin(); it != itemList.end();)
	{
		if ((*it)->IsDone())
		{
			// Delete if done
			delete *it;
			it = itemList.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}

	if (hit)
	{
		displayHit += (float)_dt;
	}

	if (displayHit >= 0.2f)
	{
		displayHit = 0.f;
		hit = false;
	}
}

// Render all entities
void EntityManager::Render()
{
	// Render all entities
	for (list<EntityBase*>::iterator it = entityList.begin(); it != entityList.end(); ++it)
	{
		(*it)->Render();
	}

	for (list<CProjectile*>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
	{
		(*it)->Render();
	}

	for (list<CFurniture*>::iterator it = fixedList.begin(); it != fixedList.end(); ++it)
	{
		(*it)->Render();
	}

	for (list<CEnemy3D*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
	{
		(*it)->Render();
	}

	for (list<EntityBase*>::iterator it = itemList.begin(); it != itemList.end(); ++it)
	{
		(*it)->Render();
	}
}

// Render the UI entities
void EntityManager::RenderUI()
{
	// Render all entities UI
	for (list<EntityBase*>::iterator it = entityList.begin(); it != entityList.end(); ++it)
	{
		(*it)->RenderUI();
	}

	for (list<CProjectile*>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
	{
		(*it)->RenderUI();
	}

	for (list<CFurniture*>::iterator it = fixedList.begin(); it != fixedList.end(); ++it)
	{
		(*it)->RenderUI();
	}

	for (list<CEnemy3D*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
	{
		(*it)->RenderUI();
	}

	for (list<EntityBase*>::iterator it = itemList.begin(); it != itemList.end(); ++it)
	{
		(*it)->RenderUI();
	}
}

// Add an entity to this EntityManager
void EntityManager::AddEntity(EntityBase* _newEntity)
{
	entityList.push_back(_newEntity);
}

void EntityManager::AddProjectile(CProjectile * _newEntity)
{
	projectileList.push_back(_newEntity);
}

void EntityManager::AddFixed(CFurniture * _newEntity)
{
	fixedList.push_back(_newEntity);
}

void EntityManager::AddPlayer(CPlayerInfo * _newEntity)
{
	playerList.push_back(_newEntity);
}

void EntityManager::AddEnemy(CEnemy3D * _newEntity)
{
	enemyList.push_back(_newEntity);
}

void EntityManager::AddItem(EntityBase * _newEntity)
{
	itemList.push_back(_newEntity);
}

// Remove an entity from this EntityManager
bool EntityManager::RemoveEntity(EntityBase* _existingEntity)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

	// Delete the entity if found
	if (findIter != entityList.end())
	{
		delete *findIter;
		findIter = entityList.erase(findIter);
		return true;	
	}
	// Return false if not found
	return false;
}

bool EntityManager::RemoveEnemy(CEnemy3D * _existingEntity)
{
	// Find the entity's iterator
	std::list<CEnemy3D*>::iterator findIter = std::find(enemyList.begin(), enemyList.end(), _existingEntity);

	// Delete the entity if found
	if (findIter != enemyList.end())
	{
		delete *findIter;
		findIter = enemyList.erase(findIter);
		return true;
	}
	// Return false if not found
	return false;
}


bool EntityManager::getSpawnPosition(Vector3 _minAABB, Vector3 _maxAABB, Vector3 _position)
{
	return CheckAABBCollision(_minAABB, _maxAABB, _position);
}

void EntityManager::setHit(bool _hit)
{
	hit = _hit;
}

bool EntityManager::getHit(void)
{
	return hit;
}

// Constructor
EntityManager::EntityManager() :
	hit(false)
{
}

// Destructor
EntityManager::~EntityManager()
{
	while (entityList.size() > 0)
	{
		EntityBase* object = entityList.back();
		delete object;
		object = nullptr;
		entityList.pop_back();
	}

	while (projectileList.size() > 0)
	{
		CProjectile* object = projectileList.back();
		delete object;
		object = nullptr;
		projectileList.pop_back();
	}

	while (fixedList.size() > 0)
	{
		CFurniture* object = fixedList.back();
		delete object;
		object = nullptr;
		fixedList.pop_back();
	}

	while (playerList.size() > 0)
	{
		CPlayerInfo* object = playerList.back();
		delete object;
		object = nullptr;
		playerList.pop_back();
	}
}

// Check for overlap
bool EntityManager::CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB)
{	
	return false;
}

// Check if this entity's bounding sphere collided with that entity's bounding sphere 
bool EntityManager::CheckSphereCollision(CProjectile *ThisEntity, CFurniture *ThatEntity)
{
	float one = ThisEntity->GetScale().x;
	float two = ThatEntity->GetScale().x;

	Vector3 relativePosition = (ThatEntity->GetPosition().x - ThisEntity->GetPosition().x, ThatEntity->GetPosition().y - ThisEntity->GetPosition().y, ThatEntity->GetPosition().z - ThisEntity->GetPosition().z);
	float magnitude = relativePosition.LengthSquared();
	float totalDisance = ThisEntity->GetScale().x + ThatEntity->GetScale().x;

	//cout << "Magnitude: " << magnitude << endl;
	//cout << "Total Distance: " << totalDisance << endl;

	if (magnitude > totalDisance)
		return false;
	else
		return true;
}

// Check if this entity collided with another entity, but both must have collider
bool EntityManager::CheckAABBCollision(Vector3 _minAABB, Vector3 _maxAABB, Vector3 _position)
{
	Vector3 playerMin = _minAABB + _position;
	Vector3 playerMax = _maxAABB + _position;

	for (list<CFurniture*>::iterator thatObj = fixedList.begin(); thatObj != fixedList.end(); ++thatObj)
	{
		CFurniture* furniture = (CFurniture*)*thatObj;
		Vector3 furnitureMin = furniture->GetMinAABB() + Vector3(furniture->GetPosition().x, -5.f, furniture->GetPosition().z);
		Vector3 furnitureMax = furniture->GetMaxAABB() + Vector3(furniture->GetPosition().x, -5.f, furniture->GetPosition().z);

		//cout << playerMin << " and " << playerMax << endl;
		//cout << furnitureMin << " and " << furnitureMax << endl;

		if ((playerMin.x < furnitureMax.x && playerMax.x > furnitureMin.x) &&
			(playerMin.y < furnitureMax.y && playerMax.y > furnitureMin.y) &&
			(playerMin.z < furnitureMax.z && playerMax.z > furnitureMin.z))
			return true;
		else
			continue;
	}

	for (list<CEnemy3D*>::iterator thatObj = enemyList.begin(); thatObj != enemyList.end(); ++thatObj)
	{
		CEnemy3D* enemy = (CEnemy3D*)*thatObj;
		Vector3 enemyMin = enemy->GetMinAABB() + Vector3(enemy->GetPosition().x, -5.f, enemy->GetPosition().z);
		Vector3 enemyMax = enemy->GetMaxAABB() + Vector3(enemy->GetPosition().x, -5.f, enemy->GetPosition().z);

		//cout << playerMin << " and " << playerMax << endl;
		//cout << enemyMin << " and " << enemyMax << endl;

		if ((playerMin.x < enemyMax.x && playerMax.x > enemyMin.x) &&
			(playerMin.y < enemyMax.y && playerMax.y > enemyMin.y) &&
			(playerMin.z < enemyMax.z && playerMax.z > enemyMin.z))
			return true;
		else
			continue;
	}

	//for (list<EntityBase*>::iterator thatObj = itemList.begin(); thatObj != itemList.end(); ++thatObj)
	//{
	//	EntityBase* item = (EntityBase*)*thatObj;
	//	Vector3 itemMin = item->GetMinAABB() + Vector3(item->GetPosition().x, -5.f, item->GetPosition().z);
	//	Vector3 itemMax = item->GetMaxAABB() + Vector3(item->GetPosition().x, -5.f, item->GetPosition().z);


	//	if ((playerMin.x < itemMax.x && playerMax.x > itemMin.x) &&
	//		(playerMin.y < itemMax.y && playerMax.y > itemMin.y) &&
	//		(playerMin.z < itemMax.z && playerMax.z > itemMin.z))
	//		return true;
	//	else
	//		continue;
	//}

	//cout << "Player Position: " << ThisEntity->GetPos() << endl;
	//cout << "Furniture Position: " << ThatEntity->GetPosition() << endl;
	//cout << "Player Min Position: " << playerMin << endl;
	//cout << "Player Max Position: " << playerMax << endl;
	return false;
}

bool EntityManager::CheckRayAABB(Vector3 _direction, Vector3 _ray, CFurniture * _furniture)
{
	/*float tmin = (_furniture->GetMinAABB().x - r.orig.x) / r.dir.x;
	float tmax = (max.x - r.orig.x) / r.dir.x;

	if (tmin > tmax) swap(tmin, tmax);

	float tymin = (min.y - r.orig.y) / r.dir.y;
	float tymax = (max.y - r.orig.y) / r.dir.y;

	if (tymin > tymax) swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (min.z - r.orig.z) / r.dir.z;
	float tzmax = (max.z - r.orig.z) / r.dir.z;

	if (tzmin > tzmax) swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;

	return true;*/
	// We've taken a shortcut here to avoid a second square root.
	// Note numerical errors can make the normal have length slightly different from 1.
	// If you need higher precision, you may need to perform a conventional normalization.

	//return (intersection, normal);
	///*Attempted but failed.*/
	//Vector3 minAABB(_furniture->GetMinAABB());
	//Vector3 maxAABB(_furniture->GetMaxAABB());
	//Vector3 furniturePos(_furniture->GetPosition());

	////double tmin = -INFINITY, tmax = INFINITY;

	////if (_projectile.Normalize().x != 0.0) {
	////	double tx1 = (minAABB.x - _projectile.x) / _projectile.Normalize().x;
	////	double tx2 = (maxAABB.x - _projectile.x) / _projectile.Normalize().x;

	////	tmin = max(tmin, min(tx1, tx2));
	////	tmax = min(tmax, max(tx1, tx2));
	////}

	////if (_projectile.Normalize().y != 0.0) {
	////	double ty1 = (minAABB.y - _projectile.y) /_projectile.Normalize().y;
	////	double ty2 = (maxAABB.y - _projectile.y) /_projectile.Normalize().y;

	////	tmin = max(tmin, min(ty1, ty2));
	////	tmax = min(tmax, max(ty1, ty2));
	////}

	////return tmax >= tmin;

	/////*Ray Tracing*/
	////float t[10];
	////t[1] = (minAABB.x - _player.x) / _projectile.x;
	////t[2] = (maxAABB.x - _player.x) / _projectile.x;
	////t[3] = (minAABB.y - _player.y) / _projectile.y;
	////t[4] = (maxAABB.y - _player.y) / _projectile.y;
	////t[5] = (minAABB.z - _player.z) / _projectile.z;
	////t[6] = (maxAABB.z - _player.z) / _projectile.z;
	////t[7] = fmax(fmax(fmin(t[1], t[2]), fmin(t[3], t[4])), fmin(t[5], t[6]));
	////t[8] = fmin(fmin(fmax(t[1], t[2]), fmax(t[3], t[4])), fmax(t[5], t[6]));
	////t[9] = (t[8] < 0 || t[7] > t[8]) ? false : t[7];

	////return t[9];


	////Let s be the start point of the ray, and d a unit vector in the direction of the ray.
	////Let c be the center point of the sphere, and r its radius.

	//// Calculate ray start's offset from the sphere center
	//Vector3 p = _ray - _furniture->GetScale().x;

	//Vector3 direction = _furniture->GetPosition() - _ray;

	//float rSquared = 1000.f;
	////cout << "Furniture Scale: " << _furniture->GetScale().LengthSquared() << endl;

	//float magnitude = sqrt(_direction.LengthSquared());
	//float p_d = p.Dot(_direction.Normalize());

	////cout << "p_d: " << p_d;
	////cout << " > 0" << endl;

	////cout << "p.Dot(p): " << p.Dot(p);
	////cout << " < " << rSquared << endl;
	//// The sphere is behind or surrounding the start point.
	//if (p_d > 0 || p.Dot(p) < rSquared)
	//{
	//	//cout << "NO COLLISION" << endl;
	//	return false;
	//}

	//// Flatten p into the plane passing through c perpendicular to the ray.
	//// This gives the closest approach of the ray to the center.
	//Vector3 a = p - p_d * _direction;

	//float aSquared = a.Dot(a);

	////cout << "aSquared: " << aSquared;
	////cout << " > rSquared: " << rSquared << endl;
	//// Closest approach is outside the sphere.
	//if (aSquared > rSquared)
	//{
	//	//cout << "NO COLLISION 2" << endl;
	//	return false;
	//}

	//// Calculate distance from plane where ray enters/exits the sphere.    
	//float h = sqrt(rSquared - aSquared);

	//// Calculate intersection point relative to sphere center.
	//Vector3 i = a - h * direction;

	//Vector3 intersection = _furniture->GetPosition() + i;
	//Vector3 normal = i / _furniture->GetScale().x;
	//// We've taken a shortcut here to avoid a second square root.
	//// Note numerical errors can make the normal have length slightly different from 1.
	//// If you need higher precision, you may need to perform a conventional normalization.
	//cout << "Intersection: " << intersection << endl;
	////return (intersection, normal);
	//return true;

	////Vector3 dirfrac(0.f, 0.f, 0.f);
	//////Vector3 dirfrac(_projectile->GetDirection().Normalize());

	////// r.dir is unit direction vector of ray

	////Vector3 projectileDir(_projectile.Normalize());
	////dirfrac.x = 1.0f / projectileDir.x;
	////dirfrac.y = 1.0f / projectileDir.y;
	////dirfrac.z = 1.0f / projectileDir.z;


	////// lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
	////// r.org is origin of ray
	////float t1 = (minAABB.x - _player.x)*dirfrac.x;
	////float t2 = (maxAABB.x - _player.x)*dirfrac.x;
	////float t3 = (minAABB.y - _player.y)*dirfrac.y;
	////float t4 = (maxAABB.y - _player.y)*dirfrac.y;
	////float t5 = (minAABB.z - _player.z)*dirfrac.z;
	////float t6 = (maxAABB.z - _player.z)*dirfrac.z;

	////float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
	////float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

	////// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
	////if (tmax < 0)
	////{
	////	float t = tmax;
	////	cout << "NO COLLIDED!" << endl;
	////	return false;
	////}

	////// if tmin > tmax, ray doesn't intersect AABB
	////if (tmin > tmax)
	////{
	////	float t = tmax;
	////	cout << "NO COLLIDED!" << endl;
	////	return false;
	////}

	////float t = tmin;
	////cout << "COLLIDED!" << endl;
	////return true;
return false;
}

list<CFurniture*>& EntityManager::returnFixed(void)
{
	return fixedList;
}

list<CEnemy3D*>& EntityManager::returnEnemy(void)
{
	return enemyList;
}

int EntityManager::enemyCount(void)
{
	int tempCount = 0;
	for (list<CEnemy3D*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
	{
		CEnemy3D* enemy = (CEnemy3D*)*it;

		if (enemy->getState() == CEnemy3D::IDLE || enemy->getState() == CEnemy3D::ALERT)
			++tempCount;
	}
	return tempCount;
}

// Check if any Collider is colliding with another Collider
bool EntityManager::CheckForCollision(CCollider* thisEntity, CCollider* thatEntity)
{
	return false;
}

bool EntityManager::CheckProjectileCollision(CProjectile * thisEntity, CFurniture * thatEntity)
{
	Vector3 projectileMin = thisEntity->GetMinAABB() + thisEntity->GetPosition();
	Vector3 projectileMax = thisEntity->GetMaxAABB() + thisEntity->GetPosition();

	Vector3 furnitureMin = thatEntity->GetMinAABB() + Vector3(thatEntity->GetPosition().x, -5.f, thatEntity->GetPosition().z);
	Vector3 furnitureMax = thatEntity->GetMaxAABB() + Vector3(thatEntity->GetPosition().x, -5.f, thatEntity->GetPosition().z);

	if ((projectileMin.x < furnitureMax.x && projectileMax.x > furnitureMin.x) &&
		(projectileMin.y < furnitureMax.y && projectileMax.y > furnitureMin.y) &&
		(projectileMin.z < furnitureMax.z && projectileMax.z > furnitureMin.z))
		return true;
	else
		return false;
}

bool EntityManager::CheckProjectileToPlayerCollision(CProjectile * thisEntity, CPlayerInfo * thatEntity)
{
	Vector3 projectileMin = thisEntity->GetMinAABB() + thisEntity->GetPosition();
	Vector3 projectileMax = thisEntity->GetMaxAABB() + thisEntity->GetPosition();

	Vector3 playerMin = thatEntity->GetMinAABB() + Vector3(thatEntity->GetPos().x, -5.f, thatEntity->GetPos().z);
	Vector3 playerMax = thatEntity->GetMaxAABB() + Vector3(thatEntity->GetPos().x, -5.f, thatEntity->GetPos().z);

	if ((projectileMin.x < playerMax.x && projectileMax.x > playerMin.x) &&
		(projectileMin.y < playerMax.y && projectileMax.y > playerMin.y) &&
		(projectileMin.z < playerMax.z && projectileMax.z > playerMin.z))
		return true;
	else
		return false;
}

bool EntityManager::CheckProjectileToEnemyCollision(CProjectile * thisEntity, CEnemy3D * thatEntity)
{
	Vector3 projectileMin = thisEntity->GetMinAABB() + thisEntity->GetPosition();
	Vector3 projectileMax = thisEntity->GetMaxAABB() + thisEntity->GetPosition();

	Vector3 enemyMin = thatEntity->GetMinAABB() + Vector3(thatEntity->GetPos().x, -5.f, thatEntity->GetPos().z);
	Vector3 enemyMax = thatEntity->GetMaxAABB() + Vector3(thatEntity->GetPos().x, -5.f, thatEntity->GetPos().z);

	if ((projectileMin.x < enemyMax.x && projectileMax.x > enemyMin.x) &&
		(projectileMin.y < enemyMax.y && projectileMax.y > enemyMin.y) &&
		(projectileMin.z < enemyMax.z && projectileMax.z > enemyMin.z))
		return true;
	else
		return false;
}
