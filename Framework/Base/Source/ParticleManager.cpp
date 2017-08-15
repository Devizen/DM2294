#include "ParticleManager.h"

ParticleManager::ParticleManager(particleObject_type typeValue)
	:type(typeValue),
	particle_Position(0.0f, 0.0f, 0.0f),
	particle_Velocity(0.0f, 0.0f, 0.0f),
	particle_Scale(1.0f, 1.0f, 1.0f),
	particle_IsActive(false),
	m_gravity(0.0f, 0.0f, 0.0f)
{
}

ParticleManager::~ParticleManager()
{
}

void ParticleManager::updateParticle(double dt)
{
}

ParticleManager * ParticleManager::getParticle(void)
{
	std::vector<ParticleManager*>::iterator it, end;
	end = list.end();
	for (it = list.begin(); it != end; it++)
	{
		ParticleManager* particle = *it;
		if (particle->particle_IsActive == false)
		{
			particle->particle_IsActive = true;
			m_particleCount++;
			return particle;
		}
	}
	//Allocating the particles 
	for (int i = 0; i < 10; i++)
	{
		ParticleManager* particle = new ParticleManager(particleObject_type::P_Water);
		list.push_back(particle);
	}
	ParticleManager* particle = list.back();
	particle->particle_IsActive = true;
	m_particleCount++;
	return particle;
}

void ParticleManager::renderParticle(particleObject_type* particle)
{
	switch (type)
	{
	case P_Water:
		break;
	case P_Total:
		break;
	default:
		break;
	}
}