#pragma once
#include <memory>
#include <vector>
#include <SFML/Audio.hpp>
#include <glm/glm.hpp>

class Explosion
{
    static std::unique_ptr<sf::SoundBuffer> buffer;
    static std::vector<std::unique_ptr<Explosion>> active;
    sf::Sound sound;
    float age = 0;
    float lifetime = 1;

public:
    explicit Explosion(glm::vec3 position);
    static void init();
    static void spawn(glm::vec3 position);
    static void update(float deltaTime);
    static void shutdown();
    static std::size_t count() { return active.size(); }
};
