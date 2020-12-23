#pragma once
#include "Particle.h"
#include <list>

//Clase abstracta
class ParticleForceGenerator
{
public:
	// Overload to provide functionality
	virtual void updateForce(Particle* particle, float t) = 0;
};

//Clase para el manejo de la gravedad de las particulas
class ParticleGravity : public ParticleForceGenerator
{
	// Acceleration for gravity
	Vector3 g;
public:
	ParticleGravity(const Vector3& gravity) : g(gravity) {}
	virtual ~ParticleGravity() {}
	virtual void updateForce(Particle* particle, float t)override;
};

class ParticleWind : public ParticleForceGenerator
{
    //Direccion del viento
	Vector3 windDirection_;
	//RenderItem
	PxTransform t ;
	RenderItem* volume = nullptr;
	//Elementos que determinan el volumen de la esfera
	Vector3 center_;
	float radius_;


public:
	ParticleWind(Vector3 windDirection, Vector3 center = Vector3(0, 0, 0), float radius = 10);
	virtual ~ParticleWind() { volume->release(); }
	virtual void updateForce(Particle* particle, float t)override;
	bool particleInsideVolume(Vector3 position);
};


class ParticleForceRegistry
{
protected:
	// Storage for generator-particle entry
	struct ParticleForceRegistration
	{
		Particle* particle;
		ParticleForceGenerator* fg;
	};
	typedef std::list<ParticleForceRegistration> Registry;
	Registry registrations;
public:
	ParticleForceRegistry()noexcept {};
	virtual ~ParticleForceRegistry();

	// Associate generator with a particle
	void add(Particle* particle, ParticleForceGenerator* fg);
	// Remove association
	void remove(Particle* particle, ParticleForceGenerator* fg);
	// Remove association of all ParticleForceGenerator from one particle;
	void remove(Particle* particle);
	// Removes all associations. Particle and Generators won't be deleted
	void clear() noexcept;
	// Update all the generators in the registry
	virtual void updateForces(float t);

};

