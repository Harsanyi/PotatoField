#include "BackgroundMusic.h"
#include "Assets.h"
#include <stdexcept>

void BackgroundMusic::onGameModeChanged(Game::State state) { setDeep(state != Game::GAME); }
void BackgroundMusic::setDeep(bool deep)
{
    deepMusic.setVolume(deep ? 100.0f : 0.0f);
    normalMusic.setVolume(deep ? 0.0f : 100.0f);
}
BackgroundMusic::BackgroundMusic()
{
    if (!normalMusic.openFromFile(Assets::file("audio/music.ogg")) ||
        !deepMusic.openFromFile(Assets::file("audio/music_deep.ogg")))
        throw std::runtime_error("Cannot load background music");
    deepMusic.setLoop(true);
    normalMusic.setLoop(true);
    deepMusic.play();
    normalMusic.play();
    setDeep(true);
    stateSubscription = Game::onStateChange.Subscribe([this](Game::State state) { onGameModeChanged(state); });
}
BackgroundMusic::~BackgroundMusic() { Game::onStateChange.Unsubscribe(stateSubscription); }
