#pragma once
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include "core.hpp"
#include "RigidBody.h"
#include <list>


using namespace physx;

//Clase abstracta
class rbForceGenerator
{
public:
	// Overload to provide functionality
	virtual void updateForce(RigidBody* rb, float t) = 0;
};

class rbInsideASphere : public rbForceGenerator {
protected:
	//RenderItem
	PxTransform t;
	RenderItem* volume = nullptr;
	PxSphereGeometry* geo = nullptr;

	//Elementos que determinan el volumen de la esfera
	Vector3 center_;
	float radius_;

public:
	rbInsideASphere(Vector3 center = Vector3(0, 0, 0), float radius = 10);
	virtual ~rbInsideASphere() { if (volume != nullptr) volume->release(); delete geo; }

	bool rbInsideVolume(Vector3 position);
	void createVolume();
};

class rbExplosion :public rbInsideASphere {
private:
	const float LIFETIME = 1.0;
	float time = 0.0;
	int forceModifier_ = 1;
	bool active_ = false;

public:
	rbExplosion(int forceModifier, Vector3 center = Vector3(0, 0, 0), float radius = 10);
	virtual ~rbExplosion() {}

	virtual void updateForce(RigidBody* rb, float t)override;
	void updateLifeTime(float t);
	void activateExplosion(const Vector3& newCenter);

	//Getters
	const bool isActive() noexcept { return active_; }

};

class RigidBodyFRegistry
{
protected:
	// Storage for generator-particle entry
	struct RbForceRegistration
	{
		RigidBody* particle;
		rbForceGenerator* fg;
	};
	typedef std::list<RbForceRegistration> RegistryRB;
	RegistryRB registrations;
public:
	RigidBodyFRegistry()noexcept {};
	virtual ~RigidBodyFRegistry();

	// Associate generator with a particle
	void add(RigidBody* particle, rbForceGenerator* fg);
	// Remove association
	void remove(RigidBody* particle, rbForceGenerator* fg);
	// Remove association of all ParticleForceGenerator from one particle;
	void remove(RigidBody* particle);
	// Removes all associations. Particle and Generators won't be deleted
	void clear() noexcept;
	// Update all the generators in the registry
	virtual void updateForces(float t);

};

