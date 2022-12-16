#pragma once
#include "GameObject/GameObject.h"
#include <glm/vec3.hpp>
#include "Client.h"
class Player
{
public:
	Player();
	~Player();
	GameObject* player;
	GameObject* bullet;
	glm::vec3 enemyTarget = glm::vec3(-500.f);
	glm::vec3 startPostion = glm::vec3(0.f);
	void Shoot(glm::vec3 target);
	void Update(float dt);
	Client* client;
	PlayerStateMessage* message;
	InputMessage* inputMessage;
};

