#pragma once
#include "Graphics.h"

class FireSprite : public Graphics
{
    float lifetime = 1;
    float age = 0;
    float maxSize = 100;
public:
    FireSprite();
    glm::vec4 getUVs() const override { return {0.03f, 0.864f, 0.077f, 0.91f}; }
    void update() override;
};
