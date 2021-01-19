#pragma once
#include "Firework.h"
#include "ParticleForceRegistry.h"
#include <vector>
#include <cstdlib>
#include <ctime>

struct Payload
{
	// Type of firework to create
	unsigned type;
	//number of particles that 
	unsigned count;
};

struct FireworkRule
{
	// Type of firework configured
	unsigned type;
	float minAge;
	float maxAge;
	// Minimum relative velocity
	Vector3 minVelocity, maxVelocity;
	float damping;
	Vector4 color;
	 
	std::vector<Payload> payloads;

	void create(Firework* firework, Firework* parent = nullptr) const
	{
		firework->setType(type);
		firework->setLifeTime(minAge + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxAge -minAge))));
		Vector3 vel = Vector3(0,0,0);

		if (parent != nullptr)
		{ // inherit velocity from parent
			firework->setPosition(parent->getPosition());
			vel = parent->getVelocity();
		}
		else
		{
			firework->setPosition(Vector3(0,0,0));
		}

		float x = minVelocity.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxVelocity.x - minVelocity.x)));
		float y = minVelocity.y + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxVelocity.y - minVelocity.y)));
		float z = minVelocity.z + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxVelocity.z - minVelocity.z)));


		vel += Vector3(x,y,z);
		firework->setVelocity(vel);

		firework->setColor(color);

		// Force the mass to 1
		firework->setMass(1);
		firework->setDamping(damping);
		//firework->setAcceleration(PxVec3(0, -10, 0));
		firework->resetTime();

	}

};

class FireworkSystem
{
private:
	std::vector<FireworkRule> rules;
	std::list<Firework*> fireworkElements;

	Vector3 posSystem_;
	ParticleGravity* systemGravity_ = nullptr;

	ParticleForceRegistry* forceController = nullptr;

	const int MAX_FIREWORKS = 100;
public:
	
	FireworkSystem(ParticleForceRegistry* fr, Vector3 initialSystemPos = Vector3(0,0,0), Vector3 systemGravity = Vector3(0, -10, 0));
	~FireworkSystem();

	void initFireworkRules();
	void update(float t);
	void deactivateFireworks();
	void createFirework(unsigned type, Firework* parent= nullptr);


	Firework* findInactiveFirework();
	FireworkRule* getFireworkRule(int  rule)  { return &rules.at(rule); }
};

