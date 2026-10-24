#pragma once
#include "vector"
#include "sstream"
#include "glm/glm.hpp"
#include "SFML/Graphics.hpp"

template<typename T>
inline void remove(std::vector<T>& vec, const T& item)
{
	auto iterator = find(vec.begin(), vec.end(), item);
	if (iterator != vec.end()) {
		vec.erase(iterator);
	}
};

template<typename T>
inline int indexOf(std::vector<T>& vec, const T& item) {
	auto iterator = find(vec.begin(), vec.end(), item);
	
	if (iterator != vec.end()) {
		return std::distance(vec.begin(), iterator);
	}
	else {
		return -1;
	}
}

template<typename... Args>
inline std::string asString(const Args&... args) {
	std::ostringstream oss;
	(oss << ... << args);
	return oss.str();
}

inline glm::vec2 rotate(const glm::vec2& v2, const float angleRad) {
	float cos = std::cos(angleRad);
	float sin = std::sin(angleRad);

	return {
		v2.x * cos - v2.y * sin,
		v2.x * sin + v2.y * cos
	};
}

inline static sf::Vector2f v2f(const glm::vec2& v2) {
	return {v2.x, v2.y};
}

inline static glm::vec2 operator +(const glm::vec2& a, const glm::vec2& b) {
	return { a.x + b.x, a.y + b.y };
}

inline static float smoothstep(const float& t) {
	return t * t * (3.-2.*t);
}

inline static float lerp(const float& a, const float& b, const float& t) {
	return (b - a) * t + a;
}

inline static sf::Vector2f lerp(const sf::Vector2f& a, const sf::Vector2f& b, const float& t) {
	return sf::Vector2f(lerp(a.x,b.x,t), lerp(a.y,b.y,t));
}

template<typename T>
inline static T& getRandom(std::vector<T>& vector) {
	return vector[Random::getInt(0, vector.size())];
}
