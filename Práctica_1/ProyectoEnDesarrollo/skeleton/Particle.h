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

    //TIEMPO DE VIDA DE LA PARTÍCULA
    const double LIFE_TIME = 2;
    double time_ = 0;

    //BOOLEANO PARA DETERMINAR SI LA BALA ESTÁ SIENDO USADA
    bool used_ = false;

public:
    Particle(PxVec3 pos,float damp = 0, float iMass = 0.3):
        position_(pos), 
        velocity_(0,0,0), 
        acceleration_(0, 0, 0),
        damping(damp),
        inverse_mass(iMass)
    {
        PxSphereGeometry* geo = new PxSphereGeometry(5);
        rItem = new RenderItem(CreateShape(*geo), &PxTransform(position_), Vector4(255, 0, 0, 1));
        delete geo;
    }
    ~Particle();
    void integrate(float t);
    void move(float t);
    void addTime(double t) noexcept;
    void resetTime() noexcept;

#pragma region Getters
    const PxVec3 getVelocity(){ return velocity_; }
    const PxVec3 getPosition(){ return position_; }
    const PxVec3 getAcceleration() { return acceleration_; }
    const double isDead() noexcept { return time_ > LIFE_TIME; }
    const bool getParticleUsed() noexcept { return used_; }
#pragma endregion

#pragma region Setters
    inline void setPosition(const PxVec3& newPos) { position_ = newPos; }
    inline void setVelocity(const PxVec3& newVel) { velocity_ = newVel; }
    inline void setAcceleration(const PxVec3& newAcc) { acceleration_ = newAcc; }
    inline void setParticleUsed(bool used) noexcept { used_ = used; }
#pragma endregion


};

