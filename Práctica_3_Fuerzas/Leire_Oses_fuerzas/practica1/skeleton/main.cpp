#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>
#include <list>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

//Includes propios
#include "checkML.h" //Basura
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

//A�adimos aqu� como variables globales los elementos necesarios para la practica
//FireworkSystem* fireworkSystem1;
//FireworkSystem* fireworkSystem2;
ParticleForceRegistry* forceSystem = nullptr;

//NUMERO M�XIMO DE PARTICULAS
constexpr int MAX_PARTICLES = 10000;

//lista con las part�culas creadas
std::list<Particle*> listParticles;
ParticleSystem* particleSystem = nullptr;

void addParticleToForceSystem(Particle* particle, ParticleForceGenerator* g) {
	if(forceSystem != nullptr)forceSystem->add(particle, g);
}

void removeParticleToForceSystem(Particle* particle, ParticleForceGenerator* g) {
	if (forceSystem != nullptr)forceSystem->remove(particle, g);
}

Particle* findUnusedParticle() {
	bool found = false;
	auto it = listParticles.begin();

	while (!found && it != listParticles.end()) {
		if (!(*it)->isActive())found = true;
		
		else ++it;
	}

	if (found) return (*it);
	else return nullptr;
}

void updateParticles(float t)
{
	//Actualizacion de posiciones
	for (Particle* p : listParticles) {
		if (!p->isActive())continue;

		p->addTime(t);

		//Si la part�cula ya ha vivido el tiempo m�ximo la dejamos de renderizar
		if (p->isDead()) p->desactivateParticle();
		else  p->integrate(t);
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
	particleSystem = new ParticleSystem(Vector3(0,0,0));


	for (int i = 0; i < MAX_PARTICLES; i++) {
		Particle* p = new Particle();
		p->setLifeTime(4.0);
		listParticles.push_back(p);
	}
	//fireworkSystem1 = new FireworkSystem(forceSystem, Vector3(0,25,0),Vector3(0,-50,0));
	//fireworkSystem2 = new FireworkSystem(forceSystem, Vector3(0,25,0), Vector3(0,20,0));

}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

	forceSystem->updateForces(t);
	particleSystem->update(t);
	updateParticles(t);
	
	//fireworkSystem1->update(t);
	//fireworkSystem2->update(t);*/
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	//delete fireworkSystem1;
	//delete fireworkSystem2;
	delete particleSystem;
	delete forceSystem;

	for (Particle* p : listParticles) {
		delete p;
		p = nullptr;
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
		//case 'B': break;
		//case ' ':	break;
	case ' ':
	{
		break;
	}
	case 'G':
	{
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