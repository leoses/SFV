#include "Particle.h"

Particle::~Particle()
{
	if(rItem != nullptr)rItem->release();

	delete geo;
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

	//Modificar el transform
	trans = PxTransform(position_);
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
	rItem = new RenderItem(shape, &PxTransform(position_), Vector4(255, 0, 0, 1));
	shape->release();

	trans = PxTransform(position_);
	rItem->transform = &trans;
}

void Particle::desactivateParticle()
{
	setActive(false);
	resetTime();
	rItem->release();
	rItem = nullptr;
}

