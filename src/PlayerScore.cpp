#include "PlayerScore.h"
#include "Config.h"
#include "Game.h"
#include "Utils.h"

void PlayerScore::onEnemyDie(Enemy*) { if (Game::getState() == Game::GAME) ++score; }
PlayerScore::PlayerScore()
{
    setText("Score:");
    setBold(true);
    setOrigin({1, 0});
    setPosition({Config::SCREEN_WIDTH - 150.0f, 20});
    destructionSubscription = Enemy::onDestroy.Subscribe([this](Enemy* enemy) { onEnemyDie(enemy); });
}
PlayerScore::~PlayerScore() { Enemy::onDestroy.Unsubscribe(destructionSubscription); }
void PlayerScore::draw() { setText(asString("Score:", score)); Label::draw(); }
void PlayerScore::reset() { score = 0; }
