#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H
#include <vector>
#include "Vector3.h"
#include "SingletonTemplate.h"

using std::vector;

enum particleObject_type
{
	P_Water = 0,
	P_Total
};
class ParticleManager : public Singleton<ParticleManager>
{
	//friend Singleton<ParticleManager>;
public:
	ParticleManager(particleObject_type = P_Water);
	~ParticleManager();

	void updateParticle(double dt);
	ParticleManager* getParticle(void);
	void renderParticle();
	void renderAllParticle();
	int getMaximumParticles(void) const;
	vector<ParticleManager*>&getParticleList(void);
	void pushParticle(particleObject_type type);
	void deleteParticle();
private:
	//Different type of particle object
	particleObject_type type;
	//Particle's position
	Vector3 particle_Position;
	//Particle's velocity/speed
	Vector3 particle_Velocity;
	//Particle's size
	Vector3 particle_Scale;
	//Particle's duration before disappearing
	float particle_LifeTime;
	//Enable/Disable particle
	bool particle_IsActive;

	std::vector<ParticleManager*> particleList;
	unsigned MAX_PARTICLES = 1000;
	int m_particleCount = 0;
	Vector3 m_gravity = Vector3(0.0f, -9.8f, 0.0f);
};

#endif // ! PARTICLEMANAGER_H
