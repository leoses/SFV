#include "Floor.h"

Floor::Floor()
{
	initializeLevel();

	PxGeometry* paredes = new physx::PxBoxGeometry(10, 50, 100);
	PxGeometry* suelos = new physx::PxBoxGeometry(50, 1, 100);

	PxShape* wallShape = CreateShape(*paredes);
	PxShape* floorShape = CreateShape(*suelos);


	for (const LevelElement& level : levelInfo) {
		StaticRigidBody* r = nullptr;
		switch (level.elem_)
		{
		case Element::Suelo:
			r = createRigidStatic(level.centerPosition_, floorShape);
			break;

		case Element::Pared:
			r = createRigidStatic(level.centerPosition_, wallShape);
			break;

		default:
			break;
		}
		if (r != nullptr)floorElements.push_back(r);
	}

	wallShape->release();
	floorShape->release();

	delete paredes;
	delete suelos;

}

Floor::~Floor()
{
	for (StaticRigidBody* r : floorElements) {
		delete r;
	}
}

void Floor::initializeLevel()
{
	levelInfo.push_back({//at (0)
		Vector3(0,0,0),
		Element::Suelo
	});

	levelInfo.push_back({//at (1)
		Vector3(0,-25,150),
		Element::Suelo
	});

	int suelos = levelInfo.size();


	for (int i = 0; i < suelos; i++) {
		createWalls(levelInfo.at(i));
	}
}

void Floor::createWalls(const LevelElement floor)
{
	levelInfo.push_back({
		Vector3(50 + 10, floor.centerPosition_.y, floor.centerPosition_.z),
		Element::Pared
	});

	levelInfo.push_back({
		Vector3(-50 - 10, floor.centerPosition_.y, floor.centerPosition_.z),
		Element::Pared
	});
}
