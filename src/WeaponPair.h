#pragma once
#include "WeaponUI.h"
class WeaponPair
{
	WeaponUI left;
	WeaponUI right;

	void onPlayerShoot(Player::ShootSide side);

public:
	WeaponPair();

	void draw();
};

