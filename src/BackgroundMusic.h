#pragma once
#include <SFML/Audio.hpp>
#include "Game.h"

class BackgroundMusic
{
    sf::Music normalMusic, deepMusic;
    Event<Game::State>::Id stateSubscription = 0;
    void onGameModeChanged(Game::State state);
    void setDeep(bool deep);
public:
    BackgroundMusic();
    ~BackgroundMusic();
};
