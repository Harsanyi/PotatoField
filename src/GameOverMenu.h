#pragma once
#include "Button.h"

class GameOverMenu
{
	Button button;

	void onClick();

public:
	GameOverMenu();

	void update();
	void draw();
};

