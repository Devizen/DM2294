#include "MeleeInfo.h"
#include "../Projectile/Projectile.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "../SoundEngine.h"
#include <iostream>
using namespace std;

CMeleeInfo::CMeleeInfo()
	:timeBetweenShots(0.5)
	, elapsedTime(0.0)
	, bFire(true)
{
}


CMeleeInfo::~CMeleeInfo()
{
}

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
		//Place holder for melee system (stabbing style)
		Vector3 distanceBetween(target - position);
		distanceBetween *= 3.f;
		distanceBetween += position;
		CProjectile* aProjectile = Create::Projectile("sphere", 
			distanceBetween,
														(target - position).Normalized(), /*Direction*/
														0.012f, /*Lifespan of Bullet*/
														800.0f, /*Speed*/
														_source);
		aProjectile->SetCollider(true);
		aProjectile->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
		bFire = false;
	}
}