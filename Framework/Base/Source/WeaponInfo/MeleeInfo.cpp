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
	, numOfStabs(0)
	, meleeRange(0.1f)
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
	// Range (life time == melee range)
	meleeRange = 0.03f;
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
void CMeleeInfo::StabEnemy(Vector3 position, Vector3 target, CPlayerInfo* _source)
{
	Vector3 distanceBetween(target - position);
	distanceBetween *= 3.f;
	distanceBetween += position;
	CProjectile* aProjectile = Create::Projectile("sphere", 
													distanceBetween,
													(target - position).Normalized(), 
													meleeRange,
													0.f,
													_source);
	aProjectile->SetCollider(true);
	aProjectile->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	aProjectile->bulletOriginated = CProjectile::FROM_PLAYER;
	numOfStabs++;
}

void CMeleeInfo::SlashEnemy(Vector3 position, Vector3 target, CPlayerInfo* _source)
{
	Vector3 topOfPlayer(position.x, position.y * 5.f, position.z);
	Vector3 slashDown(target.x - position.x, (position.y * 4.f), target.z - position.z);
	float increaseSlashDisplacement = 10.0f;
	float translateDown = 0.f;

	for (size_t i = 0; i < 100; ++i)
	{
		CProjectile* aTriggerBox = Create::Projectile("cubeBox",
			Vector3(topOfPlayer.x, topOfPlayer.y - translateDown, topOfPlayer.z) + (slashDown.Normalized() * increaseSlashDisplacement),
			slashDown.Normalized(),
			0.01f,
			0.0f,
			_source);
		aTriggerBox->SetCollider(true);
		aTriggerBox->SetAABB(Vector3(0.5, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
		aTriggerBox->bulletOriginated = CProjectile::FROM_PLAYER;
		if (i < 15)
			increaseSlashDisplacement += 1.0f;
		else
			slashDown.Normalized() -= 1.0f;

		translateDown += 0.1f;
	}
}

