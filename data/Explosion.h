#pragma once
#include "glm/glm.hpp"
#include "FireSprite.h"
#include "SFML/Audio.hpp"
#include "thread"

class Explosion
{
	glm::vec3 position;
	std::vector<FireSprite*> sprites;
	sf::SoundBuffer soundBuffer;
	sf::Sound sound;
	std::thread lifeThread;

	void startLifeCheck();

public:
	Explosion(glm::vec3 pos);
};

