#include "MeshObject.h"

MeshObject::MeshObject()
{
	this->rotation = glm::vec3(0.0f);
	this->position = glm::vec3(0.0f);
	this->friendlyName = "";
	this->meshName = "";
	this->path = "";
	this->scale = 1.0f;
	this->isWireframe = false;

	this->RGBA_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->bUse_RGBA_color = false;
	this->bDoNotLight = false;
}