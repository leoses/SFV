#include "Player.h"

Player::Player(): RigidBody()
{
	//Creacion y configuración de la parte fisico/visual del player
	PxGeometry* geo = new PxSphereGeometry(SIZE);
	PxShape* shape = CreateShape(*geo);
	body = getPhysics()->createRigidDynamic(PxTransform(iniPos));
	body->attachShape(*shape);
	rItem = new RenderItem(shape,body, Vector4(1.0, 0, 0, 1.0));
	getScene()->addActor(*body);
	PxRigidBodyExt::updateMassAndInertia(*body, PxReal(2));
	body->setName("Player");

	//Creamos el sistema de particulas que gestiona el rastro que
	//deja el player
	trail = new ParticleSystem(0.008);

	//Eliminamos recursos no necesarios
	shape->release();
	delete geo;
}

Player::~Player()
{
	delete trail;
}

void Player::update(float t)
{
	//Para que no rebote al caer contra el suelo
	if (isFalling)body->setLinearVelocity(Vector3(0, 0, 0)); 
	//Impulso que recibe el player al colisionar con una pared
	if (isWallColliding) {
		if (body->getGlobalPose().p.x > 0) vel.x = -WALL_IMPLUSE_X;
		else vel.x = WALL_IMPLUSE_X;
	}

	//Ajustes para rebotar al chocar con las paredes volver a la velocidad en x estandar (0)
	if (vel.x > 0.1) vel.x -= X_VARIATION;
	else if (vel.x < -0.1) vel.x += X_VARIATION;


	//Modificación de la velocidad->si vamos por debajo de la normal aumentamos,
	//Si no disminuimos -> aceleramos mas rapido que deceleramos para que sea mas 
	//Sencillo
	if (vel.z < MAX_COMMON_SPEED) vel.z += 0.05;
	else vel.z -= 0.01;

	//Calculamos y actualizamos posicion
	Vector3 newPos = body->getGlobalPose().p + vel*t;
	body->setGlobalPose(PxTransform(newPos));
	
	isFalling = false;
	isWallColliding = false;

	//Configuramos la nueva posición desde la que se crean las particulas de la cola
	newPos.z -= 5;
	trail->update(t, newPos);
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
		break;  
	}
	case Right: {
		PxTransform t = body->getGlobalPose();
		t.p.x += 1;
		body->setGlobalPose(t);
		break; 
	}
	default:
		break;
	}
}

void Player::reset()
{
	vel = PxVec3(0, 0, MAX_COMMON_SPEED);
	body->setGlobalPose(PxTransform(iniPos));
	playerPoints = 0;
	body->putToSleep();
}
