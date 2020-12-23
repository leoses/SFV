#include "Particle.h"

Particle::~Particle()
{
	rItem->release();
}

void Particle::integrate(float t)
{
	move(t);

	// Update linear velocity
	velocity_ += acceleration_ * t;
	// Impose drag (damping)
	velocity_ *= powf(damping, t);

}

void Particle::move(float t)
{
	// Trivial case, infinite mass --> do nothing
	if (inverse_mass <= 0.0f) return;
	// Update position
	position_ += velocity_ * t;

	//Modificar el transform del renderItem
	trans = PxTransform(position_);
	rItem->transform = &trans;
}

void Particle::addTime(double t)noexcept
{
	time_ += t;
}

void Particle::resetTime()noexcept
{
	time_ = 0;
}

