#pragma once

#include "PhysicsObject.h"
#include "../GameObject/GameObject.h"

#include <vector>

class PhysicsDebugRenderer
{
public:
	PhysicsDebugRenderer();
	~PhysicsDebugRenderer();

	void Render();

	void AddPhysicsObject(GameObject* object);

private:
	std::vector<GameObject*> m_PhysicsObjects;
	unsigned int m_BoundingBoxModelId;
	unsigned int m_BoundingSphereModelId;
	unsigned int m_DebugShaderProgramId;
};