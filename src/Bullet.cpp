#include "Bullet.h"
#include "Config.h"
#include "Game.h"
#include "Player.h"

void Bullet::update()
{
    position += velocity * Game::deltaTime;
    age += Game::deltaTime;
    if (age >= Config::BULLET_LIFETIME || Game::getState() != Game::GAME) Destroy();
}
bool Bullet::isActiveCollider() { return Game::getState() == Game::GAME; }
void Bullet::onOverlap(Graphics* other) { if (dynamic_cast<Player*>(other)) Destroy(); }
