#include "Bullet.h"
#include "config.h"
#include "chrono"
#include <iostream>
#include "Enemy.h"
#include "Potato.h"
#include "Log.h"
#include "PlayerBullet.h"

using namespace std::chrono;

Bullet::Bullet() {
	born = steady_clock::now();
}

void Bullet::update()
{
	this->position += this->velocity * Config::DELTA_TIME;

	if (duration_cast<seconds>(steady_clock::now() - born).count() >= Config::BULLET_LIFETIME) {
		Destroy();
	}
}

bool Bullet::isActiveCollider()
{
	return true;
}

void Bullet::onOverlap(Graphics* other)
{
	if (!other)return;
	if (dynamic_cast<Player*>(other)) {
		this->Destroy();
	}
}
