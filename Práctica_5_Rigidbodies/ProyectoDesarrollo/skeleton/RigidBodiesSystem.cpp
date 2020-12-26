#include "RigidBodiesSystem.h"

RigidBodiesSystem::RigidBodiesSystem(Vector3 posSystem_, float timeBetweenSolidRigids): 
	posSystem(posSystem_), timeCreateSolidRigid(timeBetweenSolidRigids)
{
	PxGeometry* geo = new PxSphereGeometry(2);//new PxBoxGeometry(Vector3(1,1,1));
	shape = CreateShape(*geo);
	delete geo;
	//inicializar generador randoms
	srand(time(NULL));
}

void RigidBodiesSystem::updateSystem(float t)
{
	timeNextSolidRigid += t;

	if (rigidBodies.size() < MAX_SOLIDRIGID && timeNextSolidRigid > timeCreateSolidRigid) {
		while (rigidBodies.size() < MAX_SOLIDRIGID && timeNextSolidRigid > timeCreateSolidRigid) {

			float x = (float(rand() % 2000)*1.25);
			float z = (float(rand() % 2000)*1.25);
			RigidBody* r = createRigidDynamic(posSystem, shape, Vector3(x,0,z));
			rigidBodies.push_back(r);
			timeNextSolidRigid -= timeCreateSolidRigid;
		}

		timeNextSolidRigid = 0;
	}


	for (auto it = rigidBodies.begin(); it != rigidBodies.end();) {
		(*it)->lifeTime += t;
		if ((*it)->lifeTime >= RIGIDBODY_LIFE_TIME) {
			removeRigidBodyFromForceSystem((*it));
			delete (*it);
			it = rigidBodies.erase(it);
		}
		else  ++it;
	}
}
