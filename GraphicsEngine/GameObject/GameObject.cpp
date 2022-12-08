#include "GameObject.h"
#include <iostream>

void GameObject::Update()
{
	PoRoSc porosc;
	porosc.pos = transform->position;
	porosc.rot = transform->rotation;
	porosc.sca = transform->scale;

	if (parent != nullptr)
	{
		porosc.pos += parent->transform->position;
		porosc.rot += parent->transform->rotation;
	}
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->Update(porosc);
	}
}

void GameObject::OnCollisionEnter(GameObject* otherGameObject)
{
	if (this->name == "bullet")
	{
		this->collided = true;
		if (otherGameObject->name == "enemy")
		{
			otherGameObject->collided = true;
			otherGameObject->meshObject->bUse_RGBA_color = true;
			otherGameObject->meshObject->RGBA_color = glm::vec4(1.f, 1.f, 1.f, 1.f);
		}
	}
}

GameObject::GameObject()
{
	transform = new Transform();
	transform->Reset();
	parent = nullptr;
	this->name = "";
	this->meshObject = new MeshObject();
}


GameObject::GameObject(MeshObject* mesh)
{
	transform = new Transform();
	transform->Reset();
	parent = nullptr;
	this->meshObject = mesh;
}

GameObject::GameObject(MeshObject* mesh, Transform* trans)
{
	transform = new Transform();
	transform->Reset();
	parent = nullptr;
	this->meshObject = mesh;
	this->transform = trans;
}

GameObject::~GameObject()
{

}

void GameObject::CreateCopy(GameObject* gameObject)
{

}
