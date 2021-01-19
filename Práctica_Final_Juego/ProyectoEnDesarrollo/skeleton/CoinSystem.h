#pragma once
#include "Particle.h"
#include "Player.h"

class Level;
class LevelElement;

using namespace physx;
using namespace std;

constexpr float COIN_DIAMETER = 5;
constexpr int MAX_MONEDAS = 200;

class CoinSystem
{
public:
	CoinSystem(float playerSize);
	~CoinSystem();

	void inizializeCoins(vector<LevelElement> & levelInfo, int coinsPerFloor);
	void updateSystem(float t, RigidBody* player);
	void restartCoins();

private:
	vector<Particle*> monedas;
	int nCoins = 0;
	float playerSize_ = 0;
};

