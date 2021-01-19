#include "Level.h"
#include <ctime>

Level::Level()
{
	srand(time(NULL));

	initializeLevel();

	PxGeometry* paredes = new physx::PxBoxGeometry(WALL_WIDTH,WALL_HEIGHT, FLOOR_SIZE.z);
	PxGeometry* suelos = new physx::PxBoxGeometry(FLOOR_SIZE);

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
			r->body->setName("Pared");
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

	delete cSystem;
	delete winningArea;
}


void Level::initializeLevel()
{
	float iniZpos = 0;
	float offset = FLOOR_SIZE.z * 2;

#pragma region Suelos
	levelInfo.push_back({//at (0)
	Vector3(0,0,iniZpos),
	Element::Suelo
		});

	iniZpos += offset;
	levelInfo.push_back({//at (1)
		Vector3(0,-50,iniZpos),
		Element::Suelo
		});

	levelInfo.push_back({//at (1)
		Vector3(0,50,iniZpos + 20),
		Element::Suelo
		});

	iniZpos += offset;
	levelInfo.push_back({//at (1)
		Vector3(0,-50,iniZpos),
		Element::Suelo
		});

	levelInfo.push_back({//at (1)
		Vector3(0,50,iniZpos + 20),
		Element::Suelo
		});

	iniZpos += offset;

	levelInfo.push_back({//at (1)
	Vector3(0,0,iniZpos + 20),
	Element::Suelo
		});
#pragma endregion

	//Colocamos monedas por el nivel
	cSystem = new CoinSystem(getPlayer()->getPlayerSize());
	cSystem->inizializeCoins(levelInfo, COINS_PER_FLOOR);

	//Colocamos los vientos
	initializeWinds();

	//Colocamos la meta al final del ultimo suelo 
	Vector3 center = levelInfo[levelInfo.size() - 1].centerPosition_;
	center.z += FLOOR_SIZE.z - WINNING_AREA_DIMENSIONS.z;
	winningArea = new rbInsideBox(WINNING_AREA_DIMENSIONS, center, Vector4(255, 255, 255, 0.0));

	//Trigger del final del nivel
	addRigidBodyToForceSystem(getPlayerRB(), winningArea);

	//Añadimos al vector de configuración las paredes correspondientes
	//a los suelos que habíamos configurado
	int suelos = levelInfo.size();
	for (int i = 0; i < suelos; i++) {
		createWalls(levelInfo.at(i));
	}
}

void Level::initializeWinds()
{
	vector<WindElement> windInfo;

#pragma region PrimerSuelo
	windInfo.push_back({//at 0
		WindType::Upwards,
		Vector3(0, 0, 160),
		20
		});

	windInfo.push_back({//at 1
		WindType::Forward,
		Vector3(-30, 0, 160),
		15
		});

	windInfo.push_back({//at 2
		WindType::Backward,
		Vector3(30, 0, 160),
		15
		});
#pragma endregion

#pragma region SuelosAltos
	windInfo.push_back({//at 0
		WindType::Backward,
		Vector3(0, 50, 340),
		20
		});

	windInfo.push_back({//at 1
		WindType::Backward,
		Vector3(-20, 50, 380),
		15
		});

	windInfo.push_back({//at 2
		WindType::Backward,
		Vector3(20, 50, 400),
		20
		});

	windInfo.push_back({//at 2
		WindType::Forward,
		Vector3(-20, 50, 460),
		18
	});

	windInfo.push_back({//at 2
		WindType::Forward,
		Vector3(0, 50, 520),
		10
		});

	windInfo.push_back({//at 2
		WindType::Backward,
		Vector3(20, 50, 540),
		20
		});

	windInfo.push_back({//at 2
		WindType::Backward,
		Vector3(5, 50, 580),
		20
		});

	windInfo.push_back({//at 2
		WindType::Backward,
		Vector3(-15, 50, 620),
		20
		});

	windInfo.push_back({//at 2
		WindType::Forward,
		Vector3(0, 50, 720),
		20
	});
	windInfo.push_back({//at 2
		WindType::Forward,
		Vector3(10, 50, 820),
		20
	});

	windInfo.push_back({//at 2
		WindType::Backward,
		Vector3(20, 50,900),
		20
		});
#pragma endregion

#pragma region SuelosBajos

	windInfo.push_back({//at 0
		WindType::Upwards,
		Vector3(0, -50, 960),
		20
		});

	windInfo.push_back({//at 1
		WindType::Upwards,
		Vector3(-30, -50, 960),
		20
		});

	windInfo.push_back({//at 2
		WindType::Upwards,
		Vector3(30, -50, 960),
		20
		});

//------------------------------------------

	windInfo.push_back({//at 0
		WindType::Backward,
		Vector3(0, -50, 900),
		16
		});

	windInfo.push_back({//at 1
		WindType::Backward,
		Vector3(-30, -50, 900),
		16
		});

	windInfo.push_back({//at 2
		WindType::Backward,
		Vector3(30, -50, 900),
		16
	});

	windInfo.push_back({//at 0
		WindType::Backward,
		Vector3(0, -50, 440),
		20
		});

	windInfo.push_back({//at 1
		WindType::Backward,
		Vector3(-20, -50, 380),
		15
		});

	windInfo.push_back({//at 2
		WindType::Forward,
		Vector3(20, -50, 400),
		20
		});

	windInfo.push_back({//at 2
		WindType::Forward,
		Vector3(-20, -50, 460),
		18
		});

	windInfo.push_back({//at 2
		WindType::Forward,
		Vector3(0, -50, 540),
		10
		});

	windInfo.push_back({//at 2
		WindType::Backward,
		Vector3(10, -50, 500),
		10
		});

	windInfo.push_back({//at 2
		WindType::Forward,
		Vector3(20, -50, 520),
		20
		});

	windInfo.push_back({//at 2
		WindType::Backward,
		Vector3(5, -50, 580),
		20
		});

	windInfo.push_back({//at 2
		WindType::Backward,
		Vector3(-15, -50, 620),
		20
		});

	windInfo.push_back({//at 2
		WindType::Forward,
		Vector3(0, -50, 820),
		20
		});
	windInfo.push_back({//at 2
		WindType::Forward,
		Vector3(10, -50, 720),
		20
		});

#pragma endregion

#pragma region SueloFinal
	windInfo.push_back({//at 2
	WindType::Forward,
	Vector3(-20, 0, 1150),
	18
		});

	windInfo.push_back({//at 2
		WindType::Forward,
		Vector3(0, 0, 1170),
		10
		});

	windInfo.push_back({//at 2
		WindType::Backward,
		Vector3(20, 0, 1190),
		20
		});

	windInfo.push_back({//at 2
		WindType::Backward,
		Vector3(5, 0, 1300),
		20
		});

	windInfo.push_back({//at 2
		WindType::Forward,
		Vector3(-5, 0, 1320),
		20
		});

	windInfo.push_back({//at 2
		WindType::Backward,
		Vector3(-25, 0, 1360),
		13
		});
	windInfo.push_back({//at 2
		WindType::Backward,
		Vector3(25, 0, 1360),
		13
		});
	windInfo.push_back({//at 2
		WindType::Backward,
		Vector3(0, 0, 1260),
		20
		});

#pragma endregion

	for (WindElement w : windInfo) {

		RigidBodyWind* wind = new RigidBodyWind(w.wType, w.pos, w.radius);
		winds_.push_back(wind);
		addRigidBodyToForceSystem(getPlayerRB(), wind);
	}
}


void Level::createWalls(const LevelElement floor)
{
	levelInfo.push_back({
		Vector3(FLOOR_SIZE.x + WALL_WIDTH, floor.centerPosition_.y, floor.centerPosition_.z),
		Element::Pared
		});

	levelInfo.push_back({
		Vector3(-FLOOR_SIZE.x - WALL_WIDTH, floor.centerPosition_.y, floor.centerPosition_.z),
		Element::Pared
		});
}

