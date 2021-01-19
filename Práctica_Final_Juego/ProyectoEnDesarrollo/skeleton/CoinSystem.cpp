#include "CoinSystem.h"
#include "Level.h"

CoinSystem::CoinSystem(float playerSize)
{
	playerSize_ = playerSize;
	PxGeometry* pointGeo = new PxSphereGeometry(3);
	PxShape* shape = CreateShape(*pointGeo);

	for (int i = 0; i < MAX_MONEDAS; i++) {
		Particle* p = new Particle(COIN_DIAMETER);
		p->setColor(Vector4(1.0, 1.0, 0, 1.0));

		monedas.push_back( p );
	}

	delete pointGeo;
	shape->release();
}

CoinSystem::~CoinSystem()
{
	for (Particle* c : monedas) {
		delete c;
	}
}

void CoinSystem::inizializeCoins(vector<LevelElement>& levelInfo, int coinsPerFloor)
{
	for (int i = 1; i < levelInfo.size(); i++) {

		Vector3 c = levelInfo.at(i).centerPosition_;
		for (int j = 0; j < coinsPerFloor && coinsPerFloor*i + j < MAX_MONEDAS; j++) {

			float x = -30 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (30 - (-30))));
			float z = c.z - 170 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (170 - (-170))));

			monedas.at(coinsPerFloor * i + j)->setPosition(Vector3(x, c.y + COIN_DIAMETER, z));
			if (!monedas.at(coinsPerFloor * i + j)->isActive()) {
				monedas.at(coinsPerFloor * i + j)->activateParticle();
				nCoins++;
			}
		}
	}
}

void CoinSystem::updateSystem(float t, RigidBody* player)
{
	for (Particle* r : monedas) {
		if ( !r->isActive())continue;

		float distance = Vector3(player->body->getGlobalPose().p - r->getPosition()).magnitude();

		if (distance < COIN_DIAMETER + playerSize_) {
			r->desactivateParticle();
			//Metodo del player que suma 1 al contador de puntos
			static_cast<Player*>(player)->addPlayerPoint();
		}
	}
}

void CoinSystem::restartCoins()
{
	for (int i = 0; i < nCoins; i++) {
		if (!monedas.at(i)->isActive()) monedas.at(i)->activateParticle();
	}
}
