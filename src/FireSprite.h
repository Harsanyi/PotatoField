#pragma once
#include "graphics.h"
#include "chrono"

using namespace std::chrono;

class FireSprite : public Graphics
{
	float lifeTime = 1;
	steady_clock::time_point born;
	float maxSize = 100;

public:
	FireSprite();

	inline virtual glm::vec4 getUVs() const override{
		return { 0.03,0.864,0.077,0.91 };
	};

	void update() override;
};

