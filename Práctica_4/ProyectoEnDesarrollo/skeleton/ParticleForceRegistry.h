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

#pragma region Practica_fuerzas
//Clase para el manejo de la gravedad de las particulas
class ParticleGravity : public ParticleForceGenerator
{
private:
	// Acceleration for gravity
	Vector3 g;
public:
	ParticleGravity(const Vector3& gravity) : g(gravity) {}
	virtual ~ParticleGravity() {}
	virtual void updateForce(Particle* particle, float t)override;
};

class ParticleForceInsideASphere : public ParticleForceGenerator {
protected:
	//RenderItem
	PxTransform t;
	RenderItem* volume = nullptr;
	PxSphereGeometry* geo = nullptr;

	//Elementos que determinan el volumen de la esfera
	Vector3 center_;
	float radius_;

public:
	ParticleForceInsideASphere(Vector3 center = Vector3(0, 0, 0), float radius = 10);
	virtual ~ParticleForceInsideASphere() { if (volume != nullptr) volume->release(); delete geo; }

	bool particleInsideVolume(Vector3 position);
	void createVolume();
};

class ParticleWind : public ParticleForceInsideASphere
{
private:
	//Direccion del viento
	Vector3 windDirection_;

public:
	ParticleWind(Vector3 windDirection, Vector3 center = Vector3(0, 0, 0), float radius = 10);
	virtual ~ParticleWind() {}
	virtual void updateForce(Particle* particle, float t)override;
};

class ParticleExplosion :public ParticleForceInsideASphere {
private:
	const float LIFETIME = 1.0;
	float time = 0.0;
	int forceModifier_ = 1;
	bool active_ = false;

public:
	ParticleExplosion(int forceModifier, Vector3 center = Vector3(0, 0, 0), float radius = 10);
	virtual ~ParticleExplosion() {}

	virtual void updateForce(Particle* particle, float t)override;
	void updateLifeTime(float t);
	void activateExplosion(const Vector3& newCenter);


	//Getters
	const bool isActive() noexcept { return active_; }

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
#pragma endregion

#pragma region Practica_Muelles
class ParticleAnchoredSpring : public ParticleForceGenerator
{
	// Position of the other end of the spring
	Vector3* anchor;
	// Elasticity constant
	float k;
	float restLength;
	RenderItem* boxRenderItem_ = nullptr;
	physx::PxBoxGeometry* box = nullptr;
	PxTransform t;

public:
	ParticleAnchoredSpring(Vector3* _anchor, float _k, float _restLength);
	virtual ~ParticleAnchoredSpring() { boxRenderItem_->release(); delete box;}
	virtual void updateForce(Particle* particle, float t) override;
	void updateSpringConstant(const float increment);
};
#pragma endregion



