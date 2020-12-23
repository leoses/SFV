#pragma once
#include <PxPhysicsAPI.h>
using namespace physx;

class RigidBodyForceGenerator
{
public:
	// Overload to provide functionality
	virtual void updateForce(PxRigidDynamic* rb, float t) = 0;
};

