#include "Knife.h"


CKnife::CKnife()
{
}


CKnife::~CKnife()
{
}

// Initialise this instance to default values
void CKnife::Init(void)
{
	// Call the parent's Init method
	CMeleeInfo::Init();

	timeBetweenShots = 0.428;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
	// Range (life time == melee range)
	meleeRange = 0.03f;
}
