#include "Player.h"
#define XMIN -37
#define XMAX 37
#define ZMIN -2
#define ZMAX 33

Player::Player()
{
	client = new Client();
	client->Initialize();
	client->SetServerAddressAndPort("127.0.0.1", 5555);
	message = new PlayerStateMessage();
	inputMessage = new InputMessage();
	client->px = 0.f;
	client->pz = 0.f;
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
	/*if (this->client->waitForInputToBeSentToServer)
		return;*/
	this->client->waitForInputToBeSentToServer = true;
	
	glm::vec3 clientPos = glm::vec3(
		client->px,
		2.f, 
		client->pz);
	glm::vec3 difference = clientPos - this->player->transform->position;
	if (difference.x < 0.5f && difference.z < 0.5f)
	{
		this->client->px = this->player->transform->position.x;
		this->client->pz = this->player->transform->position.z;
		return;
	}
	//this->player->transform->position = glm::vec3(client->px, 2.f, client->pz);
	this->player->transform->position += difference / 50.f;
	/*if (this->player->transform->position.x > XMAX)
		this->player->transform->position.x = XMAX;

	if (this->player->transform->position.x < XMIN)
		this->player->transform->position.x = XMIN;

	if (this->player->transform->position.z > ZMAX)
		this->player->transform->position.z = ZMAX;

	if (this->player->transform->position.z < ZMIN)
		this->player->transform->position.z = ZMIN;*/
	/*if(this->player->transform->position.x != client->px)
		this->player->transform->position.x += client->px / 16.f;
	if (this->player->transform->position.z != client->pz)
		this->player->transform->position.z += client->pz / 16.f;

	if (this->bullet->transform->position.x != client->bx)
		this->bullet->transform->position.x += client->bx / 8.f;

	if (this->bullet->transform->position.z != client->bz)
		this->bullet->transform->position.z += client->bz / 8.f;*/
}
