#include "ParticleForceRegistry.h"

void ParticleGravity::updateForce(Particle* particle, float t)
{
	if (particle->getInverseMass() <= 0.0f) return;
	particle->addForce(g * particle->getMass());
}

ParticleForceRegistry::~ParticleForceRegistry()
{
	registrations.clear();
}

void ParticleForceRegistry::add(Particle* particle, ParticleForceGenerator* fg)
{
	registrations.push_back({ particle, fg });
}

void ParticleForceRegistry::remove(Particle* particle, ParticleForceGenerator* fg)
{
	auto it = registrations.begin();
	bool encontrado = false;

	while (it != registrations.end() && !encontrado) {
		if ((*it).particle == particle && (*it).fg == fg) {//Hemos encontrado el elemento a borrar
			it = registrations.erase(it);//borramos 
			encontrado = true;//hemos terminado la busqueda
		}
		else {
			++it;
		}
	}

}

void ParticleForceRegistry::remove(Particle* particle)
{
	auto it = registrations.begin();

	while (it != registrations.end()) {
		if ((*it).particle == particle) {//Hemos encontrado el elemento a borrar
			it = registrations.erase(it);//borramos 
		}
		else ++it;
	}
}

void ParticleForceRegistry::clear()
{
	registrations.clear();
}

void ParticleForceRegistry::updateForces(float t)
{
	for (ParticleForceRegistration p : registrations) {
		//if (p.particle->isActive())
		p.fg->updateForce(p.particle, t);
		//else {
		//	remove(p.particle, p.fg);
		//}
	}
}
