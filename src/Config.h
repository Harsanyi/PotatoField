#pragma once
class Config
{
public:
	static constexpr int FRAMERATE = 120;
	static constexpr int SCREEN_WIDTH = 1024;
	static constexpr int SCREEN_HEIGHT = 768;
	static constexpr float POTATO_SIZE = 256;
	static constexpr int POTATO_COUNT = 1000;
	static constexpr float CAMERA_MOVE_SPEED = 1000;
	static constexpr float CAMERA_ROTATION_SPEED = 30;
	static constexpr float FIELD_SIZE = 10000;
	static constexpr float CAMERA_H_FOV = 60;
	static constexpr float DEPTH_SCALE = 100;
	static constexpr float POTATO_ANGULAR_VELOCITY = 100;
	static constexpr float POTATO_SIZE_VARIANCE = 1.5;
	static constexpr float POTATO_SPEED = 10;
	static constexpr int LOG_FONT_SIZE = 12;

	static constexpr float HALF_FIELD_SIZE = FIELD_SIZE * 0.5;
	static constexpr float DELTA_TIME = 1. / FRAMERATE;

	static constexpr float ENEMY_SPEED = 750;
	static constexpr float ENEMY_SIZE = 200;
	static constexpr float ENEMY_SHOOTING_FREQ = 2;

	static constexpr float BULLET_SIZE = 100;
	static constexpr float BULLET_SPEED = 2000; 
	static constexpr float BULLET_LIFETIME = 0.5;

	static constexpr float LIGHT_FALL_RATE = 5;

	static constexpr int PLAYER_HEALTH = 3;
	static constexpr bool PLAYER_INVINCIBLE = false;
	static constexpr float PLAYER_SHOOTRATE = 0.2;

	static constexpr int STAR_COUNT = 10000;
	static constexpr int MAX_STAR_SIZE = 10;

	static constexpr float PILL_SPAWN_RATE = 0.5;
};

