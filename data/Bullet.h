#pragma once
#include "graphics.h"
#include "glm/glm.hpp"
#include "chrono"

using namespace std::chrono;

class Bullet : public Graphics  
{
private:
	steady_clock::time_point born;

public:
	Bullet();

	glm::vec3 velocity = { 0,0,0 };

	glm::vec4 getUVs() const override {
		return { 0.03, 0.8, 0.08, 0.85 };
	};

	void update() override;

	bool isActiveCollider() override;

	void onOverlap(Graphics* other) override;
};

