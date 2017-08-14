#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H
#include <vector>
#include "Vector3.h"
enum particleObject_type
{
	P_Fire,
	P_Total
};
class ParticleManager
{
public:
	ParticleManager(particleObject_type = P_Fire);
	~ParticleManager();

	void updateParticle(double dt);
	//void getParticle(void);
	void renderParticle(particleObject_type* particle);
	

private:
	//Different type of particle object
	particleObject_type type;
	//Particle's position
	Vector3 particlePosition;
	//Particle's velocity/speed
	Vector3 particleVelocity;
	//Particle's size
	Vector3 particleScale;
	//Particle's duration before disappearing
	float particleLifeTime;
	//Enable/Disable particle
	bool particleIsActive;

	std::vector<particleObject_type> list;
	int i_particleCount;

};

#endif // ! PARTICLEMANAGER_H
