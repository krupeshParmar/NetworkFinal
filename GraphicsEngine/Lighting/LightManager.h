#pragma once
#include <vector>
#include "Light.h"
class LightManager
{
public:
	LightManager();

	static const unsigned int NUMBER_OF_LIGHTS_IN_USE = 10;
	static unsigned int LIGHT_INDEX;

	std::vector<Light*> list_Lights;

	void LoadLightUniformLocation(unsigned int shaderID);
	void CopyLightInformationToShader(unsigned int shaderID);
	void AddNewLightInfo(Light* light);
};

