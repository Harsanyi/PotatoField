#include "FireSprite.h"
#include "Log.h"
#include "Random.h"
#include "chrono"

using namespace std::chrono;

FireSprite::FireSprite()
{
	maxSize = Random::getDouble(100,300);
	lifeTime = Random::getDouble(0.5,1);
	born = steady_clock::now();
}

void FireSprite::update()
{
	float life =glm::clamp(duration_cast<milliseconds>(steady_clock::now() - born).count() / (lifeTime * 1000.f),0.f,1.f);
	size = (0.5+life*0.5) * maxSize;

	if (life >= 1) {
		Destroy();
	}
}
