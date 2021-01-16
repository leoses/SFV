#pragma once
#include "RenderUtils.hpp"
class RigidBody {
public:
	float lifeTime;
	RenderItem* rItem;
	physx::PxRigidDynamic* body;

	RigidBody():lifeTime(0.0), rItem(nullptr),body(nullptr){}
	~RigidBody() { rItem->release(); body->release(); }
};

class StaticRigidBody {
public:
	float lifeTime;
	RenderItem* rItem;
	physx::PxRigidStatic* body;

	StaticRigidBody():lifeTime(0.0), rItem(nullptr), body(nullptr) {}
	~StaticRigidBody(){ rItem->release(); body->release(); }
};