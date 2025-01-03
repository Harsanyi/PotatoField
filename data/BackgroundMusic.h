#pragma once
#include "SFML/Audio.hpp"
#include "Game.h"

class BackgroundMusic
{
	sf::Music normalMusic;
	sf::Music deepMusic;

	void onGameModeChanged(Game::State state);

	void setDeep(bool deep);

public:
	BackgroundMusic();
};

