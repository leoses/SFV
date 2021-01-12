#pragma once
#include "RigidBody.h"
enum horizontalMovement{Left, Right};

using namespace physx;

extern RigidBody* createRigidDynamic(const Vector3& t, PxShape* shape, const Vector3& speed, const Vector4& color);

class Player
{
private:
	RigidBody* player_ = nullptr;

public:
	Player();
	~Player();

	void changeTrack(horizontalMovement dir);

	physx::PxRigidDynamic* getBody() { return player_->body; }
	
};

