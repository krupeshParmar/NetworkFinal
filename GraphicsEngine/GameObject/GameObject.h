#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <map>
#include "../MeshObject/MeshObject.h"
#include "../System/Components/iComponent.h"
#include "../System/Components/Transform.h"

class GameObject
{
public:
	// can be protected
	bool enabled = true;
	Transform* transform;
	MeshObject* meshObject;
	GameObject* parent;
	std::vector<GameObject*> children;
	std::vector<iComponent*> components;
	glm::mat4x4 matModel = glm::mat4x4(1.f);

	void SetPosition(glm::vec3 position);
	void SetRotation(glm::vec3 rotation);
	void SetScale(glm::vec3 scale);
	// ----

	std::string name;

	void Update();
	void OnCollisionEnter(GameObject* otherGameObject);

	GameObject();
	GameObject(MeshObject* mesh);
	GameObject(MeshObject* mesh, Transform* trans);
	~GameObject();
	void CreateCopy(GameObject* gameObject);
	void GetPosition(glm::vec3* position);
	void GetRotation(glm::vec3* rotation);
	void GetScale(glm::vec3* scale);

	// physics project stuff
	bool collided = false;

};

