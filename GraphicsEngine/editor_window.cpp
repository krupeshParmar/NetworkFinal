/**
*	Student name:	Krupesh Ashok Parmar
*	Student number:	1124374
*	Student mail:	k_parmar180076@fanshaweonline.ca
*	Program name:	GAME DEVELOPMENT ADVANCED PROGRAMMING
*	Course name:	
*	
*	a) How to build this project:
*		- Select x64 platform
*		- Select Release configuration for best performance
*		- Right click on "GraphicsEngine" from Solution Explorer and hit build
*		- The build is now created for selected platform
*	
*	b) How to run this project:
*		- You can run project only from Visual Studio using key "F5" or "Ctrl + F5"
*	
*	c) User inputs:
*		- Hold right click to interact with camera
*		- While right click is held down use WASD keys to move camera
*		- Hold down shift key to move faster
*		- The Game Play inputs are mentioned in the editor window
*/

#include "inc_opengl.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include "global/GlobalThings.h"
#include "ShaderManager/ShaderManager.h"
#include "VAOManager/VAOManager.h"
#include "Lighting/LightHelper.h"

#include "VAOManager/VAOManager.h"
#include "GameObject/GameObject.h"
#include "System/Components/Camera.h"
#include "SceneEditor.h"
#include "System/AudioManager.h"
#include "Player.h"

SceneEditor theSceneEditor;
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
bool firstMouse = true;
bool mouseHoldDown = false;


void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	theSceneEditor.mouse_callback(window, xposIn, yposIn);
}

// Call back signatures here
void key_callback(GLFWwindow* window,
	int key, int scancode,
	int action, int mods)
{
	int sum = 0;
	bool isshot = false;

	theSceneEditor.player->inputMessage->input_sum = sum;
	/*if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		std::cout << "Clicked!" << std::endl;
		theSceneEditor.ClickObject(glm::vec2(theSceneEditor.xpos,theSceneEditor.ypos));
	}*/
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		if (theSceneEditor.selectedGameObject != nullptr)
		{
			theSceneEditor.EDITOR_CAMERA->transform->position =
				theSceneEditor.selectedGameObject->transform->position + glm::vec3(-5.f, 2.f, -5.f);
		}
	}
	if (theSceneEditor.gamePlay)
	{
		float cameraSpeed;
		glm::vec3 defFront = theSceneEditor.cameraFront;
		defFront.y = 0.f;

		cameraSpeed = 2.5f * theSceneEditor.deltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			sum += 1;
			/*if (!theSceneEditor.w_pressed)
			{
				theSceneEditor.w_pressed = true;
			}*/
		}
		//else theSceneEditor.w_pressed = false;

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			sum += 10;
			/*if (!theSceneEditor.s_pressed)
			{
				theSceneEditor.s_pressed = true;
			}*/
		}
		//else theSceneEditor.s_pressed = false;

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			sum += 100;
			/*if (!theSceneEditor.a_pressed)
			{
				theSceneEditor.a_pressed = true;
			}*/
		}
		//else theSceneEditor.a_pressed = false;

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			sum += 1000;
			/*if (!theSceneEditor.d_pressed)
			{
				theSceneEditor.d_pressed = true;
			}*/
		}
		//else theSceneEditor.d_pressed = false;

		theSceneEditor.player->inputMessage->isshot = isshot;
		theSceneEditor.player->inputMessage->input_sum = sum;
		return;
	}
	return;
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

int main()
{
	std::cout << "Booting up..." << std::endl;
	//theSceneEditor = SceneEditor();
		
	if (!glfwInit())
		exit(EXIT_FAILURE);

	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);*/
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
	glfwSetErrorCallback(error_callback);
	
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Scene view", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	theSceneEditor.InitSceneRender(window);
	/*if (audioManager.Initialize(255, FMOD_INIT_NORMAL))
	{
		return -1;
	}*/

	GLuint vertex_buffer = 0;
	GLuint shaderID = 0;
	GLint vpos_location = 0;
	GLint vcol_location = 0;

	ShaderManager* pShaderManager = new ShaderManager();
	ShaderManager::Shader vertexShader01;
	ShaderManager::Shader fragmentShader01;

	vertexShader01.filename = "assets/shaders/vertexShader01.glsl";
	fragmentShader01.filename = "assets/shaders/fragmentShader01.glsl";

	if (!pShaderManager->createProgramFromFile("Shader_1", vertexShader01, fragmentShader01))
	{
		std::cout << "Couldn\'t compile shaders" << std::endl;
		std::string theLastError = pShaderManager->getLastError();
		std::cout << "Error: " << theLastError << std::endl;
		return -1;
	}
	else
	{
		std::cout << "Shaders successfully compiled :)" << std::endl;
	}

	pShaderManager->useShaderProgram("Shader_1");

	shaderID = pShaderManager->getIDFromFriendlyName("Shader_1");
	glUseProgram(shaderID);

	::g_pTheLightManager = new LightManager();
	LightHelper* pLightHelper = new LightHelper();

	::g_pTheLightManager->LoadLightUniformLocation(shaderID);

	cVAOManager* pVAOManager = new cVAOManager();
	theSceneEditor.sceneFileName = "Scenes/PhysicsFinal.xml";
	if (theSceneEditor.LoadSceneFile(pVAOManager, shaderID))
	{
		std::cout << "Scene successfully loaded!" << std::endl;
	}
	else std::cout << "Scene failed to load!" << std::endl;
	
	theSceneEditor.RenderScene(window, shaderID);

	

	delete pShaderManager;
	delete ::g_pTheLightManager;

	theSceneEditor.ShutdownRender();

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
