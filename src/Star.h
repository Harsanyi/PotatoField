#pragma once
#include "glm/ext/quaternion_float.hpp"
#include "Random.h"
#include "glm/glm.hpp"
#include "SFML/Graphics.hpp"
#include "Texture.h"

class Star
{
	static std::vector<Star> stars;
	static sf::VertexArray verticles;
	static Texture* texture;
	static sf::RenderStates* state;

public:
	float size = Random::getDouble(1,10);

	glm::quat rot = glm::quat({
			glm::radians(Random::getDouble(-30,30) + Random::getDouble(-30,30) + Random::getDouble(-30,30)),
			glm::radians(Random::getDouble(-180,180)),
			0
		});

	sf::Color color = sf::Color(
		(sf::Uint8)Random::getDouble(50,255),
		(sf::Uint8)Random::getDouble(50,255),
		(sf::Uint8)Random::getDouble(50,255),
		(sf::Uint8)Random::getDouble(0,50)
	);

	static void create(const int& count);

	static void draw();

	static void init();
};

