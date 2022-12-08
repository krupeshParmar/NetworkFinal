#include "LightManager.h"

#include "../inc_opengl.h"
#include <sstream>
#include <iostream>

unsigned int LightManager::LIGHT_INDEX = 0;

LightManager::LightManager()
{
	/*for (unsigned int count = 0;
		count != LightManager::NUMBER_OF_LIGHTS_IN_USE;
		count++)
	{
		this->list_Lights.push_back(new Light());
	}*/
}

void LightManager::LoadLightUniformLocation(unsigned int shaderID)
{
	unsigned int lightCount = 0;
	std::string posStr = "position";
	std::string difStr = "diffuse";
	std::string speStr = "specular";
	std::string attStr = "atten";
	std::string dirStr = "direction";
	std::string pa1Str = "param1";
	std::string pa2Str = "param2";
	for (std::vector<Light*>::iterator itLight = this->list_Lights.begin();
		itLight != list_Lights.end();
		itLight++)
	{
		Light* light = *itLight;
		std::string lightsVariableName = "theLights[" + std::to_string(lightCount) + "].";
		light->position_uniformLocation = glGetUniformLocation(shaderID, (lightsVariableName + posStr).c_str());
		light->diffuse_uniformLocation = glGetUniformLocation(shaderID, (lightsVariableName + difStr).c_str());
		light->specular_uniformLocation = glGetUniformLocation(shaderID, (lightsVariableName + speStr).c_str());
		light->atten_uniformLocation = glGetUniformLocation(shaderID, (lightsVariableName + attStr).c_str());
		light->direction_uniformLocation = glGetUniformLocation(shaderID, (lightsVariableName + dirStr).c_str());
		light->param1_uniformLocation = glGetUniformLocation(shaderID, (lightsVariableName + pa1Str).c_str());
		light->param2_uniformLocation = glGetUniformLocation(shaderID, (lightsVariableName + pa2Str).c_str());
		lightCount++;
	}
}

void LightManager::CopyLightInformationToShader(unsigned int shaderID)
{
	for (std::vector<Light*>::iterator itLight = this->list_Lights.begin();
		itLight != list_Lights.end();
		itLight++)
	{
		Light* light = *itLight;
		glUniform4f(light->position_uniformLocation,
			light->position.x,
			light->position.y,
			light->position.z,
			light->position.w);

		glUniform4f(light->diffuse_uniformLocation,
			light->diffuse.x,
			light->diffuse.y,
			light->diffuse.z,
			light->diffuse.w);

		glUniform4f(light->specular_uniformLocation,
					light->specular.x,
					light->specular.y,
					light->specular.z,
					light->specular.w);

		glUniform4f(light->atten_uniformLocation,
					light->atten.x,
					light->atten.y,
					light->atten.z,
					light->atten.w);
					
		glUniform4f(light->direction_uniformLocation,
					light->direction.x,
					light->direction.y,
					light->direction.z,
					light->direction.w);
					
		glUniform4f(light->param1_uniformLocation,
					light->param1.x,
					light->param1.y,
					light->param1.z,
					light->param1.w);
					
		glUniform4f(light->param2_uniformLocation,
					light->param2.x,
					light->param2.y,
					light->param2.z,
					light->param2.w);


	}
}

void LightManager::AddNewLightInfo(Light* light)
{
	light->MyIndex = LIGHT_INDEX;
	list_Lights.push_back(light);
	LIGHT_INDEX++;
}
