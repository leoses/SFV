#include "RigidBodyFRegistry.h"
#include "Player.h"

rbInsideASphere::rbInsideASphere(Vector3 center, float radius)
{
	center_ = center;
	radius_ = radius;
	geo = new PxSphereGeometry(radius);
}

bool rbInsideASphere::rbInsideVolume(Vector3 position)
{
	float distance = Vector3(position - center_).magnitude();

	if (distance < radius_) return true;
	else return false;
}

void rbInsideASphere::createVolume( const Vector4 color)
{
	auto shape = CreateShape(*geo);
	t = PxTransform(center_);
	volume = new RenderItem(shape, &t, color);
	volume->transform = &t;
	shape->release();
}

rbExplosion::rbExplosion(int forceModifier, Vector3 center, float radius) :
	rbInsideASphere(center, radius), forceModifier_(forceModifier) {}

void rbExplosion::updateForce(RigidBody* rb, float t)
{
	if (!active_ || !rbInsideVolume(rb->body->getGlobalPose().p))return;

	Vector3 dir = Vector3(center_ - rb->body->getGlobalPose().p);
	float distance = dir.magnitude();

	Vector3 acceleration = ((radius_ - distance) * (-dir.getNormalized()) * forceModifier_);

	rb->body->addForce(acceleration);
}

void rbExplosion::updateLifeTime(float t)
{
	if (!isActive())return;

	time += t;
	if (time >= LIFETIME) {
		active_ = false;
		time = 0.0;
		volume->release();
		volume = nullptr;
	}
}

void rbExplosion::activateExplosion(const Vector3& newCenter)
{
	active_ = true;
	center_ = newCenter;
	createVolume();
}


RigidBodyFRegistry::~RigidBodyFRegistry()
{
	registrations.clear();
}

void RigidBodyFRegistry::add(RigidBody* particle, rbForceGenerator* fg)
{
	registrations.push_back({ particle, fg });
}

void RigidBodyFRegistry::remove(RigidBody* particle, rbForceGenerator* fg)
{
	auto it = registrations.begin();
	bool encontrado = false;

	while (it != registrations.end() && !encontrado) {
		if ((*it).particle == particle && (*it).fg == fg) {//Hemos encontrado el elemento a borrar
			it = registrations.erase(it);//borramos 
			encontrado = true;//hemos terminado la busqueda
		}
		else {
			++it;
		}
	}

}

void RigidBodyFRegistry::remove(RigidBody* particle)
{
	auto it = registrations.begin();

	while (it != registrations.end()) {
		if ((*it).particle == particle) {//Hemos encontrado el elemento a borrar
			it = registrations.erase(it);//borramos 
		}
		else ++it;
	}
}

void RigidBodyFRegistry::clear()noexcept
{
	registrations.clear();
}

void RigidBodyFRegistry::updateForces(float t)
{
	for (RbForceRegistration p : registrations) {
		p.fg->updateForce(p.particle, t);
	}
}

RigidBodyWind::RigidBodyWind(WindType type, Vector3 pos, float radius) :
	rbInsideASphere(pos, radius), type_(type){

	switch (type_)
	{
	case Forward:
		createVolume(Vector4(0.0,0.0,255,0.0));
		windDirection_ = Vector3(0, 0, 50);
		break;
	case Backward:
		createVolume(Vector4(255, 0.0, 0.0, 0.0));
		windDirection_ = Vector3(0, 0, -30);
		break;
	case Upwards:
		createVolume(Vector4(0.0, 255, 0.0, 0.0));
		windDirection_ = Vector3(0, 20, 0);
		break;
	default:
		break;
	}
	
}

void RigidBodyWind::updateForce(RigidBody* rb, float t) 
{
	if (!rbInsideVolume(rb->body->getGlobalPose().p))return;

	rb->body->addForce(windDirection_,physx::PxForceMode::eIMPULSE);

	if (type_ == WindType::Upwards)return;

	Player* p = static_cast<Player *>(rb);
	float off = p->getCurrCameraPlayerOffset();
	if (off > MIN_CAMERA_CENTER_OFFSET && type_ == WindType::Forward) {
		p->setCurrCameraPlayerOffset(off - 0.01);
	}
	else if (type_ == WindType::Backward) {
		p->setCurrCameraPlayerOffset(off + 0.005);
	}
	
}
