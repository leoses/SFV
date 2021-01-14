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
	//Así o usando impulsos??
	//Cambiar luego
	Vector3 pos = player_->body->getGlobalPose().p;
	switch (dir)
	{
	case Left: pos.x -= 0.5;
		break; 
	case Right:pos.x += 0.5;
		break;
	default:
		break;
	}
	physx::PxTransform trans(pos);
	player_->body->setGlobalPose(trans);

}
