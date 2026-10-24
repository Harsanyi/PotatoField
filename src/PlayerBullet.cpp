#include "PlayerBullet.h"
#include "Enemy.h"

bool PlayerBullet::isActiveCollider()
{
	return Bullet::isActiveCollider();
}

void PlayerBullet::onOverlap(Graphics* other)
{
	if (dynamic_cast<Enemy*>(other)) {
		other->Destroy();
		Destroy();
	}
}
