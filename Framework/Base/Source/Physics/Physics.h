/*Physics was done by */
#ifndef PHYSICS_H
#define PHYSICS_H

#include "Vector3.h"

struct Physics
{
	Vector3 gravity;
	Vector3 acceleration;
	Vector3 direction;
	Vector3 initialVelocity;
	Vector3 finalVelocity;
	Vector3 force;
	float mass;
	float speed;
	float time;
	float displacement;
};

#endif