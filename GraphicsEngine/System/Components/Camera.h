#pragma once
#include "iComponent.h"
#include <glm/glm.hpp>
#include "Transform.h"

#define UP glm::vec3(0.0f, 1.0f, 0.0f);

class Camera : public iComponent
{
public:
	// to be change with an enums
	int ProjectionType;
	int FOV_Axis;
	
	float FieldOfView;
	float nearPlane, farPlane;
	int priority;

	glm::vec3 cameraTarget;
	glm::vec3 cameraDirection;
	glm::vec3 cameraRight;

	// camera Attributes
	const glm::vec3 Front = glm::vec3(0.f, 0.f, -1.f);
	const glm::vec3 Up = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// euler Angles
	float Yaw;
	float Pitch;
	// camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	virtual void Reset();
	virtual void CopyComponent();
	virtual void GetParentValues(); 
	virtual void Update(PoRoSc porosc);
};
