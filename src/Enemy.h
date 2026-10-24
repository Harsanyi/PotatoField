#pragma once
#include "graphics.h"
#include "glm/glm.hpp"
#include "Player.h"
#include "chrono"
#include "Bullet.h"

using namespace std::chrono;

class Enemy : public Graphics
{
	static constexpr glm::vec4 UV{ 0.03 , 0.5, 0.28, 0.78 };

	glm::vec3 velocity = {0,0,0};
	steady_clock::time_point lastShoot = steady_clock::now();

	void destroyFX();

public:
	~Enemy() override;
	Enemy();

	static Event<Enemy*> onDestroy;

	static void create(const int& count);

	static void killInstances();

	inline glm::vec4 getUVs() const override {
		return UV;
	};

	void handleMovement();

	void handleShooting();

	bool isShouldShoot();

	void shoot();

	void update() override;
};

