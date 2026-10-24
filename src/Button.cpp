#include "Button.h"
#include "Assets.h"
#include <stdexcept>
#include "Game.h"
#include <iostream>
#include "Log.h"

void Button::updatePositions()
{
	sf::Vector2f halfSize = size * 0.5f;
	border[0].position = position + sf::Vector2f(-halfSize.x, -halfSize.y);
	border[1].position = position + sf::Vector2f(halfSize.x, -halfSize.y);
	border[2].position = position + sf::Vector2f(halfSize.x, halfSize.y);
	border[3].position = position + sf::Vector2f(-halfSize.x, halfSize.y);
	border[4].position = border[0].position;

	label.setOrigin({0.5,0.5});
	label.setPosition(position);
}

Button::Button()
{
	border = sf::VertexArray(sf::PrimitiveType::LinesStrip, 5);
	updatePositions();
	setColor(color);

	if (!hoverBuffer.loadFromFile(Assets::file("audio/hover.wav")) ||
	    !clickBuffer.loadFromFile(Assets::file("audio/click.wav")))
	    throw std::runtime_error("Cannot load UI sound");
	hoverSound.setBuffer(hoverBuffer);
	clickSound.setBuffer(clickBuffer);
}

void Button::draw()
{
	if (mouseHere) {
		Game::window->draw(border);
	}
	label.draw();
}

void Button::setPosition(const sf::Vector2f& pos)
{
	position = pos;
	updatePositions();
}

void Button::update()
{
	if (mouseHere != isMouseHere()) {
		mouseHere = !mouseHere;
		if (mouseHere) {
			hoverSound.play();
		}
	}
	buttonDown = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	label.setFillColor(color);

	if (mouseHere && lastButtonDown && !buttonDown) {
		label.setFillColor(sf::Color::Red);
		clickSound.play();
		onClick.Invoke();
	}

	lastButtonDown = buttonDown;
}

bool Button::isPosInRect(const sf::Vector2f& mousePos, const sf::Vector2f& min, const sf::Vector2f& max)
{
	return mousePos.x >= min.x && mousePos.x <= max.x && mousePos.y >= min.y && mousePos.y <= max.y;
}

bool Button::isMouseHere()
{
	return isPosInRect((sf::Vector2f)sf::Mouse::getPosition(*Game::window), border[0].position, border[2].position);
}

void Button::setText(const std::string& text)
{
	label.setText(text);
}

void Button::setColor(const sf::Color& color)
{
	this->color = color;
	label.setFillColor(color);
	for (int i = 0; i < border.getVertexCount(); i++) {
		border[i].color = color;
	}
}

void Button::setSize(const sf::Vector2f& size)
{
	this->size = size;
	updatePositions();
}

void Button::setFontSize(const float& charSize)
{
	label.setSize(charSize);
	updatePositions();
}
