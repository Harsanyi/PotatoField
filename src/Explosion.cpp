#include "Explosion.h"
#include "Assets.h"
#include "FireSprite.h"
#include "Player.h"
#include "Random.h"
#include <algorithm>
#include <stdexcept>

std::unique_ptr<sf::SoundBuffer> Explosion::buffer;
std::vector<std::unique_ptr<Explosion>> Explosion::active;

void Explosion::init()
{
    buffer = std::make_unique<sf::SoundBuffer>();
    if (!buffer->loadFromFile(Assets::file("audio/explosion.wav"))) throw std::runtime_error("Cannot load explosion sound");
}

Explosion::Explosion(glm::vec3 position)
{
    for (int i = 0; i < 5; ++i)
    {
        auto* fire = new FireSprite();
        fire->position = position + glm::vec3(Random::getDouble(-20, 20), Random::getDouble(-20, 20), Random::getDouble(-20, 20));
    }
    sound.setBuffer(*buffer);
    sound.setRelativeToListener(true);
    const auto local = Player::main->worldToLocalPosition(position);
    // SFML/OpenAL forward is -Z, while the gameplay camera looks along +Z.
    sound.setPosition(local.x, -local.y, -local.z);
    const float pitch = static_cast<float>(Random::getDouble(0.7, 1));
    sound.setPitch(pitch);
    lifetime = std::max(1.0f, buffer->getDuration().asSeconds() / pitch);
    sound.play();
}

void Explosion::spawn(glm::vec3 position)
{
    active.push_back(std::make_unique<Explosion>(position));
}
void Explosion::update(float deltaTime)
{
    for (auto& effect : active) effect->age += deltaTime;
    active.erase(std::remove_if(active.begin(), active.end(),
        [](const auto& effect) { return effect->age >= effect->lifetime; }), active.end());
}
void Explosion::shutdown() { active.clear(); buffer.reset(); }
