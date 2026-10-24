#include "BackgroundMusic.h"
#include "SFML/Audio.hpp"
#include "Game.h"

void BackgroundMusic::onGameModeChanged(Game::State state)
{
	switch (state) {
	case Game::MENU:
		setDeep(true);
		break;
	case Game::GAME_OVER:
		setDeep(true);
		break;
	case Game::GAME:
		setDeep(false);
		break;
	}
}

void BackgroundMusic::setDeep(bool deep)
{
	deepMusic.setVolume(deep ? 100 : 0);
	normalMusic.setVolume(deep ? 0 : 100);
}

BackgroundMusic::BackgroundMusic()
{	
	normalMusic.openFromFile("audio/music.wav");
	deepMusic.openFromFile("audio/music_deep.wav");

	deepMusic.setLoop(true);
	normalMusic.setLoop(true);

	deepMusic.play();
	normalMusic.play();

	setDeep(true);

	Game::onStateChange.subscribe([this](Game::State state){
		this->onGameModeChanged(state);
		});
}
