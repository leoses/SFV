#pragma once
#include "RigidBody.h"
#include <list>
#include <PxPhysicsAPI.h>
#include "RigidBodyFRegistry.h"
#include "RigidBodyFRegistry.h"
#include "checkML.h"
#include "CoinSystem.h"

class Player;

using namespace physx;
using namespace std;

extern StaticRigidBody* createRigidStatic(const Vector3& t, PxShape* shape, const Vector4& color = Vector4(0, .5, 1.0, 1.0));
extern RigidBody* getPlayerRB() noexcept;
extern void addRigidBodyToForceSystem(RigidBody* rb, rbForceGenerator* g);
extern PxScene* getScene() noexcept;
extern Player* getPlayer() noexcept;

enum Element{Suelo, Pared};

struct LevelElement {
	Vector3 centerPosition_;
	Element elem_;
};

struct WindElement {
	WindType wType;
	Vector3 pos;
	float radius;
};


class Level
{
public:
	Level();
	virtual ~Level();
	//Actualizacion de las colisiones de las monedas
	void update(float t) { cSystem->updateSystem(t, getPlayerRB()); }
	//Devuelve el trigger final del juego que marca la meta
	rbInsideBox* getWinningAreaController() noexcept { return winningArea; }

protected:
	void initializeLevel();

	//Metodo de configuracion de los vientos 
	void initializeWinds();
	//Dado un suelo crea 2 paredes a sus lados
	void createWalls(const LevelElement floor);

private:
	//vector para almacenar los suelos y paredes para su posterior eliminacion
	list<StaticRigidBody*> floorElements;

	//vectores de configuración
	vector<LevelElement> levelInfo;
	
	//Area que al llegar a ella marca el final del juego
	rbInsideBox* winningArea = nullptr;

	//Sistema que gestiona las monedas y las colisiones de las mismas con el player
	CoinSystem* cSystem = nullptr;
	
	//Vientos del nivel
	list<RigidBodyWind*> winds_;
	const int COINS_PER_FLOOR = 6;

	//Constantes para las dimensiones de las paredes y de los suelos
	const Vector3 FLOOR_SIZE = Vector3(40, 1, 200);
	const Vector3 WINNING_AREA_DIMENSIONS = Vector3(50, 2, 20);
	const float WALL_WIDTH = 10;
	const float WALL_HEIGHT = 80;
};

