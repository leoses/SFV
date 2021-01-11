#pragma once
#include "RigidBody.h"
#include <list>
#include <PxPhysicsAPI.h>

using namespace physx;
using namespace std;

extern StaticRigidBody* createRigidStatic(Vector3 t, PxShape* shape);

struct FloorLevel {
	Vector3 dimensions_;
	Vector3 centerPosition_;
};

class Floor
{
public:
	Floor();
	virtual ~Floor();
protected:
	void initializeLevel();
private:
	//vector de configuración
	list<StaticRigidBody*> floorElements;
	//vector para almacenar los suelos para su posterior eliminacion
	vector<FloorLevel> levelInfo;
};

