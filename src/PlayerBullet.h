#pragma once
#include "Bullet.h"

class PlayerBullet : public Bullet
{
public:
	glm::vec4 getUVs() const override {
		return { 0.09, 0.8, 0.14, 0.85 };
	};

	bool isActiveCollider() override;
	void onOverlap(Graphics* other) override;
};

