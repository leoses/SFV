#pragma once
#include "PxPhysicsAPI.h"
#include "RenderUtils.hpp"
#include <vector>
#include "checkML.h" //Basura

using namespace physx;

class Particle 
{
private:
    //VECTORES
    PxVec3 position_;
    PxVec3 velocity_;
    PxVec3 acceleration_;

    //FLOATS
    //FACTOR DE ROZAMIENTO
    float damping;
    //INVERSO DE LA MASA
    float inverse_mass;

    //RENDERITEM
    RenderItem* rItem = nullptr;
    //Transform auxiliar para mover la partícula
    PxTransform trans;
    //Geometría de la partícula
    PxSphereGeometry* geo;

    //TIEMPO DE VIDA DE LA PARTÍCULA
    const double LIFE_TIME = 5;
    double time_ = 0;

    //Booleano para determinar si la partícula esta activa
    //Por defecto se crean siempre desactivadas
    bool isActive_ = false;


public:
    Particle(PxVec3 pos,float damp = 0.5, float iMass = 0.3):
        position_(pos), 
        velocity_(0,0,0), 
        acceleration_(0, 0, 0),
        damping(damp),
        inverse_mass(iMass)
    {
        geo = new PxSphereGeometry(5);
    }
    ~Particle();

    void integrate(float t);
    void move(float t);
    void addTime(double t) noexcept;
    void resetTime() noexcept;
    void activateParticle();
    void desactivateParticle();

#pragma region Getters
    const PxVec3 getVelocity(){ return velocity_; }
    const PxVec3 getPosition(){ return position_; }
    const PxVec3 getAcceleration() { return acceleration_; }
    const double isDead() noexcept { return time_ > LIFE_TIME; }
    const bool isActive()noexcept { return isActive_; }
#pragma endregion

#pragma region Setters
    inline void setPosition(const PxVec3& newPos) { position_ = newPos; }
    inline void setVelocity(const PxVec3& newVel) { velocity_ = newVel; }
    inline void setAcceleration(const PxVec3& newAcc) { acceleration_ = newAcc; }
    inline void setActive (bool active) noexcept{ isActive_ = active; }
#pragma endregion


};

