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

ParticleForceInsideASphere::ParticleForceInsideASphere(Vector3 center, float radius) :ParticleForceGenerator(), center_(center), radius_(radius)
{
	geo = new PxSphereGeometry(radius);
}

bool ParticleForceInsideASphere::particleInsideVolume(Vector3 position)
{
	float distance = Vector3(position - center_).magnitude();

	if (distance < radius_) return true;
	else return false;
}

void ParticleForceInsideASphere::createVolume()
{
	auto shape = CreateShape(*geo);
	t = PxTransform(center_);
	volume = new RenderItem(shape, &t, Vector4(0.0, 0.0, 0.0, 0.0));
	volume->transform = &t;
	shape->release();
}

ParticleWind::ParticleWind(Vector3 windDirection, Vector3 center, float radius):
	ParticleForceInsideASphere(center, radius), windDirection_(windDirection) {
	createVolume();
}

void ParticleWind::updateForce(Particle* particle, float t)
{		
	if (particleInsideVolume(particle->getPosition())){
		particle->addForce(windDirection_* particle->getMass());
	}
}

void ParticleExplosion::activateExplosion(const Vector3 & newCenter)
{
	active_ = true;
	center_ = newCenter;
	createVolume();
}

ParticleExplosion::ParticleExplosion(int forceModifier, Vector3 center, float radius):
	ParticleForceInsideASphere(center, radius), forceModifier_(forceModifier){}

void ParticleExplosion::updateForce(Particle* particle, float t)
{
	if (!particle->isActive() || !active_ || !particleInsideVolume(particle->getPosition()))return;

	Vector3 dir = Vector3(center_-particle->getPosition());
	float distance = dir.magnitude();

	Vector3 acceleration = ((radius_ - distance) * (-dir.getNormalized()) * forceModifier_);

	particle->addForce(acceleration*particle->getMass());

}

void ParticleExplosion::updateLifeTime(float t)
{
	if (!isActive())return;

	time += t;
	if (time >= LIFETIME) {
		active_ = false;
		time = 0.0;
		volume->release();
		volume = nullptr;
	}

}

ParticleAnchoredSpring::ParticleAnchoredSpring(Vector3 _anchor, float _k, float _restLength) :
	ParticleForceGenerator(), anchor(_anchor), k(_k), restLength(_restLength)
{
	box = new PxBoxGeometry(5, 5, 5);
	t = PxTransform(anchor);
	boxRenderItem_ = new RenderItem(CreateShape(*box), &t, Vector4(1.0, 0.0, 0.0, 1.0));
}

void ParticleAnchoredSpring::updateForce(Particle* particle, float t)
{
	Vector3 f = particle->getPosition();
	f -= anchor;
	f *= -((f.normalize() - restLength) * k);
	particle->addForce(f);
}

void ParticleAnchoredSpring::updateSpringConstant(const float increment)
{
	if(k += increment > 0)k += increment;
}
