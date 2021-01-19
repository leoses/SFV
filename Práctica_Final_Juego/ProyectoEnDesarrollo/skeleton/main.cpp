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
#include "RigidBody.h"
#include "RigidBodyFRegistry.h"
#include "Level.h"
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


#pragma region RigidBodies

StaticRigidBody* createRigidStatic(const Vector3& t, PxShape* shape, const Vector4& color) {
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
Level* nivel = nullptr;
Player* player = nullptr;
ParticleForceRegistry* particleForceRegistry = nullptr;
FireworkSystem* fireworks1 = nullptr;
FireworkSystem* fireworks2 = nullptr;
#pragma endregion

#pragma region Particulas
//NUMERO MÁXIMO DE PARTICULAS
constexpr int MAX_PARTICLES = 1000;

//lista con las partículas creadas
std::list<Particle*> listParticles;

ParticleGravity* gravedadCola_ = nullptr;

void addParticleToForceSystem(Particle* particle, ParticleForceGenerator* g) {
	if (particleForceRegistry != nullptr)particleForceRegistry->add(particle, g);
}

void removeParticleFromForceSystem(Particle* particle, ParticleForceGenerator* g) {
	if (particleForceRegistry != nullptr)particleForceRegistry->remove(particle, g);
}


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
void desactivateParticles() {
	for (Particle* p : listParticles) {
		if (!p->isActive())continue;
		p->desactivateParticle();
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

#pragma region Funciones_auxiliares
float maxDistanceCamera;
PxScene* getScene() noexcept { return gScene; }
PxPhysics* getPhysics() noexcept { return gPhysics; }
void initFireworks() { fireworks1->createFirework(0); fireworks2->createFirework(0); }
RigidBody* getPlayerRB() noexcept { return player; }
Player* getPlayer() noexcept { return player; }
#pragma endregion

void configurateCameraPos() {
	//Configuración de la posición inicial de la camara
	Vector3 playerIniPos = player->body->getGlobalPose().p;
	//Movemos la camara mas arriba y mas atras
	GetCamera()->setEye(Vector3(playerIniPos.x, playerIniPos.y + 50, playerIniPos.z - 50)); 
	playerIniPos.z += 50;
	//Hacemos que la posicion a la que apunta no sea el propio player si no que este un poco mas adelantada
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
	sceneDesc.gravity = PxVec3(0.0f, -30.0f, 0.0f); ///GRAVEDAD
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);
	// ------------------------------------------------------

	//Add Customed Code here
	//Creamos al jugador
	player = new Player();


	//Sistema de Fuerzas de los RigidBody
	rigidBodyForceSystem = new RigidBodyFRegistry();

	//Creamos el nivel
	nivel = new Level();

	//Ajustamos la camara a la posicion del jugador
	configurateCameraPos();

	//Fuegos artificiales creados cuando llegamos al final del nivel
	particleForceRegistry = new ParticleForceRegistry();
	Vector3 lastPos = nivel->getWinningAreaController()->getCenter();
	fireworks1 = new FireworkSystem(particleForceRegistry, Vector3(-30, lastPos.y, lastPos.z), Vector3(0, -10, 0));
	fireworks2 = new FireworkSystem(particleForceRegistry, Vector3(30, lastPos.y, lastPos.z), Vector3(0, -10, 0));

	gravedadCola_ = new ParticleGravity(Vector3(0, -20, 0)); 

	//Particulas utilizadas como cola para el player
	for (int i = 0; i < MAX_PARTICLES; i++) {
		Particle* p = new Particle();
		p->setLifeTime(4.0);
		listParticles.push_back(p);

		//Relacionamos las particulas creadas con los generadores de fuerzas
		if (gravedadCola_ != nullptr)addParticleToForceSystem(p, gravedadCola_);
	}
}

// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

	if (player->getMovable()) { //True mientras el jugador no haya ganado el nivel
		player->update(t);
		updateParticles(t);
		rigidBodyForceSystem->updateForces(t);
		nivel->update(t);

		//Si la camara se ha acercado mucho al jugador porque ha perdido velocidad o
		//el jugador se ha caido al vacio el jugador pierde
		if (player->body->getGlobalPose().p.z <= GetCamera()->getEye().z + player->getMinCameraOffset() || player->body->getGlobalPose().p.y <= -100) {
			//Mostramos informacion por consola
			std::cout << "Has perdido\n";
			//El jugador vuelve a empezar desde el comienzo del nivel

			player->reset();
			configurateCameraPos();
			desactivateParticles();
		}

		GetCamera()->followPlayer(player->body->getGlobalPose().p, maxDistanceCamera, player->getVelocity(), t);
	}
	else {
		particleForceRegistry->updateForces(t);
		fireworks1->update(t);
		fireworks2->update(t);
	}
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	//Borramos recursos
	delete player;
	delete nivel;
	delete fireworks1;
	delete fireworks2;
	delete particleForceRegistry;
	delete rigidBodyForceSystem;
	delete gravedadCola_;

	for (Particle* p : listParticles) {
		delete p;
	}

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
	case 'D':
	{
		//Movimiento izq si no ha ganado todavia
		if (player->getMovable()) player->changeTrack(horizontalMovement::Left);
		break;
	}
	case 'A':
	{
		//Movimiento dcha si no ha ganado todavia
		if (player->getMovable()) player->changeTrack(horizontalMovement::Right);
		break;
	}
	case 'S': {
		if (!player->getMovable()) { //GetMovable == false -> el jugador se ha pasado el nivel
			//ponemos al player de nuevo en movimiento
			player->constrainMovement(false);
			//Movemos al jugador al principio del nivel
			player->reset();
			//Resetear zona de victoria
			nivel->getWinningAreaController()->resetWinningCondition();
			//Volvemos a configurar la camara a la distancia original en la pos original
			configurateCameraPos();
			//Eliminamos los fuegos artificiales que no hayan terminado su vida util y 
			//las particulas activas de la cola del player
			fireworks1->deactivateFireworks();
			fireworks2->deactivateFireworks();
			desactivateParticles();
		}
	}
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);

	if (actor1->getName() == "Player" && actor2->getName() == "Suelo" ||
		actor2->getName() == "Player" && actor1->getName() == "Suelo") {
		//Si el jugador esta cayendo y colisiona con el suelo, mandamos una señal al player
		//para que no rebote
		if (player->body->getLinearVelocity().y < -5.0) {
			player->setPlayerFalling();
		}
	}
	else if (actor1->getName() == "Player" && actor2->getName() == "Pared" ||
		actor2->getName() == "Player" && actor1->getName() == "Pared") {
		//Si el player choca con una pared, rebota contra ella hacia el lado contrario
		player->setPlayerWallColliding();
	}
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