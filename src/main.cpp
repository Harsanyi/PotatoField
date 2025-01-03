#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <SFML/Graphics.hpp>
#include "graphics.h"
#include "Random.h"
#include "Player.h"
#include "string"
#include <iostream>
#include "Config.h"
#include "glm/glm.hpp"
#include "Potato.h"
#include "Enemy.h"
#include "Log.h"
#include "sstream"
#include "Bullet.h"
#include "Utils.h"
#include "glm/gtx/string_cast.hpp"
#include "Label.h"
#include "Game.h"
#include <MainMenu.h>
#include "GameOverMenu.h"
#include "Texture.h"
#include "Sprite.h"
#include "Star.h"
#include "PlayerScore.h"
#include "HealthBar.h"
#include "WeaponPair.h"
#include "BackgroundMusic.h"

Player* player;
MainMenu* mainMenu;
PlayerScore* score;
GameOverMenu* gameOverMenu;
HealthBar* healthBar;
WeaponPair* weapons;
BackgroundMusic* music;

void onGameStateChange(Game::State state) {
    Player::main->isControllable = false;
    Game::showCursor(true);

    switch (state)
    {
    case Game::MENU:
        break;
    case Game::GAME:
        Player::main->isControllable = true;
        Player::main->health = 3;
        Enemy::create(1);
        Game::showCursor(false);
        score->reset();
        break;
    case Game::GAME_OVER:
        Enemy::killInstances();
        break;
    default:
        Log::log("Unhandled game state!");
        break;
    }
}

void onEnemyDestroyed(Enemy* enemy) {
    if (Game::getState() == Game::GAME) {
        Enemy::create(2);
    }
}

int main()
{
    Graphics::init();
    Star::init();
    Enemy::onDestroy.subscribe(std::function(onEnemyDestroyed));

    player = new Player();
    player->onDestroy.subscribe(std::function<void()>([]() {
        Game::setState(Game::GAME_OVER);
    }));

    Game::onStateChange.subscribe(std::function(onGameStateChange));
    Game::init();

    Star::create(Config::STAR_COUNT);
    Potato::create(Config::POTATO_COUNT);

    music = new BackgroundMusic();
    mainMenu = new MainMenu();
    gameOverMenu = new GameOverMenu();
    healthBar = new HealthBar();
    weapons = new WeaponPair();
    score = new PlayerScore();

    while (Game::window->isOpen())
    {
        try {
            Game::handleEvents();
            Game::window->clear();
            Graphics::updateBehaviours();

            Star::draw();
            Graphics::draw(); 
            weapons->draw();

            switch (Game::getState())
            {
            case Game::MENU:
                mainMenu->update();
                mainMenu->draw();
                break;
            case Game::GAME:
                score->draw();
                healthBar->draw();
                break;
            case Game::GAME_OVER:
                gameOverMenu->update();
                gameOverMenu->draw();
                break;
            default:
                break;
            }

            Log::draw();

            Game::window->display();
        }
        catch (const std::runtime_error& e) {
            std::cout << e.what() << std::endl;
        }
    }
}
