#pragma once

#include "WeaponInfo.h"

class CAssaultRifle :
	public CWeaponInfo
{
public:
	CAssaultRifle();
	virtual ~CAssaultRifle();

	// Initialise this instance to default values
	void Init(void);
};

