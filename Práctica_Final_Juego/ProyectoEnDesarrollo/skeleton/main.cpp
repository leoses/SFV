#include <ctype.h>
#include <PxPhysicsAPI.h>
#include <vector>
#include <list>
#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

//Includes propios
#include "checkML.h" 
#include "FireworkSystem.h"
#include "ParticleForceRegistry.h"
#include "ParticleSystem.h"
#include "RigidBody.h"
#include "RigidBodyFRegistry.h"
#include "Floor.h"
#include <iostream>
#include "Player.h"

using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;

PxMaterial* gMaterial = NULL;

PxPvd* gPvd = NULL;

PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;
ContactReportCallback gContactReportCallback;

//Añadimos aquí como variables globales los elementos necesarios para la practica
//NUMERO MÁXIMO DE PARTICULAS
constexpr int MAX_PARTICLES = 1000;

//lista con las partículas creadas
std::list<Particle*> listParticles;

std::list<RigidBodyWind*> winds_;

float maxDistanceCamera;

PxScene* getScene() noexcept { return gScene; }
PxPhysics* getPhysics() noexcept{ return gPhysics; }

#pragma region RigidBodies

RigidBody* createRigidDynamic(const Vector3 & t, PxShape* shape, const Vector3 & speed, const Vector4 & color = Vector4(0, .5, 1.0, 1.0)) {
	RigidBody* r = new RigidBody();
	r->body = gPhysics->createRigidDynamic(PxTransform(t));
	r->body->attachShape(*shape);
	r->rItem = new RenderItem(shape, r->body,color );

	gScene->addActor(*r->body);
	PxRigidBodyExt::updateMassAndInertia(*r->body, PxReal(2));
	r->body->addForce(speed * r->body->getMass());

	return r;
}

StaticRigidBody* createRigidStatic(const Vector3 & t, PxShape* shape, const Vector4 & color) {
	StaticRigidBody* r = new StaticRigidBody();
	r->body = gPhysics->createRigidStatic(PxTransform(t));
	r->body->attachShape(*shape);
	r->rItem = new RenderItem(shape, r->body, color);

	gScene->addActor(*r->body);

	return r;
}

RigidBodyFRegistry* rigidBodyForceSystem = nullptr;

void addRigidBodyToForceSystem(RigidBody* rb, rbForceGenerator* g) {
	if (rigidBodyForceSystem != nullptr)rigidBodyForceSystem->add(rb, g);
}

void removeRigidBodyFromForceSystem(RigidBody* rb) {
	if (rigidBodyForceSystem != nullptr)rigidBodyForceSystem->remove(rb);
}
#pragma endregion

#pragma region Juego
Floor* suelo = nullptr;
Player* player = nullptr;
#pragma endregion

#pragma region Funciones_Auxiliares
//void addParticleToForceSystem(Particle* particle, ParticleForceGenerator* g) {
//	if (forceSystem != nullptr)forceSystem->add(particle, g);
//}
//
//void removeParticleFromForceSystem(Particle* particle, ParticleForceGenerator* g) {
//	if (forceSystem != nullptr)forceSystem->remove(particle, g);
//}

Particle* findUnusedParticle() {
	bool found = false;
	auto it = listParticles.begin();

	while (!found && it != listParticles.end()) {
		if (!(*it)->isActive())found = true;

		else ++it;
	}

	if (found)return (*it);
	else return nullptr;
}

void createParticle(Vector3 pos, Vector3 vel) {
	Particle* p = findUnusedParticle();
	if (p != nullptr) {
		p->setPosition(pos);
		p->setVelocity(vel);
		p->activateParticle();
	}
}

void updateParticles(float t)
{
	//Actualizacion de posiciones
	for (Particle* p : listParticles) {
		if (!p->isActive())continue;
		p->addTime(t);

		//Si la partícula ya ha vivido el tiempo máximo la dejamos de renderizar
		if (p->isDead()) p->desactivateParticle();
		else p->integrate(t);
	}
}
#pragma endregion

void configurateCameraPos() {
	//Configuración de la posición inicial de la camara
	Vector3 playerIniPos = player->body->getGlobalPose().p;
	GetCamera()->setEye(Vector3(playerIniPos.x, playerIniPos.y + 50, playerIniPos.z - 50));
	playerIniPos.z += 50;
	GetCamera()->setDir(playerIniPos - GetCamera()->getEye());

	maxDistanceCamera = (playerIniPos - GetCamera()->getEye()).magnitude();
}

// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -30.0f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);
	// ------------------------------------------------------

	//Add Customed Code here
	//Creamos al jugador
	PxGeometry* geo = new PxSphereGeometry(5);
	PxShape* shape = CreateShape(*geo);
	player = new Player(shape);

	//shape->release();
	delete geo;
	
	//Creamos el nivel
	suelo = new Floor();

	rigidBodyForceSystem = new RigidBodyFRegistry();

	RigidBodyWind* viento = new RigidBodyWind(WindType::Backward, Vector3(0, 0, 50),25);
	winds_.push_back(viento);
	addRigidBodyToForceSystem(player, viento);
	
}

// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

	if (player->body->getGlobalPose().p.z  <= GetCamera()->getEye().z + player->getCurrCameraPlayerOffset()-10) {
		std::cout << "has perdido\n";
		player->playerLose();
		configurateCameraPos();
	}

	player->resetForces();
	rigidBodyForceSystem->updateForces(t);

	Vector3 playerPos = player->body->getGlobalPose().p;
	GetCamera()->followPlayer(playerPos, maxDistanceCamera, player->getCurrCameraPlayerOffset());
	
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	delete player;
	delete suelo;
	
	for (RigidBodyWind* v : winds_) {
		delete v;
	}

	for (Particle* p : listParticles) {
		delete p;
		p = nullptr;
	}

	delete rigidBodyForceSystem;

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();

	gFoundation->release();
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch (toupper(key))
	{
	case 'C':
	{
		player->changeTrack(horizontalMovement::Left);
		break;
	}
	case 'Z':
	{
		player->changeTrack(horizontalMovement::Right);
		break;
	}
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char* const*)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for (PxU32 i = 0; i < frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}