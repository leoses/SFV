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
//FireworkSystem* fireworkSystem1;
//FireworkSystem* fireworkSystem2;
ParticleSystem* particleSystem = nullptr;

//NUMERO MÁXIMO DE PARTICULAS
constexpr int MAX_PARTICLES = 1000;

//lista con las partículas creadas
std::list<Particle*> listParticles;

//Controlador de fuerzas
ParticleForceRegistry* forceSystem = nullptr;
ParticleGravity* gravedad_ = nullptr;
ParticleWind* wind_ = nullptr;
//ParticleAnchoredSpring* spring_ = nullptr;
ParticleExplosion* explosion_ = nullptr;
ParticleSpring* spring1_ = nullptr;
ParticleSpring* spring2_ = nullptr;

Vector3 springPos_ = Vector3(20, 50, 0);
Particle* p1 = nullptr;
Particle* p2 = nullptr;


void addParticleToForceSystem(Particle* particle, ParticleForceGenerator* g) {
	if(forceSystem != nullptr)forceSystem->add(particle, g);
}

void removeParticleFromForceSystem(Particle* particle, ParticleForceGenerator* g) {
	if (forceSystem != nullptr)forceSystem->remove(particle, g);
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
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);
	// ------------------------------------------------------

	//Add Customed Code here
	forceSystem = new ParticleForceRegistry();
	gravedad_ = new ParticleGravity(Vector3(0, -10, 0));
	explosion_ = new ParticleExplosion(200, Vector3(0,50,0), 25);
	particleSystem = new ParticleSystem(Vector3(0,0,0), 0.005);

#pragma region listaParticulas
	//for (int i = 0; i < MAX_PARTICLES; i++) {
	//	Particle* p = new Particle();
	//	p->setLifeTime(4.0);
	//	listParticles.push_back(p);

	//	//Relacionamos las particulas creadas con los generadores de fuerzas
	//	if (gravedad_ != nullptr)addParticleToForceSystem(p, gravedad_);
	//	if (wind_ != nullptr) addParticleToForceSystem(p, wind_);
	//	if (explosion_ != nullptr)addParticleToForceSystem(p, explosion_);
	//}
	//fireworkSystem1 = new FireworkSystem(forceSystem, Vector3(0,25,0),Vector3(0,-50,0));
	//fireworkSystem2 = new FireworkSystem(forceSystem, Vector3(0,25,0), Vector3(0,20,0));
#pragma endregion

	p1 = new Particle();
	p1->setPosition(springPos_);
	p1->setVelocity(Vector3(10, 0, 0));
	p1->activateParticle();
	p2 = new Particle();
	p2->setPosition(-springPos_);
	p2->setVelocity(Vector3(-10, 0, 0));
	p2->activateParticle();
	
	spring1_ = new ParticleSpring(p2, 2.0f, 9);
	spring2_ = new ParticleSpring(p1, 2.0f, 9);

	addParticleToForceSystem(p1, explosion_);
	addParticleToForceSystem(p2, explosion_);

	addParticleToForceSystem(p1, spring1_);
	addParticleToForceSystem(p2, spring2_);
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

	explosion_->updateLifeTime(t);
	forceSystem->updateForces(t);
	particleSystem->update(t);
	updateParticles(t);
	p1->integrate(t);
	p2->integrate(t);
	
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	delete forceSystem;
	delete particleSystem;
	delete gravedad_;
	delete explosion_;
	delete spring1_;
	delete spring2_;

	for (Particle* p : listParticles) {
		delete p;
		p = nullptr;
	}
	delete p1;
	delete p2;

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
		//case 'B': break;
		//case ' ':	break;
	case ' ':
	{
		if (!explosion_->isActive()) {
			explosion_->activateExplosion(75*GetCamera()->getDir() + GetCamera()->getEye()); //
		}
		break;
	}
	case '+':
	{
		spring1_->updateSpringConstant(1);
		spring2_->updateSpringConstant(1);
		break;
	}
	case '-':
	{
		spring1_->updateSpringConstant(-1);
		spring2_->updateSpringConstant(-1);
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