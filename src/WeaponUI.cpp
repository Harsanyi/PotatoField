#include "WeaponUI.h"
#include "Game.h"
#include "Utils.h"
#include <algorithm>

WeaponUI::WeaponUI()
{
    sprite.setOriginNormal({0, 1});
    sprite.setRotation(3);
    sprite.setScale({0.7f, 0.7f});
}
void WeaponUI::draw()
{
    sinceShot = std::min(kickTime + releaseTime, sinceShot + Game::deltaTime);
    sf::Vector2f pos;
    if (sinceShot >= kickTime + releaseTime) pos = position;
    else if (sinceShot <= kickTime) pos = lerp(position, kickPosition, smoothstep(sinceShot / kickTime));
    else pos = lerp(kickPosition, position, smoothstep((sinceShot - kickTime) / releaseTime));
    sprite.setPosition(pos);
    Game::window->draw(sprite);
}
void WeaponUI::shoot() { sinceShot = 0; }
void WeaponUI::flipHorizontal()
{
    const auto scale = sprite.getScale();
    sprite.setScale(-scale.x, scale.y);
    sprite.setRotation(-sprite.getRotation());
}
