#include "Collision.h"

bool Check::AABB(Vector3 thisPosition, Vector3 thatPosition)
{
	Vector3 minimumBoundary = Vector3(-10.f + thatPosition.x, -10.f + thatPosition.y, -10.f + thatPosition.z);
	Vector3 maximumBoundary = Vector3(10.f + thatPosition.x, 10.f + thatPosition.y, 10.f + thatPosition.z);

	if (thisPosition > minimumBoundary && thisPosition < maximumBoundary)
		return true;

	return false;
}
