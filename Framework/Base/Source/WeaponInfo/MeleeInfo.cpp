#include "MeleeInfo.h"
#include "../Projectile/Projectile.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "../SoundEngine.h"
#include <iostream>
using namespace std;

CMeleeInfo::CMeleeInfo()
	: /*magRounds(1)
	, maxMagRounds(1)
	, totalRounds(8)
	, maxTotalRounds(8)*/
	timeBetweenShots(0.5)
	, elapsedTime(0.0)
	, bFire(true)/*
	, defaultMaxTotalRounds(0)*/
{
}


CMeleeInfo::~CMeleeInfo()
{
}
//
//// Set the number of ammunition in the magazine for this player
//void CMeleeInfo::SetMagRound(const int magRounds)
//{
//	this->magRounds = magRounds;
//}
//
//// Set the maximum number of ammunition in the magazine for this weapon
//void CMeleeInfo::SetMaxMagRound(const int magRounds)
//{
//	this->magRounds = magRounds;
//}
//
//// The current total number of rounds currently carried by this player
//void CMeleeInfo::SetTotalRound(const int totalRounds)
//{
//	this->totalRounds = totalRounds;
//}
//
//// The max total number of rounds currently carried by this player
//void CMeleeInfo::SetMaxTotalRound(const int maxTotalRounds)
//{
//	this->maxTotalRounds = maxTotalRounds;
//}

//
//// Get the number of ammunition in the magazine for this player
//int CMeleeInfo::GetMagRound(void) const
//{
//	return magRounds;
//}
//
//// Get the maximum number of ammunition in the magazine for this weapon
//int CMeleeInfo::GetMaxMagRound(void) const
//{
//	return maxMagRounds;
//}
//
//// Get the current total number of rounds currently carried by this player
//int CMeleeInfo::GetTotalRound(void) const
//{
//	return totalRounds;
//}
//
//// Get the max total number of rounds currently carried by this player
//int CMeleeInfo::GetMaxTotalRound(void) const
//{
//	return maxTotalRounds;
//}

// Set the time between shots
void CMeleeInfo::SetTimeBetweenShots(const double timeBetweenShots)
{
	this->timeBetweenShots = timeBetweenShots;
}

// Set the firing rate in rounds per min
void CMeleeInfo::SetFiringRate(const int firingRate)
{
	timeBetweenShots = 60.0 / (double)firingRate;	// 60 seconds divided by firing rate
}

// Set the firing flag
void CMeleeInfo::SetCanFire(const bool bFire)
{
	this->bFire = bFire;
}

// Get the time between shots
double CMeleeInfo::GetTimeBetweenShots(void) const
{
	return timeBetweenShots;
}

// Get the firing rate
int CMeleeInfo::GetFiringRate(void) const
{
	return (int)(60.0 / timeBetweenShots);	// 60 seconds divided by timeBetweenShots
}

// Get the firing flag
bool CMeleeInfo::GetCanFire(void) const
{
	return bFire;
}

// Initialise this instance to default values
void CMeleeInfo::Init(void)
{
	//// The number of ammunition in a magazine for this weapon
	//magRounds = 1;
	//// The maximum number of ammunition for this magazine for this weapon
	//maxMagRounds = 1;
	//// The current total number of rounds currently carried by this player
	//totalRounds = 8;
	//// The max total number of rounds currently carried by this player
	//maxTotalRounds = 8;

	// The time between shots
	timeBetweenShots = 0.f;
	// The elapsed time (between shots)
	elapsedTime = timeBetweenShots;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}

// Update the elapsed time
void CMeleeInfo::Update(const double dt)
{
	elapsedTime += dt;
	if (elapsedTime > timeBetweenShots)
	{
		bFire = true;
		elapsedTime = 0.0;
	}
}

// Discharge this weapon
void CMeleeInfo::Slash(Vector3 position, Vector3 target, CPlayerInfo* _source)
{
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		/*if (magRounds > 0)
		{
		}*/
			Vector3 distanceBetween(target - position);
			distanceBetween *= 3.f;
			distanceBetween += position;
			CProjectile* aProjectile = Create::Projectile("sphere", 
				distanceBetween,
															(target - position).Normalized(), /*Direction*/
															0.1f, /*Lifespan of Bullet*/
															80.0f, /*Speed*/
															_source);
			aProjectile->SetCollider(true);
			aProjectile->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
			bFire = false;
			//magRounds--;
	}
}

//// Reload this weapon
//void CMeleeInfo::Reload(void)
//{
//	if (magRounds < maxMagRounds)
//	{
//		if (maxMagRounds - magRounds <= totalRounds)
//		{
//			CSoundEngine::GetInstance()->PlayASound("RELOAD");
//			/*Changed reloading system to hold and reload.*/
//			totalRounds--;
//			magRounds++;
//
//			//totalRounds -= maxMagRounds - magRounds;
//			//magRounds = maxMagRounds;
//		}
//		else
//		{
//			/*Add everything into magRounds.*/
//			magRounds += totalRounds;
//			totalRounds = 0;
//		}
//	}
//}
//
//// Add rounds
//void CMeleeInfo::AddRounds(const int newRounds)
//{
//	if (totalRounds + newRounds > maxTotalRounds)
//		totalRounds = maxTotalRounds;
//	else
//		totalRounds += newRounds;
//}
//
//// Print Self
//void CMeleeInfo::PrintSelf(void)
//{
//	cout << "CMeleeInfo::PrintSelf()" << endl;
//	cout << "========================" << endl;
//	cout << "magRounds\t\t:\t" << magRounds << endl;
//	cout << "maxMagRounds\t\t:\t" << maxMagRounds << endl;
//	cout << "totalRounds\t\t:\t" << totalRounds << endl;
//	cout << "maxTotalRounds\t\t:\t" << maxTotalRounds << endl;
//	cout << "timeBetweenShots\t:\t" << timeBetweenShots << endl;
//	cout << "elapsedTime\t\t:\t" << elapsedTime << endl;
//	cout << "bFire\t\t:\t" << bFire << endl;
//}
//
//int CMeleeInfo::getDefaultMaxTotalRounds(void)
//{
//	return defaultMaxTotalRounds;
//}
