#pragma once
#include "SFML/Graphics.hpp"

class Label : public sf::Text
{
	sf::Vector2f origin{0.5,0.5};
	bool isBold = false;

	void recalculateOrigin();
	void resetStyle();

public:
	
	Label();

	void draw();
	void setText(const sf::String& text);
	void setSize(const float& size);
	void setOrigin(const sf::Vector2f anchor);
	void setPosition(const sf::Vector2f screenPos);
	void setBold(bool bold);
};

