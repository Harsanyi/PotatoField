#pragma once
#include "Label.h"
#include "Enemy.h"

class PlayerScore : public Label
{
	int score = 0;

	void onEnemyDie(Enemy* enemy);

public:
	PlayerScore();
	~PlayerScore();

	void draw();

	void reset();
};

