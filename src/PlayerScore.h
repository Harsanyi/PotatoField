#pragma once
#include "Label.h"
#include "Enemy.h"

class PlayerScore : public Label
{
    int score = 0;
    Event<Enemy*>::Id destructionSubscription = 0;
    void onEnemyDie(Enemy* enemy);
public:
    PlayerScore();
    ~PlayerScore();
    void draw();
    void reset();
    int value() const { return score; }
};
