#include "Light.h"

Light::Light()
{
	componentType = "light";
	this->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->atten = glm::vec4(0.01f, 0.01f, 0.0f, 1.0f);
	this->direction = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	this->param1 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->param2 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

void Light::SetConstantAttenuation(float newConstAtten)
{
	this->atten.x = newConstAtten;
	return;
}

void Light::SetLinearAttenuation(float newLinearAtten)
{
	this->atten.y = newLinearAtten;
	return;
}

void Light::SetQuadraticAttenuation(float newQuadraticAtten)
{
	this->atten.z = newQuadraticAtten;
	return;
}

void Light::TurnON(void)
{
	this->param2.x = 1;
}

void Light::TurnOFF(void)
{
	this->param2.x = 0;
}

void Light::Reset()
{
}

void Light::CopyComponent()
{
}

void Light::GetParentValues()
{
	
}

void Light::Update(PoRoSc porosc)
{
	position = glm::vec4(porosc.pos,1.f);
	direction = glm::vec4(porosc.rot, 1.f);
}
