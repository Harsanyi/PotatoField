#include "HealthPill.h"
#include "config.h"

HealthPill::HealthPill()
{
	size = 200;
}

void HealthPill::update()
{
	rotation += rotationSpeed * Config::DELTA_TIME;
}

bool HealthPill::isActiveCollider()
{
	return true;
}
