#pragma once
#include "SFML/Graphics.hpp"
#include "config.h"

class Log
{
	Log();

	class Data {
	public:
		sf::String text;
		int lifeTime = -1;

		Data(const char* text, float sec = 0) {
			this->text = text;
			this->lifeTime = sec > 0. ? (int)(sec * Config::FRAMERATE) : -1;
		};
	};

	static bool inited;
	static sf::Text text;
	static std::vector<Data*> logDatas;

	static void init();

public:	
	static void draw();
	static void log(const char* text, float time=0);
};

