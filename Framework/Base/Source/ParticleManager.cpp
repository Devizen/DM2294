#include "ParticleManager.h"

ParticleManager::ParticleManager(particleObject_type typeV)
	:type(typeV),
	particlePosition(0, 0, 0),
	particleVelocity(0,0,0),
	particleScale(1,1,1),
	particleIsActive(false)
{
}

ParticleManager::~ParticleManager()
{
}

void ParticleManager::updateParticle(double dt)
{
}

//void ParticleManager::getParticle(void)
//{
//}

void ParticleManager::renderParticle(particleObject_type* particle)
{
}


