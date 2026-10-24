#include "HealthBar.h"
#include "Game.h"
#include "Player.h"

void HealthBar::draw()
{
    for (int i = 0; i < Player::main->health; ++i)
    {
        sprite.setPosition(i * 46.0f + 30, 30);
        Game::window->draw(sprite);
    }
}
