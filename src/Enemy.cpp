#include "Enemy.h"
#include "Bullet.h"
#include "Explosion.h"
#include "Game.h"
#include "HealthPill.h"
#include "Player.h"
#include "Random.h"
#include "WorldMath.h"

Event<Enemy*> Enemy::onDestroy;

Enemy::Enemy()
{
    position = {Random::getDouble(0, Config::FIELD_SIZE), Random::getDouble(0, Config::FIELD_SIZE), Random::getDouble(0, Config::FIELD_SIZE)};
    size = Config::ENEMY_SIZE * static_cast<float>(Random::getDouble(1.0 / Config::POTATO_SIZE_VARIANCE, Config::POTATO_SIZE_VARIANCE));
}

Enemy::~Enemy()
{
    // Menu/shutdown cleanup is not a gameplay kill; the base destructor runs automatically.
    if (!Graphics::isShuttingDown() && Game::getState() == Game::GAME)
    {
        destroyFX();
        if (Random::getDouble() < Config::PILL_SPAWN_RATE)
        {
            auto* pill = new HealthPill();
            pill->position = position;
        }
        onDestroy.Invoke(this);
    }
}

void Enemy::destroyFX() { Explosion::spawn(position); }
void Enemy::create(int count) { for (int i = 0; i < count; ++i) (new Enemy())->name = "Enemy"; }
void Enemy::killInstances()
{
    for (auto* object : instances) if (auto* enemy = dynamic_cast<Enemy*>(object)) enemy->Destroy();
}

void Enemy::handleMovement()
{
    const auto delta = wrappedDelta(Player::main->position - position);
    const float distance = glm::length(delta);
    if (distance > 0.001f) velocity += delta / distance * Config::ENEMY_SPEED * Game::deltaTime;
    const float speed = glm::length(velocity);
    if (speed > Config::ENEMY_SPEED) velocity *= Config::ENEMY_SPEED / speed;
    position += velocity * Game::deltaTime;
}
bool Enemy::shouldShoot() const { return shotTimer >= Config::ENEMY_SHOOTING_FREQ; }
void Enemy::handleShooting() { if (shouldShoot()) shoot(); }
void Enemy::shoot()
{
    const auto delta = wrappedDelta(Player::main->position - position);
    const float distance = glm::length(delta);
    if (distance <= 0.001f) return;
    auto* bullet = new Bullet();
    bullet->position = position;
    bullet->size = Config::BULLET_SIZE;
    bullet->velocity = delta / distance * Config::BULLET_SPEED + velocity;
    shotTimer = 0;
}
void Enemy::update()
{
    if (Game::getState() != Game::GAME) return;
    shotTimer += Game::deltaTime;
    handleMovement();
    handleShooting();
}
