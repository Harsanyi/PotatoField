#include "HealthBar.h"
#include "Player.h"
#include "Game.h"

HealthBar::HealthBar()
{
    sprite = new Sprite("img/playerLife.png");
}

HealthBar::~HealthBar()
{
    delete(sprite);
}

void HealthBar::draw()
{
    for (int i = 0; i < Player::main->health; i++) {
        sprite->setPosition({ i * 46.f + 30.f, 30.f });
        Game::window->draw(*sprite);
    }
}
