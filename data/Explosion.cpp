#include "Explosion.h"
#include "FireSprite.h"
#include "Random.h"
#include "Player.h"

void Explosion::startLifeCheck()
{
	lifeThread = std::thread([&](){
		while (true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));

			if (sound.getStatus() == sf::Sound::Status::Playing)continue;
			bool haveSprite = false;
			for (auto sprite : sprites) {
				if (sprite) {
					haveSprite = true;
					break;
				}
			}
			if (haveSprite)continue;

			delete(this);
			break;
		}
		});
}

Explosion::Explosion(glm::vec3 pos)
{
	this->position = pos;

	const int offset = 20;
	for (int i = 0; i < 5; i++) {
		auto fireFX = new FireSprite();
		fireFX->position = this->position + glm::vec3(Random::getDouble(-offset, offset), Random::getDouble(-offset, offset), Random::getDouble(-offset, offset));
		fireFX->size = 100;

		sprites.push_back(fireFX);
	}

	soundBuffer.loadFromFile("audio/explosion.wav");
	sound.setBuffer(soundBuffer);
	sound.setRelativeToListener(true);
	glm::vec3 audioPos = Player::main->position - this->position;
	sound.setPosition({audioPos.x, audioPos.y, audioPos.z});
	sound.setPitch(Random::getDouble(0.7,1.));
	sound.play();

	startLifeCheck();
}
