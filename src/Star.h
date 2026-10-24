#pragma once
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Config.h"
#include "Random.h"
#include "Texture.h"

class Star
{
    static std::vector<Star> stars;
    static sf::VertexArray vertices;
    static std::unique_ptr<Texture> texture;
    static sf::RenderStates state;
public:
    float size = static_cast<float>(Random::getDouble(1, Config::MAX_STAR_SIZE));
    glm::quat rotation = glm::quat(glm::vec3(
        glm::radians(Random::getDouble(-30, 30) + Random::getDouble(-30, 30) + Random::getDouble(-30, 30)),
        glm::radians(Random::getDouble(-180, 180)), 0));
    sf::Color color{
        static_cast<sf::Uint8>(Random::getDouble(50, 255)),
        static_cast<sf::Uint8>(Random::getDouble(50, 255)),
        static_cast<sf::Uint8>(Random::getDouble(50, 255)),
        static_cast<sf::Uint8>(Random::getDouble(0, 50))};
    static void create(int count);
    static void draw();
    static void init();
    static void shutdown();
};
