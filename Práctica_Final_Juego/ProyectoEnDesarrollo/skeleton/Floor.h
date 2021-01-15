#pragma once
#include "RigidBody.h"
#include <list>
#include <PxPhysicsAPI.h>

using namespace physx;
using namespace std;

extern StaticRigidBody* createRigidStatic(const Vector3& t, PxShape* shape, const Vector4& color = Vector4(0, .5, 1.0, 1.0));

enum Element{Suelo, Pared};

struct LevelElement {
	Vector3 centerPosition_;
	Element elem_;
};

class Floor
{
public:
	Floor();
	virtual ~Floor();
protected:
	void initializeLevel();
	void createWalls(const LevelElement floor);
private:
	//vector para almacenar los suelos y paredes para su posterior eliminacion
	list<StaticRigidBody*> floorElements;
	//vector de configuración
	vector<LevelElement> levelInfo;
};

