#pragma once
#include "PxPhysicsAPI.h"
#include "RenderUtils.hpp"
#include <vector>
#include "checkML.h" //Basura

using namespace physx;

class Particle 
{
protected:
    //VECTORES
    PxVec3 position_;
    PxVec3 velocity_;
    PxVec3 acceleration_;
    PxVec3 force;

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
    //Color de la partícula
    Vector4 color_;

    //TIEMPO DE VIDA DE LA PARTÍCULA
    float LIFE_TIME = 0;
    float time_ = 0;

    //Booleano para determinar si la partícula esta activa
    //Por defecto se crean siempre desactivadas
    bool isActive_ = false;


public:
    Particle(PxVec3 pos = Vector3(0,0,0),float damp = 0.5, float iMass = 0.3, Vector4 color = Vector4(255, 0, 0, 1)):
        position_(pos), 
        velocity_(0,0,0), 
        acceleration_(0, 0, 0),
        force(0,0,0),
        damping(damp),
        inverse_mass(iMass),
        color_(color)
    {
        geo = new PxSphereGeometry(5);
    }
    ~Particle();

    void integrate(float t);
    void addTime(double t) noexcept;
    void resetTime() noexcept;
    void activateParticle();
    void desactivateParticle();
    void clearForce(){ force = PxVec3(0.0, 0.0, 0.0); };


#pragma region Getters
    const PxVec3 getVelocity(){ return velocity_; }
    const PxVec3 getPosition(){ return position_; }
    const PxVec3 getAcceleration() { return acceleration_; }
    const double isDead() noexcept { return time_ > LIFE_TIME; }
    const bool isActive()noexcept { return isActive_; }
    const Vector4 getColor() { return color_; }
    const float getInverseMass() noexcept { return inverse_mass; }
    const float getMass()noexcept { return 1 / inverse_mass; }
#pragma endregion

#pragma region Setters
    inline void setPosition(const PxVec3& newPos) { position_ = newPos; }
    inline void setVelocity(const PxVec3& newVel) { velocity_ = newVel; }
    inline void setAcceleration(const PxVec3& newAcc) { acceleration_ = newAcc; }
    inline void setActive (bool active) noexcept{ isActive_ = active; }
    inline void setMass(float newmass)noexcept { inverse_mass = 1 / newmass;}
    inline void setDamping(float damp)noexcept { damping = damp; }
    inline void setLifeTime(float lifeTime) noexcept { LIFE_TIME = lifeTime; }
    inline void setColor(const Vector4& newColor) { color_ = newColor; }
    inline void addForce(const PxVec3& f) { force += f; };
#pragma endregion


};

