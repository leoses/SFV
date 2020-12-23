#pragma once
#include "PxPhysicsAPI.h"
#include "RenderUtils.hpp"
#include <vector>
#include "checkML.h"

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
    float damping = 0;
    //INVERSO DE LA MASA
    float inverse_mass = 0.3;

    //RENDERITEM
    //REMEMBER: No hace falta en principio destruirlo pq cuando pierde todas sus referencias se
    //libera a si mismo
    RenderItem* rItem = nullptr;
public:
    Particle(physx::PxTransform transform):
        position_(0, 0, 0), 
        velocity_(0, 0, 0), 
        acceleration_(0, 0, 0) {
        PxSphereGeometry* geo = new PxSphereGeometry(5);
        rItem = new RenderItem(CreateShape(*geo), &transform, Vector4(255, 0, 0, 1));
        delete geo;
    }
    ~Particle();
    void integrate(float t);
};

