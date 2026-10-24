#pragma once
#include "WeaponUI.h"
#include "Player.h"
class WeaponPair
{
	WeaponUI left;
	WeaponUI right;
	Event<Player::ShootSide>::Id shootSubscription = 0;

	void onPlayerShoot(Player::ShootSide side);

public:
	WeaponPair();
	~WeaponPair();

	void draw();
};
