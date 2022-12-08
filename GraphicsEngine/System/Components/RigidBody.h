#pragma once
#include "iComponent.h"
#include <glm/vec3.hpp>

class RigidBody : public iComponent
{
public:
	RigidBody();
	glm::vec3 acceleration = glm::vec3(0.f);
	float mass = 1.f;

	virtual void Reset();;
	virtual void CopyComponent();
	virtual void GetParentValues();
	virtual void Update(PoRoSc porosc);

};

