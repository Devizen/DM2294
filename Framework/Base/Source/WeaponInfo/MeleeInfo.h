#pragma once

#include "Vector3.h"
#include "SingletonTemplate.h"

class CPlayerInfo;

class CMeleeInfo : public Singleton<CMeleeInfo>
{
public:
	CMeleeInfo();
	virtual ~CMeleeInfo();
protected:
	// The time between shots in milliseconds
	double timeBetweenShots;
	// The elapsed time (between shots) in milliseconds
	double elapsedTime;
	// Boolean flag to indicate if weapon can fire now
	bool bFire;
	// Range (life time == melee range)
	float meleeRange;
public:
	// Set the time between shots
	virtual void SetTimeBetweenShots(const double timeBetweenShots);
	// Set the firing rate in rounds per min
	virtual void SetFiringRate(const int firingRate);
	// Set the firing flag
	virtual void SetCanFire(const bool bFire);

	// Get the time between shots
	virtual double GetTimeBetweenShots(void) const;
	// Get the firing rate
	virtual int GetFiringRate(void) const;
	// Get the firing flag
	virtual bool GetCanFire(void) const;

	// Initialise this instance to default values
	virtual void Init(void);
	// Update the elapsed time
	void Update(const double dt);
	// Discharge this weapon
	void StabEnemy(Vector3 position, Vector3 target, CPlayerInfo* _source = NULL);

	void SlashEnemy(Vector3 position, Vector3 target, CPlayerInfo* _source = NULL);
};
