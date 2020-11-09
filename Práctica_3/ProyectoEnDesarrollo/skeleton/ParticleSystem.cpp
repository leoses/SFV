#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(Vector3 posSystem_, float timeBetweenParticles)
{
	posSystem = posSystem_;
	timeCreateParticle = timeBetweenParticles;


	//inicializar generador randoms
	srand(time(NULL));
}


void ParticleSystem::update(float t)
{
	addParticles(t);
}


void ParticleSystem::addParticles(float t)
{
	timeNextParticle += t;

	if (timeNextParticle > timeCreateParticle) {
		while (timeNextParticle > timeCreateParticle) {
			CreateParticle();
			timeNextParticle -= timeCreateParticle;
		}
	}
}

void ParticleSystem::CreateParticle()
{
	Particle* p = findUnusedParticle();
	if (p != nullptr) {
		float x = (float(rand() % 100) * 1.25) - 80;
		float y = (float(rand() % 100) * 1.25);
		float z = (float(rand() % 100) * 1.25) - 80;

		p->setPosition(posSystem);
		p->setVelocity(Vector3(x, y, z));
		p->setAcceleration(Vector3(0, -10.0, 0));
		p->activateParticle();
	}

}

