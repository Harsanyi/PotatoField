#include "GameOverMenu.h"
#include "Game.h"

void GameOverMenu::onClick()
{
	Game::setState(Game::MENU);
}

GameOverMenu::GameOverMenu()
{
	button.setText("Game Over");
	
	sf::Vector2f center = (sf::Vector2f)Game::window->getSize();
	center.x /= 2;
	center.y /= 2;
	button.setPosition(center);
	button.setColor(sf::Color::Red);
	button.setSize({400,75});
	button.setFontSize(50);
	button.onClick.subscribe([this]() {onClick(); });
}

void GameOverMenu::update()
{
	button.update();
}

void GameOverMenu::draw()
{
	button.draw();
}
