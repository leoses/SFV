#include "Player.h"

Player::Player()
{
	PxGeometry* geo = new PxSphereGeometry(5);
	PxShape* shape = CreateShape(*geo);

	player_ = createRigidDynamic(Vector3(0, 1, -75), shape, Vector3(0, 0, 0), Vector4(1.0, 0, 0, 1.0));

	player_->body->setLinearDamping(PxReal(0));
	player_->body->setLinearVelocity(PxVec3(0, 0, 50));
	delete geo;
}

Player::~Player()
{
	delete player_;
}

void Player::changeTrack(horizontalMovement dir)
{
	switch (dir)
	{
	case Left: {
		player_->body->setLinearVelocity(PxVec3(-15, 0, 50));
		break;  
	}
	case Right: {
		player_->body->setLinearVelocity(PxVec3(15, 0, 50));
		break; 
	}
	default:
		break;
	}
}
