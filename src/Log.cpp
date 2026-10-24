#include "Log.h"
#include <iostream>
#include "config.h"
#include "Game.h"

bool Log::inited = false;
sf::Text Log::text{};
std::vector<Log::Data*> Log::logDatas{};

Log::Log()
{    
    
}

void Log::init()
{
    text.setFont(*Game::font);
    text.setCharacterSize(Config::LOG_FONT_SIZE);
    text.setFillColor(sf::Color::White);
    inited = true;

    log("Log is inited", 1);
}

void Log::draw()
{
    if (!inited) {
        init();
    }

    for (int i = logDatas.size()-1; i > -1; i--) {
        text.setString(logDatas[i]->text);
        text.setPosition(sf::Vector2f(0,i*Config::LOG_FONT_SIZE));
        Game::window->draw(text);

        logDatas[i]->lifeTime--;

        if (logDatas[i]->lifeTime == 0) {
            logDatas.erase(logDatas.begin());
        }
    }
}

void Log::log(const char* text, float time)
{
    Data* data = new Data(text,time);
    logDatas.push_back(data);
    std::cout << text << std::endl;
}
