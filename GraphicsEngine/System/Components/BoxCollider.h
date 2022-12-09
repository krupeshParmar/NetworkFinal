#pragma once
#include "iComponent.h"
#include <string>
#include <glm/vec3.hpp>
class BoxCollider : public iComponent
{
public:
	std::string box_model_path;
	glm::vec3 minPosition, maxPosition;
	BoxCollider();
	void Update();
	virtual void Reset();
	virtual void CopyComponent();
	virtual void GetParentValues();
	virtual void Update(PoRoSc porosc);

};

