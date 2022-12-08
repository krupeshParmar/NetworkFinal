#pragma once

#include <string>
#include <vector>
#include <map>

class ShaderManager
{
public:
	class Shader {
	public:
		Shader();
		~Shader();
		enum SHADER_TYPE {
			VERTEXT_SHADER,
			FRAGMENT_SHADER,
			UNKNOWN,
		};
		SHADER_TYPE shaderType;
		std::string getShaderTypeString(void);

		unsigned int ID;
		std::vector<std::string> vecSource;
		bool bSourceIsMultiLine;
		std::string filename;
	};

	class ShaderProgram{
	public:
		ShaderProgram();
		~ShaderProgram();

		unsigned int ID;
		std::string friendlyName;

		std::map<std::string, int> mapUniformName_to_UniformLocation;
		int getUniformID_From_Name(std::string name);		// returns -1 if not found
		bool LoadUniformLocation(std::string variableName);	// load uniform location and save it
	};

	ShaderManager();
	~ShaderManager();

	bool useShaderProgram(unsigned int ID);
	bool useShaderProgram(std::string friendlyName);
	bool createProgramFromFile(std::string friendlyName,
		Shader &vertexShader, Shader &fragShader);

	void setBasePath(std::string basePath);
	unsigned int getIDFromFriendlyName(std::string friendlyName);

	ShaderProgram* pGetShaderProgramFromFriendlyName(std::string friendlyName);

	std::string getLastError(void);

private:

	bool m_loadSourceFromFile(Shader &shader);
	std::string m_basepath;

	bool m_compileShaderFromSource(Shader &shader, std::string &error);
	bool m_wasThereACompileError(unsigned int shaderID, std::string& errorText);
	bool m_wasThereALinkerError(unsigned int progID, std::string& errorText);

	std::string m_lastError;

	std::map<unsigned int, ShaderProgram> m_ID_to_Shader;
	std::map < std::string, unsigned int> m_name_to_ID;

};

