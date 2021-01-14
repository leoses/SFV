#pragma once
#include <PxPhysicsAPI.h>
#include "checkML.h"
#include "core.hpp"
#include "RenderUtils.hpp"
#include <ctime>
#include <list>
#include "RigidBody.h"

using namespace physx;
using namespace std;

extern RigidBody* createRigidDynamic(const Vector3& t, PxShape* shape, const Vector3& speed);
extern void removeRigidBodyFromForceSystem(RigidBody* rb);

constexpr int  MAX_SOLIDRIGID = 100;
constexpr float RIGIDBODY_LIFE_TIME = 10.0;


class RigidBodiesSystem
{
private:
	//Variables para el manejo de la creacion de partículas
	float timeNextSolidRigid= 0;
	float timeCreateSolidRigid;

	//Posición desde la que se van a crear las partículas
	Vector3 posSystem;

	list<RigidBody*> rigidBodies;
	PxShape* shape = nullptr;

public:
	RigidBodiesSystem(Vector3 posSystem_, float timeBetweenSolidRigids = 1);
	~RigidBodiesSystem() {
		for (RigidBody* r : rigidBodies) {
			delete r;
		} 
		shape->release();
	};

	void updateSystem(float t);
};

