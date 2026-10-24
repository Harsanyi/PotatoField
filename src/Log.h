#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Log
{
    struct Data { sf::String text; float remaining; };
    static sf::Text text;
    static std::vector<Data> entries;
public:
    static void update(float deltaTime);
    static void draw();
    static void log(const char* message, float seconds = 0);
    static void clear();
    static std::size_t count() { return entries.size(); }
};
