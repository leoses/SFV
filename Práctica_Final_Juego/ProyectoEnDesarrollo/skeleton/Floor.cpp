#include "Floor.h"

Floor::Floor()
{
	initializeLevel();

	for (FloorLevel level : levelInfo) {
		PxGeometry* geo = new PxBoxGeometry(level.dimensions_);
		PxShape* shape = CreateShape(*geo);
		StaticRigidBody* r = createRigidStatic(level.centerPosition_, shape);
		floorElements.push_back(r);
		shape->release();
		delete geo;
	}
}

Floor::~Floor()
{
	for (StaticRigidBody* r : floorElements) {
		r->rItem->release();
		delete r;
	}

}

void Floor::initializeLevel()
{
	levelInfo.push_back({//at (0)
		Vector3(50,1,100),
		Vector3(0,0,0)
	});

	levelInfo.push_back({//at 1
		Vector3(50,1,100),
		Vector3(0,-25,150)
	});
}
