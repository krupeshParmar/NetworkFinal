#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "VAOManager/VAOManager.h"
#include "inc_opengl.h"
#include <pugixml.hpp>
#include <vector>
#include "System/Components/Camera.h"
#include "GameObject/GameObject.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include "Lighting/Light.h"
#include "Lighting/LightManager.h"
#include "global/GlobalThings.h"
#include <imgui/misc/imgui_stdlib.h>
#include "System/Components/BoxCollider.h"
#include "Player.h"
#include "EnemyPlayer.h"
#include "physics/PhysicsSystem.h"
#define XMIN -37
#define XMAX 37
#define ZMIN -2
#define ZMAX 33

class SceneEditor
{
public:

	SceneEditor();
	~SceneEditor();

	std::vector<GameObject*> list_GameObjects;
	static GameObject* EDITOR_CAMERA;
	static GameObject* selectedGameObject;
	GameObject* mainCamera;
	GameObject* lastTracedGameObject = nullptr;
	double hoverTime = 0;

	std::string sceneName;
	cVAOManager* mainVAOManager;
	std::string sceneFileName = "";
	std::string logMessages = "";
	std::string controlsText = "";

	

	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::vec3(0.0f, 0.0f, 0.0f);

	int SCR_WIDTH = 1200;
	int SCR_HEIGHT = 800;

	float YAW = 90.0f;
	float PITCH = 0.0f;
	float SENSITIVITY = 0.1f;
	float xpos;
	float ypos;

	float deltaTime = 0.f;
	float lastFrame = 0.f;
	float lastX = SCR_WIDTH / 2.0f;
	float lastY = SCR_HEIGHT / 2.0f;
	bool firstMouse = true;
	bool mouseHoldDown = false;
	bool mouseClicked = false;

	void GamePlayUpdate(GLFWwindow* window);
	bool gamePlay = false;

	bool LoadPlyFiles(std::string fileName, sModelDrawInfo& modelDrawInfo);
	bool LoadSceneFile(cVAOManager* pVAOManager, GLuint shaderID);
	bool SaveSceneFile();

	void DrawGizmos(GLFWwindow* window, GLuint shaderID, glm::mat4 matView, glm::mat4 matProjection, GameObject* gameObject, int index, int type);
	void RenderScene(GLFWwindow* window,GLuint shaderID);
	void RenderUI(GLFWwindow* window,GLuint shaderID);
	bool ClickObject(glm::vec2 pos, bool hover = false);
	void InitGameScene();
	void FinishGameScene();

	void CreateNewGameObject();
	GameObject* CreateNewGameObject(std::string name);
	void DeleteGameObjects(std::vector<GameObject*>);
	void ShutdownRender();
	bool InitSceneRender(GLFWwindow* window);
	void DuplicateGameObject(GameObject* gameobject);
	GameObject* GetGameObjectByName(std::string);

	void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
	void ProcessInput(GLFWwindow* window);
	void ProcessMouseMovement(float xoffset, float yoffset);

	// Physics Game Stuff
	PhysicsSystem physicsSystem;
	Player* player = new Player();
	std::vector<EnemyPlayer*> list_Enemies;
	bool w_pressed = false;
	bool s_pressed = false;
	bool a_pressed = false;
	bool d_pressed = false;
};

