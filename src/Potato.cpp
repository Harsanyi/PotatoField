#include "Potato.h"
#include "config.h"
#include "Random.h"

Potato::Potato() {
    position = glm::vec3(
        Random::getDouble(0, Config::FIELD_SIZE),
        Random::getDouble(0, Config::FIELD_SIZE),
        Random::getDouble(0, Config::FIELD_SIZE)
    );
    size = Config::POTATO_SIZE * Random::getDouble(1. / Config::POTATO_SIZE_VARIANCE, Config::POTATO_SIZE_VARIANCE);
    rotation = Random::getDouble(0, 360);
    angularVelocity = Random::getDouble(-Config::POTATO_ANGULAR_VELOCITY, Config::POTATO_ANGULAR_VELOCITY);
    velocity = {
        Random::getDouble(-Config::POTATO_SPEED, Config::POTATO_SPEED),
        Random::getDouble(-Config::POTATO_SPEED, Config::POTATO_SPEED),
        Random::getDouble(-Config::POTATO_SPEED, Config::POTATO_SPEED)
    };
}

void Potato::update()
{
	rotation += angularVelocity * Config::DELTA_TIME;
	rotation = fmod(rotation, 360.);
	position += velocity * Config::DELTA_TIME;
}

void Potato::create(const int& count)
{
    for (int i = 0; i < count; ++i) {
        auto pot = new Potato();
        pot->name = "Potato_" + std::to_string(i);
    }
}
