#pragma once
#include "graphics.h"

class HealthPill : public Graphics
{
	const float rotationSpeed = 100;

	inline glm::vec4 getUVs()const override{
		return { 0.1045,0.875,0.1279,0.8975 };
	};

public:
	HealthPill();

	void update() override;

	bool isActiveCollider() override;
};

