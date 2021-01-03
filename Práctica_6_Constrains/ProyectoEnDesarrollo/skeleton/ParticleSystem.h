#pragma once
#include "Particle.h"
#include <list>
#include <cstdlib>
#include <ctime>
#include "ParticleForceRegistry.h"

using namespace std;

extern void createParticle(Vector3 pos, Vector3 vel);

class ParticleSystem
{
private:

	//Variables para el manejo de la creacion de partículas
	float timeNextParticle = 0;
	float timeCreateParticle;

	//Posición desde la que se van a crear las partículas
	Vector3 posSystem;


public:
	//Constructora
	ParticleSystem(Vector3 posSystem_,float timeBetweenParticles = 0.002);
	//Destructora
	~ParticleSystem() {};

	//Método para actualizar el sistema
	void update(float t);

protected:
	//Metodo que maneja la creacion de particulas
	void addParticles(float t);


};

