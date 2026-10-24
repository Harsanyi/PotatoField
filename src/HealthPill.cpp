#include "Game.h"
#include "HealthPill.h"
#include "Config.h"

HealthPill::HealthPill()
{
	size = 200;
}

void HealthPill::update()
{
	rotation += rotationSpeed * Game::deltaTime;
}

bool HealthPill::isActiveCollider()
{
	return true;
}
