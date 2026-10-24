#pragma once
#include "Sprite.h"
#include "chrono"
#include "Player.h"

using namespace std::chrono;

class WeaponUI
{
	const float kickTime = 0.05;
	const float releaseTime = 0.2;

	Sprite* sprite;
	steady_clock::time_point lastShoot;

public:
	sf::Vector2f position;
	sf::Vector2f kickPosition;

	WeaponUI();
	~WeaponUI();

	void draw();

	void shoot();

	void flipHorizontal();
};

