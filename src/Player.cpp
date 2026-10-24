#include "Player.h"
#include "Assets.h"
#include "Game.h"
#include "HealthPill.h"
#include "PlayerBullet.h"
#include "Random.h"
#include "Utils.h"
#include "WorldMath.h"
#include <algorithm>
#include <stdexcept>

Player* Player::main = nullptr;
Event<Player::ShootSide> Player::onShoot;
namespace
{
    const glm::vec3 leftOffset{-60, 50, 50};
    const glm::vec3 rightOffset{60, 50, 50};
}

Player::Player()
{
    size = 100;
    name = "Player";
    position = glm::vec3(Config::HALF_FIELD_SIZE);
    shotBuffers.resize(5);
    for (std::size_t i = 0; i < shotBuffers.size(); ++i)
        if (!shotBuffers[i].loadFromFile(Assets::file("audio/shot" + std::to_string(i + 1) + ".wav")))
            throw std::runtime_error("Cannot load shot sound");
    if (!damageBuffer.loadFromFile(Assets::file("audio/got_damage.wav")) ||
        !healBuffer.loadFromFile(Assets::file("audio/pickup.wav")))
        throw std::runtime_error("Cannot load player damage/pickup sound");
    leftShotSound.setRelativeToListener(true);
    leftShotSound.setPosition(-0.8f, -0.1f, 0.1f);
    rightShotSound.setRelativeToListener(true);
    rightShotSound.setPosition(0.8f, -0.1f, 0.1f);
    damageSound.setBuffer(damageBuffer);
    healSound.setBuffer(healBuffer);
    main = this;
}

Player::~Player()
{
    if (main == this) main = nullptr;
}

void Player::update()
{
    shootCooldown = std::max(0.0f, shootCooldown - Game::deltaTime);
    handleInput();
    move(velocity * Game::deltaTime);
    shakeRemaining = std::max(0.0f, shakeRemaining - Game::deltaTime);
    if (shakeRemaining > 0)
    {
        const float amplitude = 25.0f * shakeRemaining / 0.4f;
        shakeOffset = {Random::getDouble(-amplitude, amplitude), Random::getDouble(-amplitude, amplitude),
                       Random::getDouble(-amplitude, amplitude)};
    }
    else shakeOffset = glm::vec3(0);
}

void Player::move(glm::vec3 amount) { position += amount; }
void Player::rotate(glm::vec3 angles) { rotation = glm::normalize(rotation * glm::quat(angles)); }
void Player::shake() { shakeRemaining = 0.4f; }

glm::vec3 Player::worldToLocalPosition(const glm::vec3& world) const
{
    return cameraLocalPosition(world, position, rotation) - shakeOffset;
}
glm::vec3 Player::localToScreenPosition(const glm::vec3& local) const { return projectToScreen(local); }
glm::vec3 Player::worldToScreenPosition(const glm::vec3& world) const
{
    return localToScreenPosition(worldToLocalPosition(world));
}

void Player::onOverlap(Graphics* other)
{
    if (Game::getState() != Game::GAME || health <= 0 || dynamic_cast<PlayerBullet*>(other)) return;
    if (dynamic_cast<Bullet*>(other))
    {
        damageSound.play();
        shake();
        if (!Config::PLAYER_INVINCIBLE) --health;
        if (health <= 0) onDestroy.Invoke();
    }
    else if (dynamic_cast<HealthPill*>(other))
    {
        ++health;
        healSound.play();
        other->Destroy();
    }
}

void Player::handleInput()
{
    if (!isControllable || !Game::window->hasFocus()) return;
    const sf::Vector2i center{Config::SCREEN_WIDTH / 2, Config::SCREEN_HEIGHT / 2};
    const auto mouse = sf::Mouse::getPosition(*Game::window) - center;
    sf::Mouse::setPosition(center, *Game::window);
    // Mouse delta is already per-frame displacement: keep pixel sensitivity independent of frame duration.
    rotate({-mouse.y * Config::MOUSE_SENSITIVITY, mouse.x * Config::MOUSE_SENSITIVITY, 0});
    glm::vec3 input{0};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) input.z += 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) input.z -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) input.x -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) input.x += 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) input.y -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) input.y += 1;
    velocity += rotation * input * Config::CAMERA_MOVE_SPEED * Game::deltaTime;
    const float speed = glm::length(velocity);
    if (speed > Config::CAMERA_MOVE_SPEED) velocity *= Config::CAMERA_MOVE_SPEED / speed;
    const bool pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    if (pressed && !wasShootPressed) tryShoot();
    wasShootPressed = pressed;
}

bool Player::tryShoot()
{
    if (Game::getState() != Game::GAME || health <= 0 || shootCooldown > 0) return false;
    shootCooldown = Config::PLAYER_SHOOTRATE;
    auto* bullet = new PlayerBullet();
    bullet->position = position + rotation * (leftShoot ? leftOffset : rightOffset);
    bullet->size = Config::BULLET_SIZE;
    bullet->velocity = rotation * glm::vec3(0, 0, Config::BULLET_SPEED) + velocity;
    auto& sound = leftShoot ? leftShotSound : rightShotSound;
    sound.setBuffer(getRandom(shotBuffers));
    sound.setPitch(static_cast<float>(Random::getDouble(0.7, 1)));
    sound.play();
    onShoot.Invoke(leftShoot ? LEFT : RIGHT);
    leftShoot = !leftShoot;
    return true;
}
