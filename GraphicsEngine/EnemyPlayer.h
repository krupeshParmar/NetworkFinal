#pragma once
#include "GameObject/GameObject.h"
#include <glm/vec3.hpp>
#include "Client.h"
class EnemyPlayer
{
public:
	EnemyPlayer();
	~EnemyPlayer();
	GameObject* object;
	GameObject* bullet;
	glm::vec3 enemyTarget = glm::vec3(-500.f);
	glm::vec3 startPostion = glm::vec3(0.f);
	double ID = -1;
	float px, pz;
	void Update(float dt);
	bool dead = false;
	//Client* client;
	PlayerStateMessage* message;
	InputMessage* inputMessage;
};

