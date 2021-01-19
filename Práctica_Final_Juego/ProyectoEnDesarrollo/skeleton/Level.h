#pragma once
#include "RigidBody.h"
#include <list>
#include <PxPhysicsAPI.h>
#include "RigidBodyFRegistry.h"
#include "RigidBodyFRegistry.h"

class Player;

using namespace physx;
using namespace std;

extern StaticRigidBody* createRigidStatic(const Vector3& t, PxShape* shape, const Vector4& color = Vector4(0, .5, 1.0, 1.0));
extern RigidBody* getPlayer() noexcept;
extern void addRigidBodyToForceSystem(RigidBody* rb, rbForceGenerator* g);
enum Element{Suelo, Pared};

struct LevelElement {
	Vector3 centerPosition_;
	Element elem_;
};

class Level
{
public:
	Level();
	virtual ~Level();

	rbInsideBox* getWinningAreaController() noexcept { return winningArea; }
protected:
	void initializeLevel();
	void createWalls(const LevelElement floor);
private:
	//vector para almacenar los suelos y paredes para su posterior eliminacion
	list<StaticRigidBody*> floorElements;
	//vector de configuración
	vector<LevelElement> levelInfo;
	//Area que al llegar a ella marca el final del juego
	rbInsideBox* winningArea = nullptr;
	//Vientos del nivel
	std::list<RigidBodyWind*> winds_;
	const int WINDS_PER_FLOOR = 6;
};

