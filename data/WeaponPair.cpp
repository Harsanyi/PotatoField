#include "WeaponPair.h"
#include "Game.h"

void WeaponPair::onPlayerShoot(Player::ShootSide side)
{
	switch (side)
	{
	case Player::LEFT:
		left.shoot();
		break;
	case Player::RIGHT:
		right.shoot();
		break;
	default:
		break;
	}
}

WeaponPair::WeaponPair()
{
	left.position = sf::Vector2f(-75, Game::window->getSize().y);
	left.kickPosition = left.position + sf::Vector2f(-60, 45);

	right.position = sf::Vector2f(Game::window->getSize().x + 75, Game::window->getSize().y);
	right.kickPosition = right.position + sf::Vector2f(60, 45);
	right.flipHorizontal();

	Player::onShoot.subscribe([this](Player::ShootSide side) {
		onPlayerShoot(side);
		});
}

void WeaponPair::draw()
{
	left.draw();
	right.draw();
}
