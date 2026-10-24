#pragma once
#include "SFML/Graphics.hpp"
#include "Texture.h"

class Sprite : public sf::Sprite
{
	Texture texture;

public:
	Sprite(const std::string& fileName);
	Sprite(const Texture& texture);

	sf::Vector2f getSize();
	void setSize(const sf::Vector2f& size);
	void setOriginNormal(const sf::Vector2f& normOrig);
};

