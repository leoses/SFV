#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(Vector3 posSystem_, float timeBetweenParticles)
{
	posSystem = posSystem_;
	timeCreateParticle = timeBetweenParticles;
	

	for (int i = 0; i < MAX_PARTICLES; i++) {
		listParticles.push_back(new Particle(posSystem));
	}

	//inicializar generador randoms
	srand(time(NULL));
}

ParticleSystem::~ParticleSystem()
{
	for (Particle* p : listParticles) {
		delete p;
		p = nullptr;
	}
}

void ParticleSystem::update(float t)
{
	updateParticles(t);
	addParticles(t);
}

void ParticleSystem::updateParticles(float t)
{
	//Actualizacion de posiciones
	for (Particle* p : listParticles) {
		if (!p->isActive())continue;

		//Si la partícula ya ha vivido el tiempo máximo la dejamos de renderizar
		if (p->isDead()) {
			p->desactivateParticle();
		}
		else {
			p->integrate(t);
			p->addTime(t);
		}
	}
}

void ParticleSystem::addParticles(float t)
{
	timeNextParticle += t;

	if (timeNextParticle > timeCreateParticle) {
		bool found = false;
		auto it = listParticles.begin();

		while (!found && it != listParticles.end()) {
			if (!(*it)->isActive()) {

				float x = (float(rand() % 100) * 1.25) - 80;
				float y = (float(rand() % 100) * 1.25);
				float z = (float(rand() % 100) * 1.25) - 80;

				(*it)->setPosition(posSystem);
				(*it)->setVelocity(Vector3(x, y, z));
				(*it)->setAcceleration(Vector3(0, -10.0, 0));
				(*it)->activateParticle();
				found = true;
			}
			++it;
		}

		timeNextParticle = 0;
	}
}

