#include "Level.h"
#include <ctime>

Level::Level()
{
	srand(time(NULL));

	initializeLevel();

	PxGeometry* paredes = new physx::PxBoxGeometry(10, 50, 200);
	PxGeometry* suelos = new physx::PxBoxGeometry(50, 1, 200);

	PxShape* wallShape = CreateShape(*paredes);
	PxShape* floorShape = CreateShape(*suelos);


	for (const LevelElement& level : levelInfo) {
		StaticRigidBody* r = nullptr;
		switch (level.elem_)
		{
		case Element::Suelo:
			r = createRigidStatic(level.centerPosition_, floorShape);
			r->body->setName("Suelo");
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

Level::~Level()
{
	for (StaticRigidBody* r : floorElements) {
		delete r;
	}

	for (RigidBodyWind* w : winds_) {
		delete w;
	}

	delete winningArea;
}


void Level::initializeLevel()
{
	levelInfo.push_back({//at (0)
		Vector3(0,0,0),
		Element::Suelo
	});

	levelInfo.push_back({//at (1)
		Vector3(0,-50,400),
		Element::Suelo
	});

	levelInfo.push_back({//at (1)
		Vector3(0,50,420),
		Element::Suelo
	});

	levelInfo.push_back({//at (1)
		Vector3(0,-50,800),
		Element::Suelo
	});

	/*for (int i = 0; i < levelInfo.size(); i++) {
		for (int j = 0; j < WINDS_PER_FLOOR; j++) {

			Vector3 c = levelInfo[i].centerPosition_;

			float x = -30 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (30 - (-30))));
			float z = c.z - 180 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (180 - (-180))));

			RigidBodyWind* wind = new RigidBodyWind(static_cast<WindType>(rand() % 2), Vector3(x, c.y, z), 20);

			winds_.push_back(wind);

			addRigidBodyToForceSystem(getPlayer(), wind);
		}
	}*/

	RigidBodyWind* wind = new RigidBodyWind(WindType::Upwards, Vector3(0, 0, 160), 20);
	winds_.push_back(wind);
	addRigidBodyToForceSystem(getPlayer(), wind);

	Vector3 center = levelInfo[levelInfo.size() - 1].centerPosition_;
	center.z += 200;
	winningArea = new rbInsideBox(Vector3(50, 2, 20), center, Vector4(255, 255, 255, 0.0));
	
	//Trigger del final del nivel
	addRigidBodyToForceSystem(getPlayer(), winningArea);

	int suelos = levelInfo.size();


	for (int i = 0; i < suelos; i++) {
		createWalls(levelInfo.at(i));
	}
}

void Level::createWalls(const LevelElement floor)
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
