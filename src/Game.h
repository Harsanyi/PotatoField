#pragma once
#include "SFML/Graphics.hpp"
#include "Event.h"
#include "SFML/Graphics.hpp"

class Game
{
public:
	enum State {
		MENU,
		GAME,
		GAME_OVER
	};

	static sf::RenderWindow* window;
	static Event<State> onStateChange;
	static sf::Font* font;

	static void init();
	static State getState();
	static void setState(const Game::State& newState);
	static void showCursor(const bool& show);
	static void handleEvents();

private:
	static State state;
};

