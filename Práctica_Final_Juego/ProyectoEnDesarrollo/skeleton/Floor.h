#pragma once
#include "RigidBody.h"
#include <list>
#include <PxPhysicsAPI.h>

using namespace physx;
using namespace std;

extern StaticRigidBody* createRigidStatic(const Vector3& t, PxShape* shape, const Vector4& color = Vector4(0, .5, 1.0, 1.0));

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

