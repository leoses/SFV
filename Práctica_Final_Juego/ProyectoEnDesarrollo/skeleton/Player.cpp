#include "Player.h"

Player::Player(PxShape* shape): RigidBody()
{    
	body = getPhysics()->createRigidDynamic(PxTransform(iniPos));
	body->attachShape(*shape);
	rItem = new RenderItem(shape,body, Vector4(1.0, 0, 0, 1.0));

	getScene()->addActor(*body);
	PxRigidBodyExt::updateMassAndInertia(*body, PxReal(2));
	
	shape->release();

	body->setName("Player");
}

Player::~Player()
{
}

void Player::update(float t)
{
	//Para que no rebote al caer contra el suelo
	if (isFalling)body->setLinearVelocity(Vector3(0, 0, 0)); 
	if (vel.z < 60) vel.z += 0.05;

	Vector3 newPos = body->getGlobalPose().p + vel*t;

	body->setGlobalPose(PxTransform(newPos));
	isFalling = false;
}

void Player::resetForces()
{
	body->clearForce(physx::PxForceMode::eIMPULSE);
	body->clearForce(PxForceMode::eACCELERATION);
	body->clearForce(PxForceMode::eVELOCITY_CHANGE);
}

void Player::constrainMovement(bool constrain)
{
	isUnmovable = constrain;
	body->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, constrain);
}


void Player::changeTrack(horizontalMovement dir)
{
	switch (dir)
	{
	case Left: {
		PxTransform t = body->getGlobalPose();
		t.p.x -= 1;
		body->setGlobalPose(t);
		//body->setLinearVelocity(Vector3(-15, 0, 60));
		break;  
	}
	case Right: {
		PxTransform t = body->getGlobalPose();
		t.p.x += 1;
		body->setGlobalPose(t);
		//body->setLinearVelocity(Vector3(15, 0, 60));
		break; 
	}
	default:
		break;
	}
}

void Player::reset()
{
	resetForces();
	vel = PxVec3(0, 0, 60);
	body->setGlobalPose(PxTransform(iniPos));
}
