#include "Assets.h"
#include "BackgroundMusic.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Explosion.h"
#include "FireSprite.h"
#include "Game.h"
#include "GameOverMenu.h"
#include "HealthBar.h"
#include "HealthPill.h"
#include "Log.h"
#include "MainMenu.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "PlayerScore.h"
#include "Potato.h"
#include "Star.h"
#include "Utils.h"
#include "WeaponPair.h"
#include <iostream>
#include <stdexcept>

static void check(bool condition, const char* message)
{
    if (!condition) throw std::runtime_error(message);
}
static void tick(float delta = 0)
{
    Game::deltaTime = delta;
    Graphics::updateBehaviours();
    Explosion::update(delta);
    Log::update(delta);
}
template<typename T> static std::size_t count()
{
    return static_cast<std::size_t>(std::count_if(Graphics::instances.begin(), Graphics::instances.end(),
        [](Graphics* object) { return dynamic_cast<T*>(object) != nullptr; }));
}
static void cleanWorldExceptPlayer()
{
    Game::setState(Game::MENU);
    for (auto* object : Graphics::instances) if (object != Player::main) object->Destroy();
    tick();
}
static sf::Color renderCenter()
{
    Game::window->clear(sf::Color::Black);
    Graphics::draw();
    sf::Texture capture;
    capture.create(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT);
    capture.update(*Game::window);
    Game::window->display();
    return capture.copyToImage().getPixel(Config::SCREEN_WIDTH / 2, Config::SCREEN_HEIGHT / 2);
}
int main(int argc, char** argv)
{
    std::cout << std::unitbuf;
    int result = 0;
    try
    {
        Assets::initialize(argc ? argv[0] : "");
        Game::init(true);
        Graphics::init();
        Star::init();
        Explosion::init();
        auto* player = new Player();
        {
            BackgroundMusic music;
            MainMenu menu;
            GameOverMenu gameOver;
            HealthBar health;
            WeaponPair weapons;
            PlayerScore score;
            Star::create(128);
            Game::setState(Game::GAME);
            Enemy::create(1);
            check(count<Enemy>() == 1, "Enemy spawn failed");
            auto* enemy = *std::find_if(Graphics::instances.begin(), Graphics::instances.end(),
                [](Graphics* object) { return dynamic_cast<Enemy*>(object) != nullptr; });
            enemy->position = player->position + glm::vec3(0, 0, 400);
            enemy->size = 100;
            int defeated = 0, destroyed = 0;
            const auto enemyEvent = Enemy::onDestroy.Subscribe([&](Enemy*) { ++defeated; Enemy::create(2); });
            const auto destroyEvent = Graphics::onDestroy.Subscribe([&](Graphics* object) { if (object == enemy) ++destroyed; });
            auto* shot = new PlayerBullet();
            shot->position = enemy->position;
            tick();
            check(enemy->isPendingDestruction(), "Projectile did not hit enemy");
            enemy->Destroy(); // Duplicate marking must still cause one destruction.
            tick();
            check(defeated == 1 && destroyed == 1 && score.value() == 1 && count<Enemy>() == 2,
                  "Enemy destruction, replacement spawn or score was not exactly once");
            check(Explosion::count() == 1 && count<FireSprite>() == 5, "Explosion effect did not spawn");
            Enemy::onDestroy.Unsubscribe(enemyEvent);
            Graphics::onDestroy.Unsubscribe(destroyEvent);
            cleanWorldExceptPlayer();
            check(score.value() == 1, "Menu cleanup was counted as a kill");
            for (int frame = 0; frame < 500; ++frame) tick(0.05f);
            check(Explosion::count() == 0 && count<FireSprite>() == 0, "Effect lifetime did not finish");

            const auto originalPosition = player->position;
            player->shake();
            for (int frame = 0; frame < 12; ++frame) tick(0.05f);
            check(player->position == originalPosition, "Camera shake moved gameplay position");
            check(glm::length(player->worldToLocalPosition(originalPosition + glm::vec3(0, 0, 100)) - glm::vec3(0, 0, 100)) < 0.001f,
                  "Camera shake did not expire");
            Game::setState(Game::GAME);
            player->health = 1;
            auto* pill = new HealthPill();
            pill->position = player->position;
            tick(); tick();
            check(player->health == 2 && count<HealthPill>() == 0, "Pickup did not heal once and disappear");
            int shots = 0;
            const auto shootEvent = Player::onShoot.Subscribe([&](Player::ShootSide) { ++shots; });
            check(player->tryShoot() && !player->tryShoot(), "Player shooting cooldown is inconsistent");
            for (int frame = 0; frame < 5; ++frame) tick(0.05f);
            check(player->tryShoot() && shots == 2, "Player could not fire after cooldown");
            Player::onShoot.Unsubscribe(shootEvent);
            cleanWorldExceptPlayer();

            int deaths = 0;
            const auto deathEvent = player->onDestroy.Subscribe([&] { ++deaths; Game::setState(Game::GAME_OVER); Enemy::killInstances(); });
            Game::setState(Game::GAME);
            for (int hit = 0; hit < 2; ++hit)
            {
                auto* bullet = new Bullet();
                bullet->position = player->position;
                tick(); tick();
            }
            check(player->health == 0 && deaths == 1 && Game::getState() == Game::GAME_OVER, "Damage/game-over flow failed");
            Game::setState(Game::MENU);
            check(Game::getState() == Game::MENU, "Return-to-menu flow failed");
            player->onDestroy.Unsubscribe(deathEvent);

            Log::clear();
            Log::log("permanent"); Log::log("short", 0.1f); Log::log("long", 0.2f);
            Log::update(0.15f); check(Log::count() == 2, "Log expired the wrong entry");
            Log::update(0.1f); check(Log::count() == 1, "Log lifetime/permanent entry failed");
            std::vector<int> empty;
            bool rejected = false;
            try { getRandom(empty); } catch (const std::out_of_range&) { rejected = true; }
            check(rejected, "Empty random selection was not rejected");

            cleanWorldExceptPlayer();
            player->velocity = {};
            for (int frame = 0; frame < 12; ++frame) tick(0.05f);
            const auto start = player->position;
            for (int fps : {30, 60, 120})
            {
                player->position = start;
                player->velocity = {60, 0, 0};
                for (int frame = 0; frame < fps; ++frame) tick(1.0f / fps);
                check(glm::length(player->position - start - glm::vec3(60, 0, 0)) < 0.02f,
                      "Movement speed depends on frame rate");
            }
            player->velocity = {};
            Game::setState(Game::GAME);
            player->health = 1;
            auto* wrappedPickup = new HealthPill();
            wrappedPickup->position = player->position + glm::vec3(Config::FIELD_SIZE, 0, 0);
            tick(); tick();
            check(player->health == 2 && count<HealthPill>() == 0, "Collision did not wrap across the world seam");
            Game::setState(Game::MENU);
            auto* potato = new Potato();
            potato->position = player->position + glm::vec3(0, 0, 300);
            potato->velocity = {};
            potato->size = 200;
            tick();
            check(renderCenter() != sf::Color::Black, "Front-facing sprite did not render");
            potato->position = player->position - glm::vec3(0, 0, 300);
            tick();
            check(renderCenter() == sf::Color::Black, "Behind-camera sprite rendered in front");
            potato->position = player->position;
            tick();
            check(renderCenter() == sf::Color::Black, "Near-plane sprite was not hidden");
            Star::draw(); weapons.draw(); menu.draw(); gameOver.draw(); health.draw(); score.draw();
        }
        Game::onStateChange.Clear();
        Enemy::onDestroy.Clear();
        Player::onShoot.Clear();
        Explosion::shutdown();
        Graphics::shutdown();
        Star::shutdown();
        Log::clear();
        check(Graphics::instances.empty() && Player::main == nullptr, "World shutdown retained an entity");
        Game::shutdown();
        std::cout << "Runtime smoke passed: hits, score, effects, shake, pickups, damage, states, rendering and shutdown.\n";
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << '\n';
        Game::onStateChange.Clear(); Enemy::onDestroy.Clear(); Player::onShoot.Clear();
        Explosion::shutdown(); Graphics::shutdown(); Star::shutdown(); Log::clear(); Game::shutdown();
        result = 1;
    }
    return result;
}
