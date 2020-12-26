#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include "checkML.h"
#include "RigidBodiesSystem.h"
#include "RigidBodyFRegistry.h"
#include "RigidBody.h"

using namespace std;
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

//PRÁCTICA SOLIDOS-RIGIDOS
PxRigidStatic* suelo_ = nullptr;
PxTransform sueloTransform_(0, -25, 0);
RenderItem* sueloRender_ = nullptr;

RigidBodiesSystem* rbSystem = nullptr;
rbExplosion* explosion_ = nullptr;
RigidBodyFRegistry* forceSystem = nullptr;

RigidBody* createRigidDynamic(Vector3 t, PxShape* shape, Vector3 speed) {
	RigidBody* r = new RigidBody();
	r->body = gPhysics->createRigidDynamic(PxTransform(t));
	r->body->attachShape(*shape);
	r->rItem = new RenderItem(shape, r->body, Vector4(0, .5, 1.0, 1.0));

	gScene->addActor(*r->body);
	PxRigidBodyExt::updateMassAndInertia(*r->body, PxReal(2));
	r->body->addForce(speed * r->body->getMass());

	if (forceSystem != nullptr)forceSystem->add(r, explosion_);

	return r;
}

StaticRigidBody* createRigidStatic(Vector3 t, PxShape* shape) {
	StaticRigidBody* r = new StaticRigidBody();
	r->body = gPhysics->createRigidStatic(PxTransform(t));
	r->body->attachShape(*shape);
	r->rItem = new RenderItem(shape, r->body, Vector4(0, .5, 1.0, 1.0));

	gScene->addActor(*r->body);

	return r;
}

#pragma region Funciones_Auxiliares
void addRigidBodyToForceSystem(RigidBody* rb, rbForceGenerator* g) {
	if (forceSystem != nullptr)forceSystem->add(rb, g);
}

void removeRigidBodyFromForceSystem(RigidBody* rb) {
	if (forceSystem != nullptr)forceSystem->remove(rb);
}
#pragma endregion

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
	//GRAVEDAD SIEMPRE ANTES DE gScene = gPhysics->createScene(sceneDesc)
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;

	gScene = gPhysics->createScene(sceneDesc);
	// ------------------------------------------------------

	suelo_ = gPhysics->createRigidStatic(sueloTransform_);
	PxGeometry* geo = new PxBoxGeometry(200, 5, 200);
	PxShape* shape = CreateShape(*geo);
	delete geo;
	suelo_->attachShape(*shape);
	sueloRender_ = new RenderItem(shape,suelo_, Vector4(1.0, .0, .0, 1.0));

	gScene->addActor(*suelo_);

	rbSystem = new RigidBodiesSystem(Vector3(0, 50, 0),2);
	explosion_ = new rbExplosion(1500, Vector3(0, 50, 0), 25);
	forceSystem = new RigidBodyFRegistry();
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
	rbSystem->updateSystem(t);

}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	delete forceSystem;
	sueloRender_->release();
	delete rbSystem;
	delete explosion_;

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
	case ' ':
	{
		if (!explosion_->isActive()) {
			explosion_->activateExplosion(75 * GetCamera()->getDir() + GetCamera()->getEye()); //
		}
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