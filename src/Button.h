#pragma once
#include "Label.h"
#include "Event.h"
#include "SFML/Audio.hpp"

class Button
{
	Label label;
	sf::Vector2f size{200,30};
	sf::Vector2f position{0,0};
	sf::VertexArray border;
	bool mouseHere = false;
	bool buttonDown = false;
	bool lastButtonDown = false;
	sf::Color color = sf::Color::White;
	sf::SoundBuffer hoverBuffer;
	sf::SoundBuffer clickBuffer;
	sf::Sound hoverSound;
	sf::Sound clickSound;
	
	void updatePositions();

public:
	Button();

	Event<> onClick;

	void draw();
	void setPosition(const sf::Vector2f& pos);
	void update();
	bool isPosInRect(const sf::Vector2f& mousePos,const sf::Vector2f& min, const sf::Vector2f& max);
	bool isMouseHere();
	void setText(const std::string& text);
	void setColor(const sf::Color& color);
	void setSize(const sf::Vector2f& size);
	void setFontSize(const float& charSize);
};

