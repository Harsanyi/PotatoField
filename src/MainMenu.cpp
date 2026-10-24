#include "MainMenu.h"
#include "Game.h"
#include "Button.h"

void MainMenu::onStartGame()
{
	Game::setState(Game::GAME);
}

void MainMenu::onQuit()
{
	Game::window->close();
}

MainMenu::MainMenu()
{
	sf::Vector2u windowSize = Game::window->getSize();

	title.setText("Potato Field");
	title.setSize(40);
	title.setPosition({(float)windowSize.x / 2, 200});
	title.setOrigin({0.5,0.5});
	title.setBold(true);

	startButton.setPosition({(float)windowSize.x / 2, 275});
	startButton.setText("Start Game");
	startButton.onClick.subscribe([this]() {onStartGame(); });

	quitButton.setPosition({(float)windowSize.x / 2, 325});
	quitButton.setText("Quit");
	quitButton.onClick.subscribe([this]() {onQuit(); });
}

void MainMenu::draw()
{
	title.draw();
	startButton.draw();
	quitButton.draw();
}

void MainMenu::update()
{
	startButton.update();
	quitButton.update();
}
