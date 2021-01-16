#include "Player.h"

Player::Player(PxShape* shape): RigidBody()
{    
	/*RigidBody* rb = createRigidDynamic(iniPos, shape, Vector3(0, 0, 0), Vector4(1.0, 0, 0, 1.0));

	body = rb->body;
	rItem = rb->rItem;*/

	body = getPhysics()->createRigidDynamic(PxTransform(iniPos));
	body->attachShape(*shape);
	rItem = new RenderItem(shape,body, Vector4(1.0, 0, 0, 1.0));

	getScene()->addActor(*body);
	PxRigidBodyExt::updateMassAndInertia(*body, PxReal(2));
	
	body->setLinearDamping(PxReal(0));
	body->setLinearVelocity(PxVec3(0, 0, 50));
	shape->release();
	currCameraPlayerOffset = 50;
}

Player::~Player()
{
}

void Player::resetForces()
{
	body->clearForce(physx::PxForceMode::eIMPULSE);
}

float Player::getCurrCameraPlayerOffset() noexcept
{
	return currCameraPlayerOffset;
}

void Player::setCurrCameraPlayerOffset(const float newOffset) noexcept
{
	if (newOffset > MIN_CAMERA_CENTER_OFFSET)currCameraPlayerOffset = newOffset;
}

void Player::changeTrack(horizontalMovement dir)
{
	switch (dir)
	{
	case Left: {
		body->setLinearVelocity(PxVec3(-15, 0, 50));
		break;  
	}
	case Right: {
		body->setLinearVelocity(PxVec3(15, 0, 50));
		break; 
	}
	default:
		break;
	}
}

void Player::playerLose()
{
	body->clearForce(PxForceMode::eACCELERATION);
	body->clearForce(PxForceMode::eVELOCITY_CHANGE);
	currCameraPlayerOffset = 50;
	body->setLinearVelocity(PxVec3(0, 0, 50));
	body->setGlobalPose(PxTransform(iniPos));
}
