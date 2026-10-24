#include "PlayerScore.h"
#include "config.h"
#include "Game.h"
#include "Enemy.h"

void PlayerScore::onEnemyDie(Enemy* enemy)
{
	score++;
}

PlayerScore::PlayerScore()
{
	setText("Score:");
	setBold(true);
	setOrigin({1.,0.});
	setPosition({ Config::SCREEN_WIDTH-150, 20 });

	Enemy::onDestroy.subscribe([&](Enemy* enemy) {this->onEnemyDie(enemy); });
}

PlayerScore::~PlayerScore()
{
	Enemy::onDestroy.UnSubscribe([&](Enemy* enemy) {this->onEnemyDie(enemy); });
}

void PlayerScore::draw()
{
	setText(asString("Score:",score));
	Label::draw();
}

void PlayerScore::reset()
{
	score = 0;
}
