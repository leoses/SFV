#pragma once
#include "Particle.h"
#include <list>
#include <cstdlib>
#include <ctime>

using namespace std;

class ParticleSystem
{
private:

	//NUMERO MÁXIMO DE PARTICULAS
	const int MAX_PARTICLES = 100;

	//Variables para el manejo de la creacion de partículas
	float timeNextParticle = 0;
	float timeCreateParticle;

	//lista con las partículas creadas
	list<Particle*> listParticles;

	//Posición desde la que se van a crear las partículas
	Vector3 posSystem;

public:
	//Constructora
	ParticleSystem(Vector3 posSystem_, float timeBetweenParticles = 0.3);
	//Destructora
	~ParticleSystem();

	//Método para actualizar el sistema
	void update(float t);

protected:
	//Metodo que maneja el movimiento y la destruccion de las particulas
	void updateParticles(float t);
	//Metodo que maneja la creacion de particulas
	void addParticles(float t);

};

