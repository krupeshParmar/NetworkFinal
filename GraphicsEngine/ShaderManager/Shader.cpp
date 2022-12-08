#include "ShaderManager.h"
#include "../inc_opengl.h"

ShaderManager::Shader::Shader()
{
	this->ID = 0;
	this->shaderType = Shader::UNKNOWN;
	return;
}

ShaderManager::Shader::~Shader()
{

}

std::string ShaderManager::Shader::getShaderTypeString(void)
{
	switch (this->shaderType)
	{
	case Shader::VERTEXT_SHADER:
		return "VERTEXT_SHADER";
	case Shader::FRAGMENT_SHADER:
		return "FRAGMENT_SHADER";
	case Shader::UNKNOWN:
		return "UNKNOWN";
	default:
		return "";
	}
}

bool ShaderManager::ShaderProgram::LoadUniformLocation(std::string variableName)
{
	GLint unilocation = glGetUniformLocation(this->ID, variableName.c_str());

	if (unilocation == -1)
		return false;

	this->mapUniformName_to_UniformLocation[variableName.c_str()] = unilocation;

	return true;
}

ShaderManager::ShaderProgram::ShaderProgram()
{
	return;
}

ShaderManager::ShaderProgram::~ShaderProgram()
{
}

int ShaderManager::ShaderProgram::getUniformID_From_Name(std::string name)
{
	std::map<std::string, int>::iterator itUniform = this->mapUniformName_to_UniformLocation.find(name);

	if (itUniform == this->mapUniformName_to_UniformLocation.end())
		return -1;

	return itUniform->second;

}
