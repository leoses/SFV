#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(float timeBetweenParticles)
{
	timeCreateParticle = timeBetweenParticles;

	//inicializar generador randoms
	srand(time(NULL));
}


void ParticleSystem::update(float t, Vector3 pos)
{
	addParticles(t, pos);
}


void ParticleSystem::addParticles(float t, Vector3 pos)
{
	timeNextParticle += t;

	if (timeNextParticle > timeCreateParticle) {
		while (timeNextParticle > timeCreateParticle) {

			float x = -20 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (20 - (-20))));
			float y = (float(rand() % 30) * 1.25);
			float z = -20 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0 - (-20))));

			createParticle(pos, Vector3(x,y,z));
			timeNextParticle -= timeCreateParticle;
		}

		timeNextParticle = 0;
	}
}


