#pragma once
#include <SFML/Audio.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Config.h"
#include "Graphics.h"

class Player : public Graphics
{
    bool leftShoot = true;
    bool wasShootPressed = false;
    float shootCooldown = 0;
    float shakeRemaining = 0;
    glm::vec3 shakeOffset{0};
    std::vector<sf::SoundBuffer> shotBuffers;
    sf::SoundBuffer damageBuffer, healBuffer;
    sf::Sound leftShotSound, rightShotSound, damageSound, healSound;
    void handleInput();

public:
    enum ShootSide { LEFT, RIGHT };
    static Player* main;
    static Event<ShootSide> onShoot;
    glm::quat rotation{1, 0, 0, 0};
    glm::vec3 velocity{0};
    int health = Config::PLAYER_HEALTH;
    bool isControllable = false;
    Event<> onDestroy;

    Player();
    ~Player() override;
    void update() override;
    void move(glm::vec3 amount);
    void rotate(glm::vec3 eulerAngles);
    void shake();
    bool tryShoot();
    glm::vec3 worldToLocalPosition(const glm::vec3& worldPosition) const;
    glm::vec3 localToScreenPosition(const glm::vec3& localPosition) const;
    glm::vec3 worldToScreenPosition(const glm::vec3& worldPosition) const;
    void onOverlap(Graphics* other) override;
};
