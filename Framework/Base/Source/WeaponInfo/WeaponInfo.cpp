#include "WeaponInfo.h"
#include "../Projectile/Projectile.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "../SoundEngine.h"
#include <iostream>
using namespace std;

CWeaponInfo::CWeaponInfo()
	: magRounds(1)
	, maxMagRounds(1)
	, totalRounds(8)
	, maxTotalRounds(8)
	, timeBetweenShots(0.5)
	, elapsedTime(0.0)
	, bFire(true)
	, defaultMaxTotalRounds(0)
	, fired(false)
{
}


CWeaponInfo::~CWeaponInfo()
{
}

// Set the number of ammunition in the magazine for this player
void CWeaponInfo::SetMagRound(const int magRounds)
{
	this->magRounds = magRounds;
}

// Set the maximum number of ammunition in the magazine for this weapon
void CWeaponInfo::SetMaxMagRound(const int magRounds)
{
	this->magRounds = magRounds;
}

// The current total number of rounds currently carried by this player
void CWeaponInfo::SetTotalRound(const int totalRounds)
{
	this->totalRounds = totalRounds;
}

// The max total number of rounds currently carried by this player
void CWeaponInfo::SetMaxTotalRound(const int maxTotalRounds)
{
	this->maxTotalRounds = maxTotalRounds;
}


// Get the number of ammunition in the magazine for this player
int CWeaponInfo::GetMagRound(void) const
{
	return magRounds;
}

// Get the maximum number of ammunition in the magazine for this weapon
int CWeaponInfo::GetMaxMagRound(void) const
{
	return maxMagRounds;
}

// Get the current total number of rounds currently carried by this player
int CWeaponInfo::GetTotalRound(void) const
{
	return totalRounds;
}

// Get the max total number of rounds currently carried by this player
int CWeaponInfo::GetMaxTotalRound(void) const
{
	return maxTotalRounds;
}

// Set the time between shots
void CWeaponInfo::SetTimeBetweenShots(const double timeBetweenShots)
{
	this->timeBetweenShots = timeBetweenShots;
}

// Set the firing rate in rounds per min
void CWeaponInfo::SetFiringRate(const int firingRate)
{
	timeBetweenShots = 60.0 / (double)firingRate;	// 60 seconds divided by firing rate
}

// Set the firing flag
void CWeaponInfo::SetCanFire(const bool bFire)
{
	this->bFire = bFire;
}

// Get the time between shots
double CWeaponInfo::GetTimeBetweenShots(void) const
{
	return timeBetweenShots;
}

// Get the firing rate
int CWeaponInfo::GetFiringRate(void) const
{
	return (int)(60.0 / timeBetweenShots);	// 60 seconds divided by timeBetweenShots
}

// Get the firing flag
bool CWeaponInfo::GetCanFire(void) const
{
	return bFire;
}

// Initialise this instance to default values
void CWeaponInfo::Init(void)
{
	// The number of ammunition in a magazine for this weapon
	magRounds = 1;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 1;
	// The current total number of rounds currently carried by this player
	totalRounds = 8;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 8;

	// The time between shots
	timeBetweenShots = 0.f;
	// The elapsed time (between shots)
	elapsedTime = timeBetweenShots;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}

// Update the elapsed time
void CWeaponInfo::Update(const double dt)
{
	portableDT = dt;
	if (elapsedTime != timeBetweenShots)
		elapsedTime += dt;

	if (elapsedTime > timeBetweenShots)
	{
		bFire = true;
		fired = false;
		elapsedTime = timeBetweenShots;
	}

	if (!bFire)
	{
		if (elapsedTime > 0.1f)
			fired = false;
		else
			fired = true;
	}
}

// Discharge this weapon
void CWeaponInfo::Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source)
{
	if (elapsedTime == timeBetweenShots)
	{
		Vector3 distanceBetween(0.f, 0.f, 0.f);
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			if (!_source->getLockedOn())
			{
				distanceBetween = target - _source->GetPos();
				distanceBetween *= 5.f;
				distanceBetween += _source->GetPos();
			}
			else
			{
				Vector3 shiftForward((_source->getEnemyPositionToLockOn()->GetPos() - _source->GetPos()).Normalized());
				shiftForward *= 5.f;

				distanceBetween += _source->GetPos() + shiftForward;
			}
			CProjectile* aProjectile = Create::Projectile("sphere", 
															distanceBetween,
															(target - position).Normalized(), /*Direction*/
															2.0f, /*Lifespan of Bullet*/
															800.0f, /*Speed*/
															_source);
			aProjectile->SetCollider(true);
			aProjectile->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
			aProjectile->bulletOriginated = CProjectile::FROM_PLAYER;
			bFire = false;
			magRounds--;
			elapsedTime = 0.f;
		}
	}
}

// Reload this weapon
void CWeaponInfo::Reload(void)
{
	if (magRounds < maxMagRounds)
	{
		if (maxMagRounds - magRounds <= totalRounds)
		{
			CSoundEngine::GetInstance()->PlayASound("RELOAD");
			/*Changed reloading system to hold and reload.*/
			totalRounds--;
			magRounds++;

			//totalRounds -= maxMagRounds - magRounds;
			//magRounds = maxMagRounds;
		}
		else
		{
			/*Add everything into magRounds.*/
			magRounds += totalRounds;
			totalRounds = 0;
		}
	}
}

// Add rounds
void CWeaponInfo::AddRounds(const int newRounds)
{
	if (totalRounds + newRounds > maxTotalRounds)
		totalRounds = maxTotalRounds;
	else
		totalRounds += newRounds;
}

// Print Self
void CWeaponInfo::PrintSelf(void)
{
	cout << "CWeaponInfo::PrintSelf()" << endl;
	cout << "========================" << endl;
	cout << "magRounds\t\t:\t" << magRounds << endl;
	cout << "maxMagRounds\t\t:\t" << maxMagRounds << endl;
	cout << "totalRounds\t\t:\t" << totalRounds << endl;
	cout << "maxTotalRounds\t\t:\t" << maxTotalRounds << endl;
	cout << "timeBetweenShots\t:\t" << timeBetweenShots << endl;
	cout << "elapsedTime\t\t:\t" << elapsedTime << endl;
	cout << "bFire\t\t:\t" << bFire << endl;
}

int CWeaponInfo::getDefaultMaxTotalRounds(void)
{
	return defaultMaxTotalRounds;
}

double CWeaponInfo::GetElapsedTime(void)
{
	return elapsedTime;
}
