#pragma once
#include "GameObject/GameObject.h"
#include <glm/vec3.hpp>
class Player
{
public:
	GameObject* frontCollider;
	GameObject*	backCollider;
	GameObject* rightCollider;
	GameObject* leftCollider;
	GameObject* player;
	GameObject* bullet;
	glm::vec3 enemyTarget = glm::vec3(-500.f);
	glm::vec3 startPostion = glm::vec3(0.f);
	void Shoot(glm::vec3 target);
	void Update(float dt);
};

