#pragma once
#include "Particle.h"

class ParticleContact
{
public:
	// Particles involved in the collision
	Particle* particle[2];
	// Restitution coefficient
	float restitution;
	// Contact normal,
	// from the first particle perspective
	Vector3 contactNormal;
	// How "intense" is the collision between the 2 particles
	float penetration;
	// Resolves this contact for both velocity and interpenetration.

	ParticleContact(){}
	void resolve(float t);
	float calculateSeparatingVelocity() const;

private:
	void resolveVelocity(float t);
	void resolveInterpenetration(float t);
};



