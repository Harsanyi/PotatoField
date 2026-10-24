#pragma once
#include "Player.h"
#include "SFML/Graphics.hpp"
#include "cmath"
#include "Config.h"
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "Bullet.h"
#include <iostream>
#include "Log.h"
#include "chrono"
#include "PlayerBullet.h"
#include "SFML/Audio.hpp"
#include "iostream"
#include "fstream"
#include "Random.h"
#include "thread"
#include "HealthPill.h"

using namespace std::chrono;

Player* Player::main = nullptr;
const glm::vec3 LEFT_OFFSET(-60,50,50);
const glm::vec3 RIGHT_OFFSET(60,50,50);

Event<Player::ShootSide> Player::onShoot;

Player::Player()
{
	main = this;
	this->size = 100;
	name = "Player";
	position = { Config::HALF_FIELD_SIZE,Config::HALF_FIELD_SIZE,Config::HALF_FIELD_SIZE };

	sf::SoundBuffer* buffer = new sf::SoundBuffer();
	buffer->loadFromFile("audio/shot1.wav");
	shotBuffers.emplace_back(*buffer);

	buffer = new sf::SoundBuffer();
	buffer->loadFromFile("audio/shot2.wav");
	shotBuffers.emplace_back(*buffer);

	buffer = new sf::SoundBuffer();
	buffer->loadFromFile("audio/shot3.wav");
	shotBuffers.emplace_back(*buffer);

	buffer = new sf::SoundBuffer();
	buffer->loadFromFile("audio/shot4.wav");
	shotBuffers.emplace_back(*buffer);

	buffer = new sf::SoundBuffer();
	buffer->loadFromFile("audio/shot5.wav");
	shotBuffers.emplace_back(*buffer);

	leftShotSound.setRelativeToListener(true);
	leftShotSound.setPosition(sf::Vector3f(-0.8, -0.1, 0.1));
	rightShotSound.setRelativeToListener(true);
	rightShotSound.setPosition(sf::Vector3f(0.8, -0.1, 0.1));

	damageBuffer.loadFromFile("audio/got_damage.wav");
	damageSound.setBuffer(damageBuffer);

	healBuffer.loadFromFile("audio/pickup.wav");
	healSound.setBuffer(healBuffer);
}

void Player::update()
{
	handleInput();
	move(velocity * Config::DELTA_TIME);
}

void Player::move(glm::vec3 amount)
{
	this->position += amount;
}

void Player::rotate(glm::vec3 eulerAngles)
{
	rotation = this->rotation * glm::quat(eulerAngles);
}

void Player::shake()
{
	std::thread thread([&]() {
		time_point start = steady_clock::now();
		int elapsed = 0;
		const int shakeTime = 400;
		const int minSleep = 1;
		const int maxSleep = 25;
		const int maxOffset = 50;
		while (this && elapsed < shakeTime) {
			this->position += glm::vec3(
				Random::getDouble(-maxOffset,maxOffset),
				Random::getDouble(-maxOffset, maxOffset),
				Random::getDouble(-maxOffset, maxOffset)
			);
		
			std::this_thread::sleep_for(milliseconds((long)Random::getDouble(minSleep,maxSleep)));
		
			elapsed = duration_cast<milliseconds>(steady_clock::now() - start).count();
		}
		});
	thread.detach();
}

glm::vec3 Player::world2LocalPos(const glm::vec3& worldPos)
{
	glm::vec3 localPos;
	glm::vec3 camPos;

	camPos = this->position;
	camPos = {
		fmod(this->position.x, Config::FIELD_SIZE),
		fmod(this->position.y, Config::FIELD_SIZE),
		fmod(this->position.z, Config::FIELD_SIZE)
	};
	if (camPos.x < 0)camPos.x += Config::FIELD_SIZE;
	if (camPos.y < 0)camPos.y += Config::FIELD_SIZE;
	if (camPos.z < 0)camPos.z += Config::FIELD_SIZE;

	localPos = worldPos - camPos;

	if (localPos.x < -Config::HALF_FIELD_SIZE)localPos.x += Config::FIELD_SIZE;
	if (localPos.x > Config::HALF_FIELD_SIZE)localPos.x -= Config::FIELD_SIZE;
	if (localPos.y < -Config::HALF_FIELD_SIZE)localPos.y += Config::FIELD_SIZE;
	if (localPos.y > Config::HALF_FIELD_SIZE)localPos.y -= Config::FIELD_SIZE;
	if (localPos.z < -Config::HALF_FIELD_SIZE)localPos.z += Config::FIELD_SIZE;
	if (localPos.z > Config::HALF_FIELD_SIZE)localPos.z -= Config::FIELD_SIZE;

	localPos = glm::inverse(this->rotation) * localPos;

	return localPos;
}

glm::vec3 Player::local2ScreenPos(const glm::vec3& locPos)
{	
	float depth = locPos.z;
	if (depth < 1) {
		depth = 1;
	}

	float fovRad = Config::CAMERA_H_FOV * (3.141592654 / 180.);
	float scale = 1. / std::tanf(fovRad / 2.) * depth;

	float x = locPos.x / scale * 0.5 * Config::SCREEN_WIDTH;
	float y = locPos.y / scale * 0.5 * Config::SCREEN_HEIGHT;

	return glm::vec3(x, y, scale);
}

glm::vec3 Player::world2ScreenPos(const glm::vec3& worldPos)
{
	return local2ScreenPos(world2LocalPos(worldPos));
}

void Player::onOverlap(Graphics* other)
{
	if (dynamic_cast<PlayerBullet*>(other)) {
		return;
	}

	if (dynamic_cast<Bullet*>(other)) {
		damageSound.play();
		shake();

		if (!Config::PLAYER_INVINCIBLE) {
			health--;
		}
		std::cout << "Player Health: " << health << std::endl;
		if (health <= 0) {
			onDestroy.Invoke();			
		}
		return;
	}

	if (dynamic_cast<HealthPill*>(other)) {
		health++;
		healSound.play();
		other->Destroy();

		return;
	}
}

bool Player::canShoot()
{
	return duration_cast<milliseconds>(steady_clock::now()-shootTime).count() >= Config::PLAYER_SHOOTRATE * 1000;
}

void Player::handleInput()
{
	if (!isControllable)return;

	sf::Vector2i screenCenter = sf::Vector2i(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT) / 2;
	sf::Vector2i deltaMousePos = sf::Mouse::getPosition() - screenCenter;
	sf::Mouse::setPosition(screenCenter);

	float rotateSpeed = Config::DELTA_TIME * Config::CAMERA_ROTATION_SPEED;
	rotate(glm::vec3(-deltaMousePos.y * rotateSpeed * Config::DELTA_TIME, deltaMousePos.x * rotateSpeed * Config::DELTA_TIME, 0.));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		velocity += rotation * glm::vec3(0, 0, Config::CAMERA_MOVE_SPEED * Config::DELTA_TIME);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		velocity += rotation * glm::vec3(0, 0, -Config::CAMERA_MOVE_SPEED * Config::DELTA_TIME);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		velocity += rotation * glm::vec3(-Config::CAMERA_MOVE_SPEED * Config::DELTA_TIME, 0, 0);
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		velocity += rotation * glm::vec3(Config::CAMERA_MOVE_SPEED * Config::DELTA_TIME, 0, 0);
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		velocity += rotation * glm::vec3(0, -Config::CAMERA_MOVE_SPEED * Config::DELTA_TIME, 0);
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		velocity += rotation * glm::vec3(0, Config::CAMERA_MOVE_SPEED * Config::DELTA_TIME, 0);
	}

	if (isLastShoot != sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		isLastShoot = !isLastShoot;
		if (isLastShoot && canShoot()) {
			shoot();
		}
	}

	float speed = glm::length(velocity);
	if (speed > Config::CAMERA_MOVE_SPEED) {
		velocity /= speed / Config::CAMERA_MOVE_SPEED;
	}
}

void Player::shoot()
{
	shootTime = steady_clock::now();

	PlayerBullet* bullet = new PlayerBullet();
	bullet->position = this->position + (this->rotation * (leftShoot ? LEFT_OFFSET : RIGHT_OFFSET));
	bullet->size = Config::BULLET_SIZE;
	bullet->velocity = rotation * glm::vec3(0.,0.,1.) * Config::BULLET_SPEED + this->velocity;

	if (leftShoot) {
		leftShotSound.setBuffer(getRandom(shotBuffers));
		leftShotSound.setPitch(Random::getDouble(0.7, 1));
		leftShotSound.play();
	}
	else {
		rightShotSound.setBuffer(getRandom(shotBuffers));
		rightShotSound.setPitch(Random::getDouble(0.7, 1));
		rightShotSound.play();
	}

	onShoot.Invoke(leftShoot ? Player::LEFT : Player::RIGHT);
	leftShoot = !leftShoot;
}
