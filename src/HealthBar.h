#pragma once
#include "Sprite.h"

class HealthBar
{
	Sprite* sprite;

public:
	HealthBar();
	~HealthBar();

	void draw();
};

