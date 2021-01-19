#pragma once
#include "RenderUtils.hpp"
class RigidBody {
public:
	float lifeTime;
	RenderItem* rItem;
	physx::PxRigidDynamic* body;

	RigidBody():lifeTime(0.0), rItem(nullptr),body(nullptr){}
	virtual ~RigidBody() { rItem->release(); body->release(); }
};

class StaticRigidBody {
public:
	float lifeTime;
	RenderItem* rItem;
	physx::PxRigidStatic* body;

	StaticRigidBody():lifeTime(0.0), rItem(nullptr), body(nullptr) {}
	virtual ~StaticRigidBody(){ rItem->release(); body->release(); }
};