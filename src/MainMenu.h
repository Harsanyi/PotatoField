#pragma once
#include "Label.h"
#include "Button.h"

class MainMenu
{
	Label title;
	Button startButton;
	Button quitButton;

	void onStartGame();
	void onQuit();

public:
	MainMenu();

	void draw();
	void update();
};

