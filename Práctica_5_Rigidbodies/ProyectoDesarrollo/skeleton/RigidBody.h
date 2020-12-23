#pragma once
#include "RenderUtils.hpp"
struct RigidBody {
	float lifeTime;
	RenderItem* rItem;
	physx::PxRigidDynamic* body;

	RigidBody():lifeTime(0.0), rItem(nullptr),body(nullptr){}
};