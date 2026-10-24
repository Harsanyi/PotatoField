#include "FireSprite.h"
#include "Game.h"
#include "Random.h"

FireSprite::FireSprite()
{
    maxSize = static_cast<float>(Random::getDouble(100, 300));
    lifetime = static_cast<float>(Random::getDouble(0.5, 1));
}
void FireSprite::update()
{
    age += Game::deltaTime;
    const float fraction = glm::clamp(age / lifetime, 0.0f, 1.0f);
    size = (0.5f + fraction * 0.5f) * maxSize;
    if (age >= lifetime) Destroy();
}
