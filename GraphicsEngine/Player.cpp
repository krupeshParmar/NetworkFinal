#include "Player.h"

void Player::Shoot(glm::vec3 target)
{
	this->startPostion = this->player->transform->position;
	this->enemyTarget = target;
}

void Player::Update(float dt)
{
	if (enemyTarget != glm::vec3(-500.f))
	{
		if (bullet->collided)
		{
			bullet->transform->position = startPostion + glm::vec3(0.f, -0.7f,0.f);
			enemyTarget = glm::vec3(-500.f);
			bullet->collided = false;
			return;
		}
		glm::vec3 direction = enemyTarget - player->transform->position;
		bullet->transform->position += direction * dt;
	}
}
