#include "ShaderManager.h"

#include "../inc_opengl.h"

#include <fstream>
#include <sstream>
#include <vector>

#include <algorithm>
#include <iterator>

ShaderManager::ShaderManager()
{
	return;
}

ShaderManager::~ShaderManager()
{
	return;
}

bool ShaderManager::useShaderProgram(unsigned int ID)
{
	glUseProgram(ID);
	return true;
}

bool ShaderManager::useShaderProgram(std::string friendlyName)
{
	std::map<std::string, unsigned int>::iterator itShad = this->m_name_to_ID.find(friendlyName);

	if (itShad == this->m_name_to_ID.end())
		return false;

	glUseProgram(itShad->second);

	return true;
}

unsigned int ShaderManager::getIDFromFriendlyName(std::string friendlyName)
{
	std::map<std::string, unsigned int>::iterator itShad = this->m_name_to_ID.find(friendlyName);
	if (itShad == this->m_name_to_ID.end())
		return 0;
	return itShad->second;
}

ShaderManager::ShaderProgram* 
	ShaderManager::pGetShaderProgramFromFriendlyName(std::string friendlyName)
{
	unsigned int shaderID = this->getIDFromFriendlyName(friendlyName);

	std::map<unsigned int, ShaderProgram>::iterator itShad = this->m_ID_to_Shader.find(shaderID);

	if (itShad == this->m_ID_to_Shader.end())
		return NULL;

	return &(itShad->second);
}

const unsigned int MAXLINELENGTH = 65536;

void ShaderManager::setBasePath(std::string basePath)
{
	this->m_basepath = basePath;
	return;
}

bool ShaderManager::m_loadSourceFromFile(Shader& shader)
{
	std::string fullFileName = this->m_basepath + shader.filename;
	std::ifstream theFile(fullFileName.c_str());

	if (!theFile.is_open())
		return false;

	shader.vecSource.clear();

	char pLineTemp[MAXLINELENGTH] = { 0 };

	while (theFile.getline(pLineTemp, MAXLINELENGTH))
	{
		std::string tempString(pLineTemp);
		shader.vecSource.push_back(tempString);
	}
	theFile.close();
	return true;
}

bool ShaderManager::m_wasThereACompileError(unsigned int shaderID, std::string& errorText)
{
	errorText = "";

	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		char* pLogText = new char[maxLength];
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, pLogText);
		errorText.append(pLogText);

		this->m_lastError.append("\n");
		this->m_lastError.append(errorText);

		delete[] pLogText;
		return true;
	}
	return false;
}

bool ShaderManager::m_wasThereALinkerError(unsigned int programID, std::string& errorText)
{
	errorText = "";

	GLint wasError = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, &wasError);
	if (wasError == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

		char* pLogText = new char[maxLength];

		glGetProgramInfoLog(programID, maxLength, &maxLength, pLogText);
		errorText.append(pLogText);

		this->m_lastError.append("\n");
		this->m_lastError.append(errorText);

		delete[] pLogText;

		return true;
	}
	return false;
}

std::string ShaderManager::getLastError(void)
{
	std::string lastErrorTemp = this->m_lastError;
	this->m_lastError = "";
	return lastErrorTemp;
}

bool ShaderManager::m_compileShaderFromSource(ShaderManager::Shader& shader, std::string& error)
{
	error = "";

	const unsigned int MAXLINESIZE = 8 * 1024;

	unsigned int numberOfLines = static_cast<unsigned int>(shader.vecSource.size());

	char** arraySource = new char* [numberOfLines];

	memset(arraySource, 0, numberOfLines);

	for (unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++)
	{
		unsigned int numCharacters = (unsigned int)shader.vecSource[indexLine].length();

		arraySource[indexLine] = new char[numCharacters + 2];	// for the '\n' and '\0'
		memset(arraySource[indexLine], 0, numCharacters + 2);

		for (unsigned int indexChar = 0; indexChar != shader.vecSource[indexLine].length(); indexChar++)
		{
			arraySource[indexLine][indexChar] = shader.vecSource[indexLine][indexChar];
		}

		arraySource[indexLine][numCharacters + 0] = '\n';
		arraySource[indexLine][numCharacters + 1] = '\0';
	}

	glShaderSource(shader.ID, numberOfLines, arraySource, NULL);
	glCompileShader(shader.ID);

	for (unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++)
		delete[] arraySource[indexLine];

	delete[] arraySource;

	std::string errorText = "";
	if (this->m_wasThereACompileError(shader.ID, errorText))
	{
		std::stringstream ssError;
		ssError << shader.getShaderTypeString();
		ssError << " compile error: ";
		ssError << errorText;
		error = ssError.str();
		return false;
	}
	return true;
}

bool ShaderManager::createProgramFromFile(
	std::string friendlyName,
	Shader& vertexShader,
	Shader& fragmentShader
) {
	std::string errorText = "";
	printf("Compiling shaders...");

	vertexShader.ID = glCreateShader(GL_VERTEX_SHADER);
	vertexShader.shaderType = Shader::VERTEXT_SHADER;

	if (!this->m_loadSourceFromFile(vertexShader))
		return false;

	errorText = "";
	if (!this->m_compileShaderFromSource(vertexShader, errorText))
	{
		this->m_lastError = errorText;
		return false;
	}

	errorText = "";
	
	fragmentShader.ID = glCreateShader(GL_FRAGMENT_SHADER);
	fragmentShader.shaderType = Shader::FRAGMENT_SHADER;

	if (!this->m_loadSourceFromFile(fragmentShader))
		return false;

	if( ! this->m_compileShaderFromSource(fragmentShader, errorText))
	{
		this->m_lastError = errorText;
		return false;
	}

	ShaderProgram curProgram;
	curProgram.ID = glCreateProgram();

	glAttachShader(curProgram.ID, vertexShader.ID);
	glAttachShader(curProgram.ID, fragmentShader.ID);
	glLinkProgram(curProgram.ID);

	errorText = "";

	if (this->m_wasThereALinkerError(curProgram.ID, errorText))
	{
		std::stringstream ssError;
		ssError << "Shader program link error";
		ssError << errorText;
		this->m_lastError = ssError.str();
		return false;
	}

	curProgram.friendlyName = friendlyName;

	this->m_ID_to_Shader[curProgram.ID] = curProgram;
	this->m_name_to_ID[curProgram.friendlyName] = curProgram.ID;

	return true;
}
