#pragma once
#include "Graphics.h"
class Bullet : public Graphics
{
    float age = 0;
public:
    glm::vec3 velocity{0};
    glm::vec4 getUVs() const override { return {0.03f, 0.8f, 0.08f, 0.85f}; }
    void update() override;
    bool isActiveCollider() override;
    void onOverlap(Graphics* other) override;
};
