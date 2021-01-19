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

public:
	//Constructora
	ParticleSystem(float timeBetweenParticles = 0.002);
	//Destructora
	~ParticleSystem() {};

	//Método para actualizar el sistema
	void update(float t, Vector3 pos);

protected:
	//Metodo que maneja la creacion de particulas
	void addParticles(float t, Vector3 pos);


};

