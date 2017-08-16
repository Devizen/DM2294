#include "ParticleManager.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include <iostream>

using std::cout;
using std::endl;

ParticleManager::ParticleManager(particleObject_type typeValue)
	:type(typeValue),
	particle_Position(0.0f, 0.0f, 0.0f),
	particle_Velocity(0.0f, 0.0f, 0.0f),
	particle_Scale(1.0f, 1.0f, 1.0f),
	particle_IsActive(false),
	m_gravity(0.0f, -9.8f, 0.0f),
	m_particleCount(0)
{
}

ParticleManager::~ParticleManager()
{
}

void ParticleManager::updateParticle(double dt)
{
	if (m_particleCount < MAX_PARTICLES)
	{
		ParticleManager* particle = getParticle();
		particle->type = particleObject_type::P_Water;
		particle->particle_Scale.Set(5.0f, 5.0f, 5.0f);
		particle->particle_Velocity.Set(0.0f, 0.0f, 0.0f);
		particle->particle_Position.Set(Math::RandFloatMinMax(-350, 350), 1000.f, Math::RandFloatMinMax(-350, 350));
	}
	//Update all particle
	std::vector<ParticleManager*>::iterator it, end;
	end = particleList.end();
	for (it = particleList.begin(); it != end; ++it)
	{
		ParticleManager* particle = (ParticleManager*)*it;
		if (!particle->particle_IsActive)
			continue;
		//Only active particle
		if (particle->type == particleObject_type::P_Water)
		{
			particle->particle_Velocity += m_gravity * (float)dt;
			particle->particle_Position += particle->particle_Velocity;
		}

		if (particle->particle_Position.y <= -10.f)
			particle->particle_IsActive = false;
	}
}

ParticleManager * ParticleManager::getParticle(void)
{

	for (vector<ParticleManager*>::iterator it = particleList.begin(); it != particleList.end(); ++it)
	{
		ParticleManager* particle = (ParticleManager*)*it;
		if (!particle->particle_IsActive)
		{
			particle->particle_IsActive = true;
			m_particleCount++;
			return particle;
		}
	}
	//Allocating the particles 
	for (unsigned i = 0; i < 10; ++i)
	{
		ParticleManager* particle = new ParticleManager(particleObject_type::P_Water);
		particleList.push_back(particle);
	}
	ParticleManager* particle = particleList.back();
	particle->particle_IsActive = true;
	m_particleCount++;
	return particle;
}

void ParticleManager::renderParticle()
{
	switch (type)
	{
	case P_Water:
		Mesh* modelMesh;
		modelMesh = MeshBuilder::GetInstance()->GetMesh("PISTOL");
		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		modelStack.PushMatrix();
		modelStack.Translate(particle_Position.x, particle_Position.y, particle_Position.z);
		modelStack.Scale(particle_Scale.x, particle_Scale.y, particle_Scale.z);
		RenderHelper::RenderMesh(modelMesh);
		modelStack.PopMatrix();
		break;
	}
}

void ParticleManager::renderAllParticle()
{
	std::vector<ParticleManager*>::iterator it, end;
	end = particleList.end();
	for (it = particleList.begin(); it != end; ++it)
	{
		ParticleManager*particle = (ParticleManager*)*it;
		if (!particle->particle_IsActive)
			continue;

		particle->renderParticle();
	}
}

int ParticleManager::getMaximumParticles(void) const
{
	return MAX_PARTICLES;
}

vector<ParticleManager*>& ParticleManager::getParticleList(void)
{
	return particleList;
}

void ParticleManager::pushParticle(particleObject_type type)
{
	ParticleManager* particle = new ParticleManager(particleObject_type::P_Water);
	particle->particle_IsActive = false;
	
	if (type == particleObject_type::P_Water)
		particle->type = particleObject_type::P_Water;
	
	particleList.push_back(particle);
}

//Updates needed in scene
/*Init*/
//ParticleManager* particleManager = ParticleManager::GetInstance();
//vector<ParticleManager*>particleList = particleManager->getParticleList();
//for (int i = 0; i < particleManager->getMaximumParticles(); ++i)
//{
//	particleManager->pushParticle(particleObject_type::P_Water);
//}
//cout << "Particle List Size in Scene: " << particleList.size() << endl;
/*Update*/
//ParticleManager::GetInstance()->updateParticle(dt);
/*Renderworld*/
//ParticleManager::GetInstance()->renderAllParticle();