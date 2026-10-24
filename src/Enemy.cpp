#include "Enemy.h"
#include "Bullet.h"
#include "chrono"
#include "Random.h"
#include "Log.h"
#include "FireSprite.h"
#include "Random.h"
#include "Explosion.h"
#include "HealthPill.h"

Event<Enemy*> Enemy::onDestroy;

void Enemy::destroyFX()
{
	Explosion* explosion = new Explosion(this->position);
}

Enemy::~Enemy()
{
	destroyFX();
	if (Random::getDouble() < Config::PILL_SPAWN_RATE) {
		HealthPill* pill = new HealthPill();
		pill->position = this->position;
	}
	onDestroy.Invoke(this);
	Graphics::~Graphics();
}

Enemy::Enemy()
{
	position = glm::vec3(
		Random::getDouble(0, Config::FIELD_SIZE),
		Random::getDouble(0, Config::FIELD_SIZE),
		Random::getDouble(0, Config::FIELD_SIZE)
	);
	size = Config::ENEMY_SIZE * Random::getDouble(1. / Config::POTATO_SIZE_VARIANCE, Config::POTATO_SIZE_VARIANCE);
}

void Enemy::create(const int& count)
{
	for (int i = 0; i < count; ++i) {
		auto enemy = new Enemy();
		enemy->name = "Enemy_" + std::to_string(i);
	}
}

void Enemy::killInstances()
{
	Enemy* cur;
	for (auto grap : instances) {
		cur = dynamic_cast<Enemy*>(grap);
		if (cur) {
			cur->Destroy();
		}
	}
}

void Enemy::handleMovement()
{
	glm::vec3 toCam = Player::main->position - this->position;
	toCam = glm::normalize(toCam) * Config::ENEMY_SPEED * Config::DELTA_TIME;

	this->velocity += toCam;
	float realSpeed = glm::length(velocity);
	if (realSpeed > Config::ENEMY_SPEED) {
		velocity *= Config::ENEMY_SPEED / realSpeed;
	}
	this->position += this->velocity * Config::DELTA_TIME;
}

void Enemy::handleShooting()
{
	if (isShouldShoot()) {
		shoot();
	}
}

bool Enemy::isShouldShoot()
{
	return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now()- lastShoot).count() >= Config::ENEMY_SHOOTING_FREQ;
}

void Enemy::shoot()
{
	Bullet* bullet = new Bullet();
	bullet->position = this->position;
	bullet->size = Config::BULLET_SIZE;
	bullet->velocity = glm::normalize(Player::main->position - bullet->position) * Config::BULLET_SPEED + this->velocity;
	
	lastShoot = std::chrono::steady_clock::now();	
}

void Enemy::update()
{
	handleMovement();
	handleShooting();
}
