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

			float x = (float(rand() % 20) * 1.25) - 20;
			float y = (float(rand() % 75) * 1.25);
			float z = (float(rand() % 20) * 1.25) - 30;

			createParticle(posSystem, Vector3(x,y,z));
			timeNextParticle -= timeCreateParticle;
		}

		timeNextParticle = 0;
	}
}


