#pragma once
#include "RigidBody.h"
#include "ParticleSystem.h"
#include <iostream>
enum horizontalMovement{Left, Right};

using namespace physx;

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
	//Distancia minima entre el player y la camara
	const float MIN_CAMERA_CENTER_OFFSET = 45;
	//Radio del player
	const float SIZE = 5;
	//Velocidad a aumentar en x cuando el player colisiona con un muro
	const float WALL_IMPLUSE_X = 15;
	//Modificador de velocidad en x
	const float X_VARIATION = 0.05;
#pragma endregion
	
	//Variable que marca si el jugador se puede mover o no
	//SOLO NO SE MUEVE CUANDO HA GANADO
	bool isUnmovable = false;
	//Variable que marca si el jugador esta cayendo
	bool isFalling = false;
	//Variable que marca si el jugador ha colisionado con una pared
	bool isWallColliding = false;
	//Velocidad 
	Vector3 vel = Vector3(0, 0, MAX_COMMON_SPEED);
	//Puntos iniciales del jugador
	int playerPoints = 0;
	ParticleSystem* trail;

public:
	Player();
	virtual ~Player();

	//Actualizacion del movimiento/estado del player
	void update(float t);
	//Metodo para mover al player hacia los lados
	void changeTrack(horizontalMovement dir);
	//Reseteamos el estado del jugador al inicial
	void reset();

#pragma region Getters
	const Vector3 getPlayerLinealVelocity() { return body->getLinearVelocity(); }
	bool getMovable() noexcept { return !isUnmovable; }
	const Vector3 getVelocity() { return vel; }
	const Vector3 getPlayerIniPos() { return iniPos; }
	const float getMaxZVel() noexcept { return MAX_Z_SPEED; }
	const float getMinZVel() noexcept { return MIN_Z_SPEED; }
	bool isPlayerFalling() noexcept { return isFalling; }
	int getPlayerPoints() noexcept { return playerPoints; }
	float getPlayerSize()noexcept { return SIZE; }
	float getMinCameraOffset()noexcept { return MIN_CAMERA_CENTER_OFFSET; }
#pragma endregion
	
#pragma region Setters
	void constrainMovement(bool constrain);
	void setVelocity(const Vector3 newDir) { vel = newDir; }
	void setPlayerFalling() noexcept { isFalling = true; }
	void setPlayerWallColliding()noexcept { isWallColliding = true; }
	void addPlayerPoint() { playerPoints++; std::cout << playerPoints << "\n"; }
	void resetPlayerPoints()noexcept { playerPoints = 0; }
#pragma endregion

};

