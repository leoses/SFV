#include "FireworkSystem.h"

FireworkSystem::FireworkSystem(Vector3 initialSystemPos)
{
	initFireworkRules();
	posSystem_ = initialSystemPos;

	for (int i = 0; i < MAX_FIREWORKS; i++) {
		fireworkElements.push_back(new Firework());
	}

	//inicializar generador randoms
	srand(time(NULL));

	//Creamos el primer fuego artificial
	createFirework(0);

	

}

FireworkSystem::~FireworkSystem()
{
	for (Particle* p : fireworkElements) {
		delete p;
		p = nullptr;
	}
}

Firework* FireworkSystem::findInactiveFirework()
{
	auto it = fireworkElements.begin();
	while ( it != fireworkElements.end()) {
		if (!(*it)->isActive()) return *it; //El primero que encontremos que no esté activo es el que devolvemos
		++it;
	}
	return nullptr;
}

void FireworkSystem::update(float t)
{
	//Actualizacion de posiciones
	for (Firework* p : fireworkElements) {
		if (!p->isActive())continue;

		//Si el firework ha vivido el tiempo suficiente
		//"Eliminamos" el padre y según el payload que toque, creamos tantos elementos queramos
		if (p->isDead()) {
			FireworkRule* rule = getFireworkRule(p->getType());

			for (Payload payloads : rule->payloads) {
				for (int i = 0; i < payloads.count; i++) {
					createFirework(payloads.type, p);
				}
			}

			p->desactivateParticle();
		}
		else {
			p->integrate(t);
			p->addTime(t);
		}
	}
}

void FireworkSystem::initFireworkRules()
{
	rules.push_back({ //at(0)
			0, // type
			1.5f, 2.4f, // age range
			Vector3(-15, 30, -15), Vector3(15, 48, 15), // velocity range
			0.1, // damping
			Vector4(0,255, 0, 1),//color
	});
	rules.at(0).payloads.push_back(
		{ 1, 5 }); //5 elementos del tipo 3

	rules.push_back({ //at(1)
			1, // type
			1.0f, 2.0f, // age range
			Vector3(-40,60,-40), Vector3(40,80,40), // velocity range
			0.1, // damping
			Vector4(0,0, 255, 1),//color
    }); 
	rules.at(1).payloads.push_back(
		{ 2, 3 }); //3 elementos del tipo 2

	rules.push_back({ //at(2)
			2, // type
			1.0f, 2.0f, // age range
			Vector3(-40,60,-40), Vector3(40,80,40), // velocity range
			0.1, // damping
			Vector4(255,0, 0, 1),//color
	});
}

void FireworkSystem::createFirework(unsigned type, Firework* parent)
{
	FireworkRule* rule = getFireworkRule(type);
	Firework* newFirework = findInactiveFirework();

	if(newFirework != nullptr){
		rule->create(newFirework, parent); //Si hemos encontrado un firework desactivado, creamos el nuevo
		newFirework->activateParticle();
	}
}
