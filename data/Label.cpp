#include "Label.h"
#include "Game.h"
#include "SFML/Graphics.hpp"

void Label::recalculateOrigin()
{
	auto size = getGlobalBounds().getSize();
	sf::Text::setOrigin({origin.x * size.x, origin.y * size.y});
}

void Label::resetStyle()
{
	setStyle(isBold * sf::Text::Bold);
}

Label::Label()
{
	setFont(*Game::font);
	setText("Label");
	setSize(20);
}

void Label::draw()
{
	Game::window->draw(*this);
}

void Label::setText(const sf::String& text)
{
	setString(text);
	recalculateOrigin();
}

void Label::setSize(const float& size)
{
	setCharacterSize(size);
	recalculateOrigin();
}

void Label::setOrigin(const sf::Vector2f origin)
{
	this->origin = origin;
	recalculateOrigin();
}

void Label::setPosition(const sf::Vector2f screenPos)
{
	auto delta = getPosition() - (getGlobalBounds().getPosition() + getGlobalBounds().getSize() * 0.5f);
	sf::Text::setPosition(screenPos + delta);
	recalculateOrigin();
}

void Label::setBold(bool bold)
{
	isBold = bold;
	resetStyle();
	recalculateOrigin();
}
