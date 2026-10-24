#include "Game.h"
#include "Assets.h"
#include "Config.h"
#include <algorithm>
#include <stdexcept>

std::unique_ptr<sf::RenderWindow> Game::window;
std::unique_ptr<sf::Font> Game::font;
Event<Game::State> Game::onStateChange;
Game::State Game::state = Game::MENU;
sf::Clock Game::frameClock;
float Game::deltaTime = 0;

void Game::init(bool hidden)
{
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT), "PotatoField", sf::Style::Titlebar | sf::Style::Close);
    window->setVisible(!hidden);
    window->setFramerateLimit(Config::FRAMERATE);
    font = std::make_unique<sf::Font>();
    if (!font->loadFromFile(Assets::file("fonts/RobotoMono.ttf"))) throw std::runtime_error("Cannot load Roboto Mono font");
    state = MENU;
    deltaTime = 0;
    frameClock.restart();
}

void Game::shutdown()
{
    onStateChange.Clear();
    font.reset();
    if (window) window->close();
    window.reset();
    state = MENU;
}

void Game::beginFrame()
{
    deltaTime = std::clamp(frameClock.restart().asSeconds(), 0.0f, 0.05f);
}

Game::State Game::getState() { return state; }

void Game::setState(State next)
{
    if (state == next) return;
    state = next;
    if (window && window->hasFocus())
        sf::Mouse::setPosition({Config::SCREEN_WIDTH / 2, Config::SCREEN_HEIGHT / 2}, *window);
    onStateChange.Invoke(state);
}

void Game::showCursor(bool show) { if (window) window->setMouseCursorVisible(show); }

void Game::handleEvents()
{
    sf::Event event{};
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed ||
            (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            window->close();
    }
}
