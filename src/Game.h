#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "Event.h"

class Game
{
public:
    enum State { MENU, GAME, GAME_OVER };
    static std::unique_ptr<sf::RenderWindow> window;
    static std::unique_ptr<sf::Font> font;
    static Event<State> onStateChange;
    static float deltaTime;

    static void init(bool hidden = false);
    static void shutdown();
    static void beginFrame();
    static State getState();
    static void setState(State newState);
    static void showCursor(bool show);
    static void handleEvents();

private:
    static State state;
    static sf::Clock frameClock;
};
