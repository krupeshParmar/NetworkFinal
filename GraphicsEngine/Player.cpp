#include "Player.h"

Player::Player()
{
	client = new Client();
	client->Initialize();
	client->SetServerAddressAndPort("127.0.0.1", 5555);
}

Player::~Player()
{
}

void Player::Shoot(glm::vec3 target)
{
	this->startPostion = this->player->transform->position;
	this->enemyTarget = target;
}

void Player::Update(float dt)
{
	this->client->CheckForUpdateFromGameServer();
	this->player->transform->position.x += client->px / 16.f;
	this->player->transform->position.z += client->pz / 16.f;
	this->bullet->transform->position.x += client->bx / 8.f;
	this->bullet->transform->position.z += client->bz / 8.f;
}
