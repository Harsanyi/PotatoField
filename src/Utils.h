#pragma once
#include <algorithm>
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include "Random.h"

template<typename T>
inline void remove(std::vector<T>& values, const T& item)
{
    const auto found = std::find(values.begin(), values.end(), item);
    if (found != values.end()) values.erase(found);
}
template<typename... Args>
inline std::string asString(const Args&... args)
{
    std::ostringstream stream;
    (stream << ... << args);
    return stream.str();
}
inline glm::vec2 rotate(const glm::vec2& value, float angle)
{
    const float cosine = std::cos(angle), sine = std::sin(angle);
    return {value.x * cosine - value.y * sine, value.x * sine + value.y * cosine};
}
inline sf::Vector2f v2f(const glm::vec2& value) { return {value.x, value.y}; }
inline float smoothstep(float t) { return t * t * (3 - 2 * t); }
inline float lerp(float a, float b, float t) { return (b - a) * t + a; }
inline sf::Vector2f lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t)
{
    return {lerp(a.x, b.x, t), lerp(a.y, b.y, t)};
}
template<typename T>
inline T& getRandom(std::vector<T>& values)
{
    if (values.empty()) throw std::out_of_range("Cannot choose from an empty vector");
    return values[Random::getIndex(values.size())];
}
