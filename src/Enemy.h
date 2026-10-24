#pragma once
#include "Graphics.h"
#include "Config.h"

class Enemy : public Graphics
{
    static constexpr glm::vec4 UV{0.03f, 0.5f, 0.28f, 0.78f};
    glm::vec3 velocity{0};
    float shotTimer = 0;
    void destroyFX();

public:
    Enemy();
    ~Enemy() override;
    static Event<Enemy*> onDestroy;
    static void create(int count);
    static void killInstances();
    glm::vec4 getUVs() const override { return UV; }
    void handleMovement();
    void handleShooting();
    bool shouldShoot() const;
    void shoot();
    void update() override;
};
