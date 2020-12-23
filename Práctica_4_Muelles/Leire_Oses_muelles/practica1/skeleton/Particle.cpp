#include "Particle.h"

Particle::~Particle()
{
	if(rItem != nullptr)rItem->release();

	delete geo;
}

void Particle::integrate(float t)
{
	// Trivial case, infinite mass --> do nothing
	if (inverse_mass <= 0.0f) return;
	// Update position
	position_ += velocity_ * t;

	Vector3 totalAcc = acceleration_;
	totalAcc += force * inverse_mass;

	// Update linear velocity
	velocity_ += totalAcc * t;
	// Impose drag (damping)
	velocity_ *= powf(damping, t);

	//Modificar el transform
	trans = PxTransform(position_);

	clearForce();

}

void Particle::addTime(double t)noexcept
{
	time_ += t;
}

void Particle::resetTime()noexcept
{
	time_ = 0;
}

void Particle::activateParticle()
{
	setActive(true);

	auto shape = CreateShape(*geo);
	rItem = new RenderItem(shape, &PxTransform(position_), color_);
	shape->release();

	trans = PxTransform(position_);
	rItem->transform = &trans;
}

void Particle::desactivateParticle()
{
	setVelocity(Vector3(0, 0, 0));
	setAcceleration(Vector3(0, 0, 0));
	clearForce();

	setActive(false);
	resetTime();

	rItem->release();
	rItem = nullptr;
}

