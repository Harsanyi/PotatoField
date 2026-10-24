#pragma once
#include "SFML/Graphics.hpp"
#include "glm/ext/quaternion_float.hpp"
#include "config.h"
#include "graphics.h"
#include "Event.h"
#include "chrono"
#include "SFML/Audio.hpp"

using namespace std::chrono;

class Player : public Graphics
{
	bool leftShoot = true;

	std::vector<sf::SoundBuffer> shotBuffers;
	sf::SoundBuffer damageBuffer;
	sf::Sound leftShotSound;
	sf::Sound rightShotSound;
	sf::Sound damageSound;
	sf::SoundBuffer healBuffer;
	sf::Sound healSound;

public:
	enum ShootSide {LEFT,RIGHT};

	static Player* main;
	static Event<ShootSide> onShoot;

	glm::quat rotation = { 1., 0, 0, 0 };
	glm::vec3 velocity = {};
	int health = Config::PLAYER_HEALTH;
	bool isControllable = true;
	bool isLastShoot = false;
	steady_clock::time_point shootTime;

	Event<> onDestroy;

	Player();

	void update() override;
	void move(glm::vec3 dir);
	void rotate(glm::vec3 axis);
	void shake();

	glm::vec3 world2LocalPos(const glm::vec3& worldPos);
	glm::vec3 local2ScreenPos(const glm::vec3& locPos);
	glm::vec3 world2ScreenPos(const glm::vec3& worldPos);
	void onOverlap(Graphics* other) override;

private:
	bool canShoot();
	void handleInput();
	void shoot();
};
