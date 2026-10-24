#pragma once
#include "Sprite.h"

class WeaponUI
{
    const float kickTime = 0.05f;
    const float releaseTime = 0.2f;
    Sprite sprite{"img/weapon.png"};
    float sinceShot = 1;
public:
    sf::Vector2f position;
    sf::Vector2f kickPosition;
    WeaponUI();
    void draw();
    void shoot();
    void flipHorizontal();
};
