#include "ParticleLink.h"

float ParticleLink::currentLength() const
{
	Vector3 relativePos = particle[0]->getPosition() - particle[1]->getPosition();
	return relativePos.magnitude();

}

unsigned ParticleCable::addContact(ParticleContact* contact) const
{
	// Find the length of the cable.
	float length = currentLength();
	// Check if we’re overextended.
	if (length < maxLength)
	{
		return 0;
	}
	// Otherwise, return the contact.
	contact->particle[0] = particle[0];
	contact->particle[1] = particle[1];
	// Calculate the normal.Add information to contact: normal, penetration & restitution.
	contact->contactNormal = (particle[1]->getPosition() - particle[0]->getPosition()).getNormalized();
	contact->penetration = length - maxLength;
	contact->restitution = restitution;
	
	return 1;
}
