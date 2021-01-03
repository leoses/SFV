#pragma once
#include "ParticleContact.h"
class ParticleLink
{
public:
	//Holds the pair of particles that are connected by this link.
	Particle* particle[2];
	virtual unsigned addContact(ParticleContact* contact) const = 0;
	ParticleLink(Particle* p1, Particle* p2) { particle[0] = p1; particle[1] = p2; }
protected:
	float currentLength() const;
};

class ParticleCable : public ParticleLink
{
public:
	float maxLength;
	float restitution;
	virtual unsigned addContact(ParticleContact* contact) const;
	ParticleCable(Particle* p1, Particle* p2) :ParticleLink(p1, p2){}
};

