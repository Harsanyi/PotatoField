#include "WeaponUI.h"
#include "Game.h"
#include "chrono"
#include "Utils.h"
#include "Player.h"
#include <Log.h>

using namespace std::chrono;

WeaponUI::WeaponUI()
{
	sprite = new Sprite("img/weapon.png");
	sprite->setOriginNormal(sf::Vector2f(0,1));
	sprite->setRotation(3);
	sprite->setScale({0.7,0.7});

	lastShoot = steady_clock::now();
	lastShoot -= seconds((int)(kickTime + releaseTime + 1));
}

WeaponUI::~WeaponUI()
{
	delete(sprite);
}

void WeaponUI::draw()
{
	sf::Vector2f pos;
	float elapsed = duration_cast<milliseconds>(steady_clock::now()-lastShoot).count()/1000.;
	if (elapsed >= (kickTime + releaseTime)) {
		pos = position;
	}
	else if (elapsed <= kickTime) {
		pos = lerp(position, kickPosition, smoothstep(elapsed / kickTime));
	}
	else {
		pos = lerp(kickPosition, position, smoothstep((elapsed-kickTime) / releaseTime));
	}

	sprite->setPosition(pos);
	Game::window->draw(*sprite);
}

void WeaponUI::shoot()
{
	lastShoot = steady_clock::now();
}

void WeaponUI::flipHorizontal()
{
	sf::Vector2f scale = sprite->getScale();
	scale.x *= -1;
	sprite->setScale(scale);
	sprite->setRotation(sprite->getRotation()*-1.);
}
