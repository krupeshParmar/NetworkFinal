#include "Transform.h"

Transform::Transform()
{
	position = glm::vec3(0.f, 0.f, 0.f);
	rotation = glm::vec3(0.f, 0.f, 0.f);
	scale = glm::vec3(1.f, 1.f, 1.f);
}

Transform::~Transform()
{
}

void Transform::operator=(Transform& transform)
{
	this->position = transform.position;
	this->rotation = transform.rotation;
	this->scale = transform.scale;
}

void Transform::Reset()
{
	position = glm::vec3(0.f, 0.f, 0.f);
	rotation = glm::vec3(0.f, 0.f, 0.f);
	scale = glm::vec3(1.f, 1.f, 1.f);
}

void Transform::CopyComponent()
{

}

void Transform::GetParentValues()
{

}

void Transform::Update(PoRoSc porosc)
{
}
