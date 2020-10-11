#include "Particle.h"

Particle::~Particle()
{
	rItem->release();
}

void Particle::integrate(float t)
{
	// Trivial case, infinite mass --> do nothing
	if (inverse_mass <= 0.0f) return;
	// Update position
	position_ += velocity_ * t;
	// Update linear velocity
	velocity_ += acceleration_ * t;
	// Impose drag (damping)
	velocity_ *= powf(damping, t);

}

