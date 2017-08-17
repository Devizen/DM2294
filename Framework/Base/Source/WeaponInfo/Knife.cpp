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

	//// The number of ammunition in a magazine for this weapon
	//magRounds = 8;
	//// The maximum number of ammunition for this magazine for this weapon
	//maxMagRounds = 8;
	//// The current total number of rounds currently carried by this player
	//totalRounds = 40;
	//// The max total number of rounds currently carried by this player
	//maxTotalRounds = 40;
	////Set max total round for bullet pick-up.
	//defaultMaxTotalRounds = maxTotalRounds;
	// The time between shots
	//timeBetweenShots = 0.3333;
	/*Knife OGG Time Length*/
	timeBetweenShots = 0.428;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}
