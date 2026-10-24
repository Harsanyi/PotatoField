#pragma once
#include <string>
#include "SFML/Graphics.hpp"

class Texture : public sf::Texture
{

public:
	Texture(const std::string& fileName);
	Texture();
};

