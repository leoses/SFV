#include "ParticleContact.h"

void ParticleContact::resolve(float t)
{
	resolveVelocity(t);
	resolveInterpenetration(t);
}

float ParticleContact::calculateSeparatingVelocity() const
{
	Vector3 relativeVelocity = particle[0]->getVelocity();
	if (particle[1]) relativeVelocity -= particle[1]->getVelocity();
	return relativeVelocity.dot(contactNormal);
}

void ParticleContact::resolveVelocity(float t)
{
	// Get current separating velocity
	float separatingVelocity1 = calculateSeparatingVelocity();

	//if separatingVelocity greater than 0, no impulse needeed 
	//because particles are eather getting apart or at the same distance
	if (separatingVelocity1 >= 0) return;
	//Two objects have infiniteMass ->no movement so no impulse
	if (particle[0]->getInverseMass() <= 0.0f && particle[1]->getInverseMass() <= 0.0f) return;

	// Compute separating velocity after collision
	//separatingVelocity at the next moment
	float separatingVelocity2 = (-restitution) * separatingVelocity1;

	//Computing diference between current and next separation velocities
	//to calculate impulse 
	float deltaSepVelocity = separatingVelocity2 - separatingVelocity1;

	// Compute total impulse -> g = m.v = v/(m^-1)
	//units kg*m/s
	float impulse = deltaSepVelocity / (particle[0]->getInverseMass() + particle[1]->getInverseMass());

	//Direction in which the impulse is applied
	Vector3 totalImpulseInContact = contactNormal * impulse; // kg*m/s

	// Set new velocity for each particle
	//newvel = oldvel + 1/m*imPerIMass
	//units m/s -> m/s, 1/kg * kg * m/s = m/s, m/s;
	particle[0]->setVelocity(particle[0]->getVelocity() + totalImpulseInContact * particle[0]->getInverseMass());
	particle[1]->setVelocity(particle[1]->getVelocity() - totalImpulseInContact * particle[1]->getInverseMass()); //opossite dir

}

void ParticleContact::resolveInterpenetration(float t)
{
	// Check if there is interpenetration
	if (penetration <= 0) return;
	float totalInvMass = particle[0]->getInverseMass() + particle[1]->getInverseMass();
	
	//Two objects have infiniteMass ->no movement so no need to change position
	if (totalInvMass <= 0) return;

	// Calculate factor of each particle depending on mass
	//Vector3 displacementPerIMass = contactNormal / (-penetration * totalInvMass);
	Vector3 displacementPerIMass = contactNormal *(-penetration / totalInvMass);

	
	// Calculate the movement for each particle and apply movement to the position of each particle
	particle[0]->setPosition(particle[0]->getPosition() + displacementPerIMass * particle[0]->getInverseMass());
	particle[1]->setPosition(particle[1]->getPosition() + displacementPerIMass * particle[1]->getInverseMass());

}