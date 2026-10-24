#include "Assets.h"
#include "BackgroundMusic.h"
#include "Enemy.h"
#include "Explosion.h"
#include "Game.h"
#include "GameOverMenu.h"
#include "Graphics.h"
#include "HealthBar.h"
#include "Log.h"
#include "MainMenu.h"
#include "Player.h"
#include "PlayerScore.h"
#include "Potato.h"
#include "Star.h"
#include "WeaponPair.h"
#include <iostream>

int main(int argc, char** argv)
{
    int result = 0;
    try
    {
        Assets::initialize(argc ? argv[0] : "");
        Game::init();
        Graphics::init();
        Star::init();
        Explosion::init();
        new Player(); // The world registry owns the player and all spawned world entities.
        {
            BackgroundMusic music;
            MainMenu mainMenu;
            GameOverMenu gameOverMenu;
            HealthBar healthBar;
            WeaponPair weapons;
            PlayerScore score;
            const auto stateSubscription = Game::onStateChange.Subscribe([&score](Game::State state)
            {
                Player::main->isControllable = state == Game::GAME;
                Game::showCursor(state != Game::GAME);
                if (state == Game::GAME)
                {
                    Player::main->health = Config::PLAYER_HEALTH;
                    Enemy::create(1);
                    score.reset();
                }
                else if (state == Game::GAME_OVER) Enemy::killInstances();
            });
            const auto enemySubscription = Enemy::onDestroy.Subscribe([](Enemy*)
            {
                if (Game::getState() == Game::GAME) Enemy::create(2);
            });
            const auto playerSubscription = Player::main->onDestroy.Subscribe([] { Game::setState(Game::GAME_OVER); });
            Star::create(Config::STAR_COUNT);
            Potato::create(Config::POTATO_COUNT);

            while (Game::window->isOpen())
            {
                Game::beginFrame();
                Game::handleEvents();
                if (!Game::window->isOpen()) break;
                Graphics::updateBehaviours();
                Explosion::update(Game::deltaTime);
                Log::update(Game::deltaTime);
                Game::window->clear();
                Star::draw();
                Graphics::draw();
                weapons.draw();
                switch (Game::getState())
                {
                case Game::MENU: mainMenu.update(); mainMenu.draw(); break;
                case Game::GAME: score.draw(); healthBar.draw(); break;
                case Game::GAME_OVER: gameOverMenu.update(); gameOverMenu.draw(); break;
                }
                Log::draw();
                Game::window->display();
            }
            Game::onStateChange.Unsubscribe(stateSubscription);
            Enemy::onDestroy.Unsubscribe(enemySubscription);
            Player::main->onDestroy.Unsubscribe(playerSubscription);
        }
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << '\n';
        result = 1;
    }
    // Disconnect observers before deleting their subjects, including exceptional shutdown.
    Game::onStateChange.Clear();
    Enemy::onDestroy.Clear();
    Player::onShoot.Clear();
    Explosion::shutdown();
    Graphics::shutdown();
    Star::shutdown();
    Log::clear();
    Game::shutdown();
    return result;
}
