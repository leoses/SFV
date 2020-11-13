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

void ParticleForceRegistry::clear()noexcept
{
	registrations.clear();
}

void ParticleForceRegistry::updateForces(float t)
{
	for (ParticleForceRegistration p : registrations) {
		p.fg->updateForce(p.particle, t);
	}
}

ParticleWind::ParticleWind(Vector3 windDirection, Vector3 center, float radius):windDirection_(windDirection), center_(center), radius_(radius) {
	PxSphereGeometry* geo = new PxSphereGeometry(radius);
	auto shape = CreateShape(*geo);
	t = PxTransform(center);
	volume = new RenderItem(shape, &t, Vector4(0.0, 0.0, 0.0, 0.0));
	volume->transform = &t;
	shape->release();
	delete geo;
}

void ParticleWind::updateForce(Particle* particle, float t)
{		
	if (particleInsideVolume(particle->getPosition())){
		particle->addForce(windDirection_* particle->getMass());
	}
}

bool ParticleWind::particleInsideVolume(Vector3 position)
{
	float distance = abs(Vector3(position - center_).magnitude());

	if (distance < radius_) return true;
	else return false;
}
