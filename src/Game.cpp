#include "Game.h"
#include "SFML/Graphics.hpp"
#include "config.h"
#include "Event.h"
#include "config.h"

sf::RenderWindow* Game::window;
sf::Font* Game::font;

Game::State Game::state = Game::MENU;
Event<Game::State> Game::onStateChange;

void Game::init()
{
    window = new sf::RenderWindow({ Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT }, "PotatoField");
    window->setFramerateLimit(Config::FRAMERATE);
    font = new sf::Font();
    font->loadFromFile("roboto.ttf");

    state = Game::GAME_OVER;
    setState(Game::MENU);
}

Game::State Game::getState()
{
    return state;
}

void Game::setState(const Game::State& newState)
{
    if (state == newState)return;

    sf::Mouse::setPosition({Config::SCREEN_WIDTH/2,Config::SCREEN_HEIGHT/2});
    state = newState;
    onStateChange.Invoke(state);
}

void Game::showCursor(const bool& show)
{
    window->setMouseCursorVisible(show);
}

void Game::handleEvents()
{
	for (auto event = sf::Event{}; window->pollEvent(event);)
	{
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Escape) {
				window->close();
			}
		}

		if (event.type == sf::Event::Closed)
		{
			window->close();
		}
	}
}
