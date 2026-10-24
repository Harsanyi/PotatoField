#include "Log.h"
#include "Config.h"
#include "Game.h"
#include <algorithm>
#include <iostream>

sf::Text Log::text;
std::vector<Log::Data> Log::entries;

void Log::update(float deltaTime)
{
    entries.erase(std::remove_if(entries.begin(), entries.end(), [deltaTime](Data& entry)
    {
        if (entry.remaining < 0) return false;
        entry.remaining -= deltaTime;
        return entry.remaining <= 0;
    }), entries.end());
}
void Log::draw()
{
    text.setFont(*Game::font);
    text.setCharacterSize(Config::LOG_FONT_SIZE);
    text.setFillColor(sf::Color::White);
    for (std::size_t i = 0; i < entries.size(); ++i)
    {
        text.setString(entries[i].text);
        text.setPosition(0, static_cast<float>(i * Config::LOG_FONT_SIZE));
        Game::window->draw(text);
    }
}
void Log::log(const char* message, float seconds)
{
    entries.push_back({message, seconds > 0 ? seconds : -1});
    std::cout << message << '\n';
}
void Log::clear() { entries.clear(); text = sf::Text{}; }
