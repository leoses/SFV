#include "RigidBodyFRegistry.h"

rbInsideASphere::rbInsideASphere(Vector3 center, float radius)
{
	radius_ = radius;
	geo = new PxSphereGeometry(radius);
}

bool rbInsideASphere::rbInsideVolume(Vector3 position)
{
	float distance = Vector3(position - center_).magnitude();

	if (distance < radius_) return true;
	else return false;
}

void rbInsideASphere::createVolume()
{
	auto shape = CreateShape(*geo);
	t = PxTransform(center_);
	volume = new RenderItem(shape, &t, Vector4(0.0, 0.0, 0.0, 0.0));
	volume->transform = &t;
	shape->release();
}

rbExplosion::rbExplosion(int forceModifier, Vector3 center, float radius) :
	rbInsideASphere(center, radius), forceModifier_(forceModifier) {}

void rbExplosion::updateForce(RigidBody* rb, float t)
{
	if (!active_ || !rbInsideVolume(rb->body->getGlobalPose().p))return;

	Vector3 dir = Vector3(center_ - rb->body->getGlobalPose().p);
	float distance = dir.magnitude();

	Vector3 acceleration = ((radius_ - distance) * (-dir.getNormalized()) * forceModifier_);

	rb->body->addForce(acceleration);
}

void rbExplosion::updateLifeTime(float t)
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

void rbExplosion::activateExplosion(const Vector3& newCenter)
{
	active_ = true;
	center_ = newCenter;
	createVolume();
}


RigidBodyFRegistry::~RigidBodyFRegistry()
{
	registrations.clear();
}

void RigidBodyFRegistry::add(RigidBody* particle, rbForceGenerator* fg)
{
	registrations.push_back({ particle, fg });
}

void RigidBodyFRegistry::remove(RigidBody* particle, rbForceGenerator* fg)
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

void RigidBodyFRegistry::remove(RigidBody* particle)
{
	auto it = registrations.begin();

	while (it != registrations.end()) {
		if ((*it).particle == particle) {//Hemos encontrado el elemento a borrar
			it = registrations.erase(it);//borramos 
		}
		else ++it;
	}
}

void RigidBodyFRegistry::clear()noexcept
{
	registrations.clear();
}

void RigidBodyFRegistry::updateForces(float t)
{
	for (RbForceRegistration p : registrations) {
		p.fg->updateForce(p.particle, t);
	}
}
