#pragma once
#include "RigidBody.h"
#include "checkML.h"
enum horizontalMovement{Left, Right};

using namespace physx;

constexpr float MIN_CAMERA_CENTER_OFFSET = 45;

extern PxScene* getScene() noexcept;
extern PxPhysics* getPhysics()noexcept;

class Player: public RigidBody
{
private:
#pragma region Constantes
	//Posicion inicial en la que se crea el jugador
	const Vector3 iniPos = Vector3(0, 1, -75);

	//Velocidades maximas/minimas alcanzables en z cuando entramos en zonas de vientos
	const float MAX_Z_SPEED = 80.0;
	const float MIN_Z_SPEED = 30.0;

	//Velocidad máxima alcanzable por el jugador sin pasar por ningún viento
	const float MAX_COMMON_SPEED = 60.0;
#pragma endregion
	
	bool isUnmovable = false;
	bool isFalling = false;
	Vector3 vel = Vector3(0, 0, MAX_COMMON_SPEED);

public:
	Player(PxShape* shape);
	virtual ~Player();

	void update(float t);

	void resetForces();
	void changeTrack(horizontalMovement dir);
	void reset();

	const Vector3 getPlayerLinealVelocity() { return body->getLinearVelocity(); }
	bool getMovable() noexcept { return !isUnmovable; }
	const Vector3 getVelocity() { return vel; }
	const Vector3 getPlayerIniPos() { return iniPos; }
	const float getMaxZVel() noexcept { return MAX_Z_SPEED; }
	const float getMinZVel() noexcept { return MIN_Z_SPEED; }
	bool isPlayerFalling() noexcept { return isFalling; }

	void constrainMovement(bool constrain);
	void setVelocity(const Vector3 newDir) { vel = newDir; }
	void setPlayerFalling() noexcept { isFalling = true; }
};

