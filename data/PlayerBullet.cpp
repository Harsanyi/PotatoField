#include "PlayerBullet.h"
#include "Log.h"
#include "Enemy.h"

bool PlayerBullet::isActiveCollider()
{
	return true;
}

void PlayerBullet::onOverlap(Graphics* other)
{
	if (dynamic_cast<Enemy*>(other)) {
		other->Destroy();
		Destroy();
	}
}
