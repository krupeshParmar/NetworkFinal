#pragma once
#include <glm/vec3.hpp>
#include "../Components/iComponent.h"

class Transform : public iComponent
{
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	Transform();
	~Transform();

	void operator=(Transform& transform);
	virtual void Reset();
	virtual void CopyComponent();
	virtual void GetParentValues();
	virtual void Update(PoRoSc porosc);
};