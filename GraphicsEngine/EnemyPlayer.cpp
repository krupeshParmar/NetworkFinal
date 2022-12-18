#include "EnemyPlayer.h"

EnemyPlayer::EnemyPlayer()
{
}

EnemyPlayer::~EnemyPlayer()
{
}

void EnemyPlayer::Update(float dt)
{
	glm::vec3 clientPos = glm::vec3(
		this->px,
		this->object->transform->position.y,
		this->pz);
	if (this->object == nullptr)
		return;

	if (this->dead)
	{	
		this->object->transform->position = clientPos;
		this->dead = false;
		return;
	}
	glm::vec3 difference = clientPos - this->object->transform->position;
	this->object->transform->position += difference / 54.f;
}
