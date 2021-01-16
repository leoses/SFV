#pragma once
#include "RigidBody.h"
#include "checkML.h"
enum horizontalMovement{Left, Right};

using namespace physx;

constexpr float MIN_CAMERA_CENTER_OFFSET = 45;

extern RigidBody* createRigidDynamic(const Vector3& t, PxShape* shape, const Vector3& speed, const Vector4& color);

class Player: public RigidBody
{
private:
	float currCameraPlayerOffset;
	const Vector3 iniPos = Vector3(0, 1, -75);

public:
	Player(PxShape* shape);
	virtual ~Player();

	void resetForces();
	float getCurrCameraPlayerOffset()noexcept;
	void setCurrCameraPlayerOffset(const float newOffset) noexcept;
	void changeTrack(horizontalMovement dir);

	const Vector3 getPlayerIniPos() { return iniPos; }
	void playerLose();
	//RigidBody* getRigidBody()noexcept { return player_; }
	//physx::PxRigidDynamic* getBody() noexcept { return player_->body; }
	
};

