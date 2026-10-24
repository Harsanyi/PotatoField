#pragma once
#include "glm/glm.hpp"
#include "graphics.h"
#include "Bullet.h"

class Potato : public Graphics
{
	static constexpr glm::vec4 UV{ 0,0,0.5,0.5 };

public:
	Potato();

	float angularVelocity = 10;
	glm::vec3 velocity = { 0.,0.,0. };

	void update() override;

	glm::vec4 getUVs()const override{
		return UV;
	};

	static void create(const int& count);
};

