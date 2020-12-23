#pragma once
#include "Particle.h"


class Firework : public Particle
{
private:
	// Type of payload it has (what it will generate)
	unsigned int type = 0;

public:
	Firework() noexcept :Particle() {};
	Firework(unsigned int payload, double timeAlive) : Particle() { type = payload; LIFE_TIME = timeAlive; };

	inline void setType(unsigned int newType) noexcept{ type = newType; }

	const int getType() noexcept { return type; }
};





