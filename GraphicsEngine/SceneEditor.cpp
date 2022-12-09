#include "SceneEditor.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "System/Components/RigidBody.h"

GameObject* SceneEditor::EDITOR_CAMERA = nullptr;
GameObject* SceneEditor::selectedGameObject = nullptr;

SceneEditor::SceneEditor()
{
	SceneEditor::EDITOR_CAMERA = new GameObject();
}

SceneEditor::~SceneEditor()
{
	delete SceneEditor::EDITOR_CAMERA;
}

bool SceneEditor::InitSceneRender(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& imGuiIO = ImGui::GetIO();

	if (!ImGui_ImplGlfw_InitForOpenGL(window, true) || !ImGui_ImplOpenGL3_Init("#version 460"))
	{
		return false;
	}
	ImGui::StyleColorsDark();
	return true;
}


void SceneEditor::ProcessInput(GLFWwindow* window)
{
	int sum = 0;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && gamePlay)
	{
		gamePlay = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		FinishGameScene();
		std::cout << "Gameplay finished";
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS
		|| glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		const glm::vec2 click(xpos, ypos);

		ClickObject(click);
		mouseClicked = true;
	}
	else mouseClicked = false;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		mouseHoldDown = true;
	else mouseHoldDown = false;

	if (mouseHoldDown || gamePlay)
	{
		glm::vec3 defFront = cameraFront;
		if (gamePlay)
			defFront.y = 0.f;
		float cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && !gamePlay)
			cameraSpeed = 10.5f * deltaTime;
		else  cameraSpeed = 2.5f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			mainCamera->transform->position += cameraSpeed * defFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			mainCamera->transform->position -= cameraSpeed * defFront;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			mainCamera->transform->position -= cameraRight * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			mainCamera->transform->position += cameraRight * cameraSpeed;
		
		if (gamePlay)
		{
			if (mainCamera->transform->position.x > XMAX)
				mainCamera->transform->position.x = XMAX;

			if (mainCamera->transform->position.x < XMIN)
				mainCamera->transform->position.x = XMIN;

			if (mainCamera->transform->position.z > ZMAX)
				mainCamera->transform->position.z = ZMAX;

			if (mainCamera->transform->position.z < ZMIN)
				mainCamera->transform->position.z = ZMIN;
		}

		if (!gamePlay)
		{
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)     // Down
			{
				mainCamera->transform->position.y -= cameraSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)      // Up
			{
				mainCamera->transform->position.y += cameraSpeed;
			}
		}
	}
}


void SceneEditor::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	xpos = static_cast<float>(xposIn);
	ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	//if (mouseClicked)
	//{
	//	/*const glm::vec2 click(xpos, ypos);
	//	ClickObject(click);*/
	//}
	if (mouseHoldDown || gamePlay)
		ProcessMouseMovement(xoffset, yoffset);
}

void SceneEditor::ProcessMouseMovement(float xoffset, float yoffset)
{
	xoffset *= SENSITIVITY;
	yoffset *= SENSITIVITY;

	YAW += xoffset;
	PITCH += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (PITCH > 89.9f)
		PITCH = 89.9f;
	if (PITCH < -89.9f)
		PITCH = -89.9f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(YAW)) * cos(glm::radians(PITCH));
	direction.y = sin(glm::radians(PITCH));
	direction.z = sin(glm::radians(YAW)) * cos(glm::radians(PITCH));
	cameraFront = glm::normalize(direction);
	cameraRight = glm::normalize(glm::cross(cameraFront, WorldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));

	// update Front, Right and Up Vectors using the updated Euler angles

}


void SceneEditor::DuplicateGameObject(GameObject* gameobject)
{

}

GameObject* SceneEditor::GetGameObjectByName(std::string name)
{
	std::vector<GameObject*>::iterator gameObjectIterator = list_GameObjects.begin();
	for (; gameObjectIterator != list_GameObjects.end(); gameObjectIterator++)
	{
		GameObject* gameobject = *gameObjectIterator;
		if (gameobject->name == name)
			return gameobject;
	}
	return nullptr;
}


void SceneEditor::CreateNewGameObject()
{
	GameObject* gameObject = new GameObject();
	gameObject->name = "GameObject" + std::to_string(list_GameObjects.size());
	list_GameObjects.push_back(gameObject);
	selectedGameObject = gameObject;
}


GameObject* SceneEditor::CreateNewGameObject(std::string name)
{
	GameObject* gameObject = new GameObject();
	gameObject->name = name;
	gameObject->meshObject->meshName = name;
	list_GameObjects.push_back(gameObject);
	return gameObject;
}

void SceneEditor::DeleteGameObjects(std::vector<GameObject*> gameobjectList)
{
	for (int i = 0; i < gameobjectList.size(); i++)
	{
		std::vector<GameObject*>::iterator iter;
		for (iter = list_GameObjects.begin(); iter != list_GameObjects.end(); iter++)
		{
			if (*iter == gameobjectList[i])
				break;
		}
		if (iter != list_GameObjects.end()) {
			list_GameObjects.erase(iter);
		}
	}
}

void SceneEditor::GamePlayUpdate(GLFWwindow* window)
{
	player->Update(deltaTime);
	//physicsSystem.UpdateStep(deltaTime);
}

void SceneEditor::RenderScene(GLFWwindow* window, GLuint shaderID)
{
	glfwGetWindowSize(window,&SCR_WIDTH, &SCR_HEIGHT);
	GLint mvp_location = glGetUniformLocation(shaderID, "MVP");
	GLint mModel_location = glGetUniformLocation(shaderID, "mModel");
	GLint mView_location = glGetUniformLocation(shaderID, "mView");
	GLint mProjection_location = glGetUniformLocation(shaderID, "mProjection");

	GLint mModelInverseTransform_location = glGetUniformLocation(shaderID, "mModelInverseTranspose");

	cameraRight = glm::normalize(glm::cross(cameraFront, WorldUp));
	while (!glfwWindowShouldClose(window))
	{
		float currentFfame = glfwGetTime();
		deltaTime = currentFfame - lastFrame;
		lastFrame = currentFfame;

		ProcessInput(window);

		::g_pTheLightManager->CopyLightInformationToShader(shaderID);

		float ratio;
		int width, height;

		glm::mat4x4 matModel;
		glm::mat4x4 matProjection;
		glm::mat4x4 matView;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);


		matView = glm::lookAt(mainCamera->transform->position,
			mainCamera->transform->position + cameraFront,
			upVector);

		GLint eyeLocation_UniLoc = glGetUniformLocation(shaderID, "eyeLocation");
		glUniform4f(eyeLocation_UniLoc, mainCamera->transform->position.x,
			mainCamera->transform->position.y, mainCamera->transform->position.z, 1.0f);

		matProjection = glm::perspective(
			0.6f,
			ratio,
			0.1f,
			10000.0f);

		for (std::vector<GameObject*>::iterator itCurrentGameObject = list_GameObjects.begin();
			itCurrentGameObject != list_GameObjects.end();
			itCurrentGameObject++)
		{
			//MeshObject* currentMeshObject = *itCurrentMesh;
			GameObject* currentGameObject = *itCurrentGameObject;
			if (!currentGameObject->enabled)
				continue;
			if (currentGameObject->parent != nullptr)
				if (!currentGameObject->parent->enabled)
					continue;
			glCullFace(GL_BACK);

			glEnable(GL_DEPTH_TEST);

			if (currentGameObject->parent == nullptr)
				matModel = glm::mat4x4(1.0f);  // identity matrix
			else matModel = currentGameObject->parent->matModel;

			//// Rotate the object
			glm::mat4 matRoationZ = glm::rotate(glm::mat4(1.0f),
				currentGameObject->transform->rotation.z,                // Angle to rotate
				glm::vec3(0.0f, 0.0f, 1.0f));       // Axis to rotate around

			glm::mat4 matRoationY = glm::rotate(glm::mat4(1.0f),
				currentGameObject->transform->rotation.y,                // Angle to rotate
				glm::vec3(0.0f, 1.0f, 0.0f));       // Axis to rotate around

			glm::mat4 matRoationX = glm::rotate(glm::mat4(1.0f),
				currentGameObject->transform->rotation.x,                // Angle to rotate
				glm::vec3(1.0f, 0.0f, 0.0f));       // Axis to rotate around


			// Move the object 
			glm::mat4 matTranslation = glm::translate(glm::mat4(1.0f),
				currentGameObject->transform->position);


			// Scale the object
			glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
				glm::vec3(currentGameObject->transform->scale));

			// Applying all these transformations to the MODEL 
			// (or "world" matrix)
			matModel = matModel * matTranslation;

			matModel = matModel * matRoationX;
			matModel = matModel * matRoationY;
			matModel = matModel * matRoationZ;

			matModel = matModel * matScale;

			currentGameObject->matModel = matModel;

			glUniformMatrix4fv(mModel_location, 1, GL_FALSE, glm::value_ptr(matModel));
			glUniformMatrix4fv(mView_location, 1, GL_FALSE, glm::value_ptr(matView));
			glUniformMatrix4fv(mProjection_location, 1, GL_FALSE, glm::value_ptr(matProjection));

			// Inverse transpose of a 4x4 matrix removes the right column and lower row
			// Leaving only the rotation (the upper left 3x3 matrix values)
			glm::mat4 mModelInverseTransform = glm::inverse(glm::transpose(matModel));
			glUniformMatrix4fv(mModelInverseTransform_location, 1, GL_FALSE, glm::value_ptr(mModelInverseTransform));

			if (currentGameObject->meshObject->isWireframe)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			GLint RGBA_COLOR_ULocID = glGetUniformLocation(shaderID, "RGBA_Colour");

			glUniform4f(RGBA_COLOR_ULocID,
				currentGameObject->meshObject->RGBA_color.r,
				currentGameObject->meshObject->RGBA_color.g,
				currentGameObject->meshObject->RGBA_color.b,
				currentGameObject->meshObject->RGBA_color.w);

			GLint bUseRGBA_Color_ULocID = glGetUniformLocation(shaderID, "bUseRGBA_Colour");

			if (currentGameObject->meshObject->bUse_RGBA_color)
				glUniform1f(bUseRGBA_Color_ULocID, (GLfloat)GL_TRUE);
			else
				glUniform1f(bUseRGBA_Color_ULocID, (GLfloat)GL_FALSE);

			GLint bDoNotLight_Colour_ULocID = glGetUniformLocation(shaderID, "bDoNotLight");
			if (currentGameObject->meshObject->bDoNotLight)
				glUniform1f(bDoNotLight_Colour_ULocID, (GLfloat)GL_TRUE);
			else
				glUniform1f(bDoNotLight_Colour_ULocID, (GLfloat)GL_FALSE);

			sModelDrawInfo drawingInformation;
			if (mainVAOManager->FindDrawInfoByModelName(currentGameObject->meshObject->meshName, drawingInformation))
			{
				glBindVertexArray(drawingInformation.VAO_ID);

				glDrawElements(GL_TRIANGLES,
					drawingInformation.numberOfIndices,
					GL_UNSIGNED_INT,
					(void*)0);

				glBindVertexArray(0);
			}

			if (gamePlay)
				continue;
			for (int c = 0; c < currentGameObject->components.size(); c++)
			{
				if (currentGameObject->components[c]->componentType == "boxcollider")
				{
					this->DrawGizmos(window, shaderID, matView, matProjection, currentGameObject,c, 0);
				}
			}
		}
		this->RenderUI(window,shaderID);
		if (gamePlay)
		{
			GamePlayUpdate(window);
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
		std::stringstream ssTitle;
		ssTitle << "Camera (x,y,z): "
			<< mainCamera->transform->position.x << ", "
			<< mainCamera->transform->position.y << ", "
			<< mainCamera->transform->position.z;

		std::string theText = ssTitle.str();

		glfwSetWindowTitle(window, ssTitle.str().c_str());
	}
	
}

void SceneEditor::InitGameScene()
{
	GameObject* camera = GetGameObjectByName("Camera");
	if (camera != nullptr)
	{
		mainCamera = camera;
		mainCamera->transform->position = glm::vec3(0, 2, 0);
	}
}
void SceneEditor::FinishGameScene()
{
	mainCamera = EDITOR_CAMERA;
	GameObject* gameObject = GetGameObjectByName("player");
}

void SceneEditor::RenderUI(GLFWwindow* window,GLuint shaderID)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Log");
	ImGui::BeginChild("##log", ImVec2(600, 100));
	ImGui::Text(logMessages.c_str());
	ImGui::EndChild();
	ImGui::End();
	ImGui::Begin("Controls");
	ImGui::BeginChild("##controls", ImVec2(1200, 150));
	ImGui::Text(controlsText.c_str());
	ImGui::EndChild();
	ImGui::End();
	ImGui::Begin("Hierarchy");
	ImGui::BeginChild("##pauseplay", ImVec2(100, 30));
	ImGui::Checkbox("GamePlay##gameplay", &gamePlay);
	if (ImGui::IsItemEdited())
	{
		if (gamePlay)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			InitGameScene();
			std::cout << "Gameplay stareted";
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			FinishGameScene();
			std::cout << "Gameplay finished";
		}
	}
	ImGui::EndChild();
	if (gamePlay)
	{
		ImVec2 window_size =
			ImGui::GetIO().DisplaySize;
		ImVec2 window_center = ImVec2(window_size.x * 0.5f, window_size.y * 0.5f);
		
		ImGui::GetForegroundDrawList()->AddCircle(window_center, window_size.y * 0.01f, IM_COL32(100, 255, 0, 150), 0, 10);

		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		return;
	}
	ImGui::BeginGroup();
	ImGui::BeginChild("##scene", ImVec2(1920 * 0.15, 0));
	ImGui::Button("Save");
	if (ImGui::IsItemClicked())
	{
		std::cout << "Saving " << sceneName << std::endl;
		this->SaveSceneFile();
	}
	int i = 0;
	/*if (ImGui::CollapsingHeader(sceneFileName.c_str()))
	{

	}*/
	for (std::vector<GameObject*>::iterator gameObjectsIterator = list_GameObjects.begin();
		gameObjectsIterator != list_GameObjects.end();
		gameObjectsIterator++)
	{
		GameObject* gameObject = *gameObjectsIterator;
		gameObject->Update();
		std::string gameObjectName = gameObject->name;
		if (gameObject->children.size() > 0)
		{
			if (ImGui::TreeNode(gameObjectName.c_str()))
			{
				if (ImGui::IsItemClicked())
				{
					SceneEditor::selectedGameObject = gameObject;
				}
				for (int k = 0; k < gameObject->children.size(); k++)
				{
					if (selectedGameObject == gameObject->children[k])
					{
						ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), gameObject->children[k]->name.c_str());
					}
					else ImGui::Text(gameObject->children[k]->name.c_str());
					if (ImGui::IsItemClicked())
					{
						SceneEditor::selectedGameObject = gameObject->children[k];
					}
				}
				ImGui::TreePop();
			}
		}
		if (gameObject->parent != nullptr || gameObject->children.size() > 0)
			continue;
		if (selectedGameObject == gameObject)
		{
			ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), gameObjectName.c_str());
		}
		else ImGui::Text(gameObjectName.c_str());
		if (ImGui::IsItemClicked())
		{
			SceneEditor::selectedGameObject = gameObject;
		}
		i++;
	}
	ImGui::Button("Add new GameObject");
	if (ImGui::IsItemClicked())
	{
		this->CreateNewGameObject();
	}
	ImGui::EndChild();
	ImGui::EndGroup();
	ImGui::End();
	ImGui::Begin("Inspector");
	if (selectedGameObject != nullptr)
	{
		ImGui::BeginChild(selectedGameObject->name.c_str(), ImVec2(1920 * 0.2, 0));
		/*TRANSFORM*/
		ImGui::BeginGroup();
		/*--Position*/
		std::string goName = selectedGameObject->name;
		ImGui::InputText("##Gameobject",
			&goName);
		if (ImGui::IsItemDeactivated())
		{
			if (!goName.empty())
			{
				selectedGameObject->name = goName;
			}
		}

		ImGui::SameLine();
		ImGui::Checkbox("##enabled", &selectedGameObject->enabled);
		ImGui::SameLine();
		ImGui::Button("Duplicate");
		if (ImGui::IsItemClicked())
		{
			this->DuplicateGameObject(selectedGameObject);
		}


		ImGui::Separator();
		{
			ImGui::BulletText("Transform");
			{
				ImGui::Text("Position");
				ImGui::SameLine();
				float pos[3] = {
				selectedGameObject->transform->position.x,
				selectedGameObject->transform->position.y,
				selectedGameObject->transform->position.z };

				ImGui::InputFloat3("##pos", pos);
				selectedGameObject->transform->position.x = pos[0];
				selectedGameObject->transform->position.y = pos[1];
				selectedGameObject->transform->position.z = pos[2];

				ImGui::Text("Rotation");
				ImGui::SameLine();

				float rot[3] = {
				selectedGameObject->transform->rotation.x,
				selectedGameObject->transform->rotation.y,
				selectedGameObject->transform->rotation.z };

				ImGui::InputFloat3("##rot", rot);

				selectedGameObject->transform->rotation.x = rot[0];
				selectedGameObject->transform->rotation.y = rot[1];
				selectedGameObject->transform->rotation.z = rot[2];

				ImGui::Text("Scale");
				ImGui::SameLine();

				float sca[3] = {
				selectedGameObject->transform->scale.x,
				selectedGameObject->transform->scale.y,
				selectedGameObject->transform->scale.z };

				ImGui::InputFloat3("##sca", sca);

				selectedGameObject->transform->scale.x = sca[0];
				selectedGameObject->transform->scale.y = sca[1];
				selectedGameObject->transform->scale.z = sca[2];
			}
			ImGui::EndGroup();
		}
		{
			ImGui::Separator();
			ImGui::BeginGroup();
			ImGui::BulletText("Children");
			int childrenSize = selectedGameObject->children.size();
			ImGui::InputInt("Size##childrensize", &childrenSize);
			if (childrenSize != selectedGameObject->children.size())
			{
				if (childrenSize > 0)
				{
					selectedGameObject->children.clear();
					for (int i = 0; i < childrenSize; i++)
					{
						selectedGameObject->children.push_back(new GameObject());
						selectedGameObject->children[i]->name = "";
					}
				}
				else if (childrenSize == 0)
				{
					for (int i = 0; i < selectedGameObject->children.size(); i++)
					{
						selectedGameObject->children[i]->parent = nullptr;
					}
					selectedGameObject->children.clear();
				}

			}
			if (childrenSize > 0)
			{
				std::vector<std::string> childrenNames;
				for (int i = 0; i < childrenSize; i++)
				{
					childrenNames.push_back("");
					if (selectedGameObject->children[i] != nullptr
						&& selectedGameObject->children[i]->name != "")
					{
						childrenNames[i] = selectedGameObject->children[i]->name;
					}
					std::string label = "##child" + std::to_string(i);
					ImGui::InputText(label.c_str(), &childrenNames[i]);
					selectedGameObject->children[i]->name = childrenNames[i];
					if (ImGui::IsItemDeactivated()) {
						if (selectedGameObject->children[i] != nullptr &&
							selectedGameObject->children[i]->name != "")
						{
							selectedGameObject->children[i] = this->GetGameObjectByName(childrenNames[i]);
							selectedGameObject->children[i]->parent = selectedGameObject;
						}
					}
				}
			}
			ImGui::EndGroup();
		}
		/*components*/
		for (int j = 0; j < selectedGameObject->components.size(); j++)
		{
			iComponent* component = selectedGameObject->components[j];
			if (component->componentType == "light")
			{
				Light* light = (Light*)component;
				//ImGui::Text(list_GameObjects[selectedGameObject]->name.c_str());
				ImGui::Separator();
				ImGui::BulletText("Light");
				{
					ImGui::Text("Diffuse");
					ImGui::InputFloat("R_D", &light->diffuse.x);
					ImGui::InputFloat("G_D", &light->diffuse.y);
					ImGui::InputFloat("B_D", &light->diffuse.z);
				}
				ImGui::Text("Specular");
				{
					ImGui::InputFloat("R_S", &light->specular.x);
					ImGui::InputFloat("G_S", &light->specular.y);
					ImGui::InputFloat("B_S", &light->specular.z);
				}
				ImGui::Text("Attenuation");
				{
					ImGui::InputFloat("X_A", &light->atten.x);
					ImGui::InputFloat("Y_A", &light->atten.y);
					ImGui::InputFloat("Z_A", &light->atten.z);
				}
				ImGui::Text("Parameter 1");
				{
					ImGui::InputFloat("X_P1", &light->param1.x);
					ImGui::InputFloat("Y_P1", &light->param1.y);
					ImGui::InputFloat("Z_P1", &light->param1.z);
				}
				ImGui::Text("Parameter 2");
				{
					ImGui::InputFloat("X_P2", &light->param2.x);
					ImGui::InputFloat("Y_P2", &light->param2.y);
					ImGui::InputFloat("Z_P2", &light->param2.z);
				}
			}
			if (component->componentType == "boxcollider")
			{

			}
		}
		ImGui::Separator();
		if (selectedGameObject->meshObject == nullptr)
			ImGui::ColorButton("Add mesh object", ImVec4(1.f, 0.f, 1.f, 1.f));
		else {
			ImGui::InputText("Model name",
				&selectedGameObject->meshObject->meshName);

			ImGui::InputText("Path value",
				&selectedGameObject->meshObject->path);
			ImGui::Button("Load model");
			if (ImGui::IsItemClicked())
			{
				// Load model
				sModelDrawInfo smodeldrawInfo;
				if (this->LoadPlyFiles(selectedGameObject->meshObject->path, smodeldrawInfo))
				{
					mainVAOManager->LoadModelIntoVAO(selectedGameObject->meshObject->meshName,
						smodeldrawInfo, shaderID);
				}
			}

			bool usesLight = !selectedGameObject->meshObject->bDoNotLight;
			ImGui::Checkbox("Uses light", &usesLight);
			selectedGameObject->meshObject->bDoNotLight = !usesLight;

			bool usesRGBA = selectedGameObject->meshObject->bUse_RGBA_color;
			ImGui::Checkbox("Uses RGBA values", &usesRGBA);
			selectedGameObject->meshObject->bUse_RGBA_color = usesRGBA;

			ImGui::InputFloat("Red", &selectedGameObject->meshObject->RGBA_color.r);
			ImGui::InputFloat("Green", &selectedGameObject->meshObject->RGBA_color.g);
			ImGui::InputFloat("Blue", &selectedGameObject->meshObject->RGBA_color.b);
			ImGui::InputFloat("Alpha", &selectedGameObject->meshObject->RGBA_color.a);

			bool isWireFrame = selectedGameObject->meshObject->isWireframe;
			ImGui::Checkbox("Wireframe", &isWireFrame);
			selectedGameObject->meshObject->isWireframe = isWireFrame;
		}

		bool hasLight = false;
		bool hasBoxCollider = false;
		bool hasRigidBody = false;
		for (int l = 0; l < selectedGameObject->components.size(); l++)
		{
			if (selectedGameObject->components[l]->componentType == "light")
				hasLight = true;
			if (selectedGameObject->components[l]->componentType == "boxcollider")
				hasBoxCollider = true;
			if (selectedGameObject->components[l]->componentType == "rigidbody")
				hasRigidBody = true;
		}
		if (!hasLight)
		{
			ImGui::Separator();
			ImGui::Button("Add light component");
			if (ImGui::IsItemClicked())
			{
				Light* light = new Light();
				::g_pTheLightManager->AddNewLightInfo(light);
				::g_pTheLightManager->LoadLightUniformLocation(shaderID);
				selectedGameObject->components.push_back(light);
			}
		}
		if (!hasRigidBody)
		{
			ImGui::Separator();
			ImGui::Button("Add Rigidbody component");
			if (ImGui::IsItemClicked())
			{
				RigidBody* rigidbody = new RigidBody();
				selectedGameObject->components.push_back(rigidbody);
			}
		}
		if (!hasBoxCollider && selectedGameObject->meshObject != nullptr)
		{
			ImGui::Separator();
			ImGui::Button("Add Box Collider");
			if (ImGui::IsItemClicked())
			{
				BoxCollider* boxCollider = new BoxCollider();
				sModelDrawInfo modelDrawInfo;
				if (!this->mainVAOManager->FindDrawInfoByModelName("boxcollider", modelDrawInfo))
				{
					if (this->LoadPlyFiles(boxCollider->box_model_path, modelDrawInfo))
						this->mainVAOManager->LoadModelIntoVAO("boxcollider", modelDrawInfo, shaderID);
				}
				boxCollider->minPosition = selectedGameObject->meshObject->minPoint;
				boxCollider->maxPosition = selectedGameObject->meshObject->maxPoint;
				selectedGameObject->components.push_back(boxCollider);
			}
		}

		ImGui::EndChild();
	}
	ImGui::End();
	ImGui::ShowDemoWindow();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool SceneEditor::ClickObject(glm::vec2 pos, bool hover)
{
	// 1. Cursor Position on the Screen
	glm::vec3 cursorPositionOnScreenSpace(
		pos.x,				// X is fine from left to right
		pos.y,				// Since Y is origin at the top, and positive as it goes down the screen
							// we need to fix it like this.
		1.f
	);
	
	// 2. Viewport: Window Information
	glm::vec4 viewport = glm::vec4(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// 3 Projection Matrix
	glm::mat4 projectionMatrix = glm::perspective(
		0.6f,			// Field of View
		(float)SCR_WIDTH / (float)SCR_HEIGHT,	// Aspect Ratio
		0.1f,							// zNear plane
		10000.0f							// zFar plane
	);

	glm::mat4 viewMatrix = glm::lookAt(
		mainCamera->transform->position,				// Position of the Camera
		mainCamera->transform->position + cameraFront,			// Target view point
		glm::vec3(0, 1, 0)				// Up direction
	);
	cursorPositionOnScreenSpace.x = SCR_WIDTH / 2;
	cursorPositionOnScreenSpace.y = SCR_HEIGHT / 2;
	// Calculate our position in world space
	glm::vec3 pointInWorldSpace = glm::unProject(cursorPositionOnScreenSpace, viewMatrix, projectionMatrix, viewport);

	GameObject* gameObject;

	return false;
}

void SceneEditor::DrawGizmos(GLFWwindow* window, GLuint shaderID, glm::mat4 matView, glm::mat4 matProjection, GameObject* gameObject,int index, int type)
{
	GLint mModel_location = glGetUniformLocation(shaderID, "mModel");
	GLint mView_location = glGetUniformLocation(shaderID, "mView");
	GLint mProjection_location = glGetUniformLocation(shaderID, "mProjection");
	GLint mModelInverseTransform_location = glGetUniformLocation(shaderID, "mModelInverseTranspose");

	if (type == 0)
	{
		glm::mat4 matModel = gameObject->matModel;

		glm::mat4 matTranslation = glm::translate(glm::mat4(1.0),
			gameObject->meshObject->centerPoint);

		glm::mat4 matRoation = glm::mat4_cast(glm::quat(1, 0, 0, 0));

		glm::mat4 matScale = glm::scale(glm::mat4(1.f), gameObject->meshObject->halfExtent);
		matModel = matModel * matTranslation;

		matModel = matModel * matRoation;

		matModel = matModel * matScale;

		glUniformMatrix4fv(mModel_location, 1, GL_FALSE, glm::value_ptr(matModel));
		glUniformMatrix4fv(mView_location, 1, GL_FALSE, glm::value_ptr(matView));
		glUniformMatrix4fv(mProjection_location, 1, GL_FALSE, glm::value_ptr(matProjection));

		// Inverse transpose of a 4x4 matrix removes the right column and lower row
		// Leaving only the rotation (the upper left 3x3 matrix values)
		glm::mat4 mModelInverseTransform = glm::inverse(glm::transpose(matModel));
		glUniformMatrix4fv(mModelInverseTransform_location, 1, GL_FALSE, glm::value_ptr(mModelInverseTransform));

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		GLint RGBA_COLOR_ULocID = glGetUniformLocation(shaderID, "RGBA_Colour");

		glUniform4f(RGBA_COLOR_ULocID, 1.f,1.f,1.f,1.f);

		GLint bUseRGBA_Color_ULocID = glGetUniformLocation(shaderID, "bUseRGBA_Colour");
		
		glUniform1f(bUseRGBA_Color_ULocID, (GLfloat)GL_TRUE);

		GLint bDoNotLight_Colour_ULocID = glGetUniformLocation(shaderID, "bDoNotLight");
		glUniform1f(bDoNotLight_Colour_ULocID, (GLfloat)GL_TRUE);

		sModelDrawInfo drawingInformation;
		if (mainVAOManager->FindDrawInfoByModelName("boxcollider", drawingInformation))
		{
			glBindVertexArray(drawingInformation.VAO_ID);

			glDrawElements(GL_TRIANGLES,
				drawingInformation.numberOfIndices,
				GL_UNSIGNED_INT,
				(void*)0);

			glBindVertexArray(0);
		}
	}
}

void SceneEditor::ShutdownRender()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}

bool SceneEditor::LoadPlyFiles(std::string fileName, sModelDrawInfo& modelDrawInfo)
{
	struct sVertex_XYZ_N_RGBA_UV
	{
		float x, y, z;
		float nx, ny, nz;

		float red, green, blue, alpha;
		float texture_u, texture_v;
	};

	struct sTrianglePLY
	{
		unsigned int vertexIndices[3];
	};

	sVertex_XYZ_N_RGBA_UV* pTheModelArray = NULL;
	sTrianglePLY* pTheModelTriangleArray = NULL;

	std::ifstream theFile(fileName);
	if (!theFile.is_open())
	{
		std::cout << "Couldn\'t open the model file";
		return false;
	}

	char buffer[10000];
	theFile.getline(buffer, 10000);

	std::string theNextToken;

	while (theFile >> theNextToken)
		if (theNextToken == "vertex")
			break;

	theFile >> modelDrawInfo.numberOfVertices;

	bool hasColours = false;
	while (theFile >> theNextToken)
	{
		if (theNextToken == "red")
			hasColours = true;
		if (theNextToken == "face")
			break;
	}

	theFile >> modelDrawInfo.numberOfTriangles;

	while (theFile >> theNextToken)
		if (theNextToken == "end_header")
			break;

	pTheModelArray = new sVertex_XYZ_N_RGBA_UV[modelDrawInfo.numberOfVertices];

	std::cout << "Loading";
	for (unsigned int count = 0; count != modelDrawInfo.numberOfVertices; count++)
	{
		theFile >> pTheModelArray[count].x;
		theFile >> pTheModelArray[count].y;
		theFile >> pTheModelArray[count].z; 

		theFile >> pTheModelArray[count].nx;
		theFile >> pTheModelArray[count].ny;
		theFile >> pTheModelArray[count].nz;

		if (hasColours)
		{
			theFile >> pTheModelArray[count].red;
			theFile >> pTheModelArray[count].green;
			theFile >> pTheModelArray[count].blue;
			theFile >> pTheModelArray[count].alpha;
		}
		if (fileName == "assets/models/island.ply")
		{
			if (pTheModelArray[count].y <= 1)
			{
				pTheModelArray[count].red = 64.f / 255.f;
				pTheModelArray[count].green = 224.f / 255.f;
				pTheModelArray[count].blue = 208.f / 255.f;
				pTheModelArray[count].alpha = 1.f / 255.f;

			}
			if (pTheModelArray[count].y > 1 && pTheModelArray[count].y < 50.f)
			{
				pTheModelArray[count].red = 123.f / 255.f;
				pTheModelArray[count].green = 63.f / 255.f;
				pTheModelArray[count].blue = 0.f / 255.f;
				pTheModelArray[count].alpha = 1.f / 255.f;
			}
			if (pTheModelArray[count].y >= 50.f)
			{
				pTheModelArray[count].red = 255.f / 255.f;
				pTheModelArray[count].green = 255.f / 255.f;
				pTheModelArray[count].blue = 255.f / 255.f;
				pTheModelArray[count].alpha = 1.f / 255.f;
			}
		}

		theFile >> pTheModelArray[count].texture_u;
		theFile >> pTheModelArray[count].texture_v;

		if (count % 10000 == 0)
			std::cout << ".";

	}

	std::cout << "done" << std::endl;

	pTheModelTriangleArray = new sTrianglePLY[modelDrawInfo.numberOfTriangles];

	for (unsigned int count = 0; count != modelDrawInfo.numberOfTriangles; count++)
	{
		// to discard first value
		unsigned int discard = 0;
		theFile >> discard;

		theFile >> pTheModelTriangleArray[count].vertexIndices[0];
		theFile >> pTheModelTriangleArray[count].vertexIndices[1];
		theFile >> pTheModelTriangleArray[count].vertexIndices[2];
	}

	theFile.close();

	modelDrawInfo.pVertices = new sVertex[modelDrawInfo.numberOfVertices];
	glm::vec3 minPoints = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	glm::vec3 maxPoints = glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN);
	for (unsigned int index = 0; index != modelDrawInfo.numberOfVertices; index++)
	{
		// To shader								from File
		modelDrawInfo.pVertices[index].x = pTheModelArray[index].x;
		modelDrawInfo.pVertices[index].y = pTheModelArray[index].y;
		modelDrawInfo.pVertices[index].z = pTheModelArray[index].z;

		if (minPoints.x > pTheModelArray[index].x)
			minPoints.x = pTheModelArray[index].x;
		
		if (minPoints.y > pTheModelArray[index].y)
			minPoints.y = pTheModelArray[index].y;
		
		if (minPoints.z > pTheModelArray[index].z)
			minPoints.z = pTheModelArray[index].z;

		if (maxPoints.x < pTheModelArray[index].x)
			maxPoints.x = pTheModelArray[index].x;
			
		if (maxPoints.y < pTheModelArray[index].y)
			maxPoints.y = pTheModelArray[index].y;
			
		if (maxPoints.z < pTheModelArray[index].z)
			maxPoints.z = pTheModelArray[index].z;

		modelDrawInfo.pVertices[index].r = pTheModelArray[index].red;
		modelDrawInfo.pVertices[index].g = pTheModelArray[index].green;
		modelDrawInfo.pVertices[index].b = pTheModelArray[index].blue;

		modelDrawInfo.pVertices[index].nx = pTheModelArray[index].nx;
		modelDrawInfo.pVertices[index].ny = pTheModelArray[index].ny;
		modelDrawInfo.pVertices[index].nz = pTheModelArray[index].nz;
	}
	std::cout << fileName << " Min values: " << minPoints.x << ", "
		<< minPoints.y << ", " << minPoints.z << "\nMax values: " << maxPoints.x << ", "
		<< maxPoints.y << ", " << maxPoints.z << std::endl;
	modelDrawInfo.numberOfIndices = modelDrawInfo.numberOfTriangles * 3;
	modelDrawInfo.minValues = minPoints;
	modelDrawInfo.maxValues = maxPoints;

	modelDrawInfo.pIndices = new unsigned int[modelDrawInfo.numberOfIndices];

	unsigned int vertex_element_index_index = 0;

	for (unsigned int triangleIndex = 0; triangleIndex != modelDrawInfo.numberOfTriangles; triangleIndex++)
	{
		modelDrawInfo.pIndices[vertex_element_index_index + 0] = pTheModelTriangleArray[triangleIndex].vertexIndices[0];
		modelDrawInfo.pIndices[vertex_element_index_index + 1] = pTheModelTriangleArray[triangleIndex].vertexIndices[1];
		modelDrawInfo.pIndices[vertex_element_index_index + 2] = pTheModelTriangleArray[triangleIndex].vertexIndices[2];

		vertex_element_index_index += 3;
	}

	delete[] pTheModelArray;
	delete[] pTheModelTriangleArray;
	return true;
}

bool SceneEditor::LoadSceneFile(cVAOManager* pVAOManager, GLuint shaderID)
{
	mainVAOManager = pVAOManager;
	pugi::xml_document sceneDoc;
	if (!sceneDoc.load_file(sceneFileName.c_str()))
	{
		std::cout << "Couldn't open " + sceneFileName;
		std::cout << "Wanna create a new scene?\n1-Yes\n2-No" << std::endl;
		int choice = -1;
		while (choice < 0 || choice > 2)
		{
			std::cout << "Choice:";
			std::cin >> choice;
		}
		if (choice == 1)
		{
			SceneEditor::EDITOR_CAMERA = new GameObject();
			SceneEditor::EDITOR_CAMERA->transform->Reset();
			this->SaveSceneFile();
			return this->LoadSceneFile(pVAOManager, shaderID);
		}
		return false;
	}

	pugi::xml_object_range<pugi::xml_node_iterator> sceneData = sceneDoc.child("scene").children();

	if (sceneData.empty())
	{
		std::cout << "Scene has no data!" << std::endl;
		return false;
	}

	pugi::xml_node_iterator sceneNodeIterator = sceneData.begin();

	for (; sceneNodeIterator != sceneData.end(); sceneNodeIterator++)
	{
		pugi::xml_node sceneNode = *sceneNodeIterator;
		std::string sceneNodeName = sceneNode.name();
		if (sceneNodeName == "name")
		{
			SceneEditor::sceneName = sceneNode.child_value();
		}
		if (sceneNodeName == "gameobject")
		{
			GameObject* gameObject = new GameObject();
			std::vector<std::string> childrenNames;
			pugi::xml_object_range<pugi::xml_node_iterator> gameObjectData = sceneNode.children();

			for (pugi::xml_node_iterator gameobjectsDataIterator = gameObjectData.begin();
				gameobjectsDataIterator != gameObjectData.end(); gameobjectsDataIterator++)
			{
				pugi::xml_node gameObjectNode = *gameobjectsDataIterator;
				std::string nodeName = gameObjectNode.name();
				if (nodeName == "name")
				{
					gameObject->name = gameObjectNode.child_value();
				}
				if (nodeName == "enabled")
				{
					std::string isEnabled = gameObjectNode.child_value();
					if (isEnabled == "1")
						gameObject->enabled = true;
					else gameObject->enabled = false;
				}
				if (nodeName == "components")
				{
					pugi::xml_object_range<pugi::xml_node_iterator> 
						componentsNodesChildren = gameObjectNode.children();
					for (pugi::xml_node_iterator componentsNodeIterator = componentsNodesChildren.begin();
						componentsNodeIterator != componentsNodesChildren.end();
						componentsNodeIterator++)
					{
						pugi::xml_node componentNode = *componentsNodeIterator;
						std::string componentNodeName = componentNode.name();
						if (componentNodeName == "transform")
						{
							pugi::xml_object_range<pugi::xml_node_iterator>
								transformNodeChildren = componentNode.children();
							for (pugi::xml_node_iterator transformNodeIterator = transformNodeChildren.begin();
								transformNodeIterator != transformNodeChildren.end();
								transformNodeIterator++)
							{
								pugi::xml_node transformNode = *transformNodeIterator;
								std::string transformNodeName = transformNode.name();
								if (transformNodeName == "position")
								{
									pugi::xml_object_range<pugi::xml_node_iterator>
										positionNodeChildren = transformNode.children();
									for (pugi::xml_node_iterator positionNodeIterator = positionNodeChildren.begin();
										positionNodeIterator != positionNodeChildren.end();
										positionNodeIterator++)
									{
										pugi::xml_node positionNode = *positionNodeIterator;
										std::string positionNodeName = positionNode.name();
										if (positionNodeName == "x")
											gameObject->transform->position.x = 
											std::stof(positionNode.child_value());
										if (positionNodeName == "y")
											gameObject->transform->position.y =
											std::stof(positionNode.child_value()); 
										if (positionNodeName == "z")
											gameObject->transform->position.z =
											std::stof(positionNode.child_value());
									}
								}
								if (transformNodeName == "rotation")
								{
									pugi::xml_object_range<pugi::xml_node_iterator>
										rotationNodeChildren = transformNode.children();
									for (pugi::xml_node_iterator rotationNodeIterator =
										rotationNodeChildren.begin();
										rotationNodeIterator != rotationNodeChildren.end();
										rotationNodeIterator++)
									{
										pugi::xml_node rotationNode = *rotationNodeIterator;
										std::string rotationNodeName = rotationNode.name();
										if (rotationNodeName == "x")
											gameObject->transform->rotation.x =
											std::stof(rotationNode.child_value());
										if (rotationNodeName == "y")
											gameObject->transform->rotation.y =
											std::stof(rotationNode.child_value());
										if (rotationNodeName == "z")
											gameObject->transform->rotation.z =
											std::stof(rotationNode.child_value());
									}
								}
								if (transformNodeName == "scale")
								{
									pugi::xml_object_range<pugi::xml_node_iterator>
										scaleNodeChildren = transformNode.children();
									for (pugi::xml_node_iterator scaleNodeIterator = scaleNodeChildren.begin();
										scaleNodeIterator != scaleNodeChildren.end();
										scaleNodeIterator++)
									{
										pugi::xml_node scaleNode = *scaleNodeIterator;
										std::string scaleNodeName = scaleNode.name();
										if (scaleNodeName == "x")
											gameObject->transform->scale.x =
											std::stof(scaleNode.child_value());
										if (scaleNodeName == "y")
											gameObject->transform->scale.y =
											std::stof(scaleNode.child_value());
										if (scaleNodeName == "z")
											gameObject->transform->scale.z =
											std::stof(scaleNode.child_value());
									}
								}
							}
						}
						if (componentNodeName == "camera")
						{
							Camera* camera = new Camera();
							pugi::xml_object_range<pugi::xml_node_iterator>
								cameraNodeChildren = componentNode.children();
							for (pugi::xml_node_iterator cameraNodeIterator = cameraNodeChildren.begin();
								cameraNodeIterator != cameraNodeChildren.end();
								cameraNodeIterator++)
							{
								pugi::xml_node cameraNode = *cameraNodeIterator;
								std::string cameraNodeName = cameraNode.name();
								if (cameraNodeName == "priority")
									camera->priority = std::stoi(cameraNode.child_value());
							}
							camera->gameObjectName = gameObject->name;
							gameObject->components.push_back(camera);
						}
						if (componentNodeName == "light")
						{
							Light* light = new Light();
							pugi::xml_object_range<pugi::xml_node_iterator>
								lightNodeChildren = componentNode.children();
							//continue;
							for (pugi::xml_node_iterator lightNodeIterator = lightNodeChildren.begin();
								lightNodeIterator != lightNodeChildren.end();
								lightNodeIterator++)
							{
								pugi::xml_node lightNode = *lightNodeIterator;
								std::string lightNodeName = lightNode.name();
								if (lightNodeName == "diffuse")
								{
									pugi::xml_object_range<pugi::xml_node_iterator>
										diffuseNodeChildren = lightNode.children();
									for (pugi::xml_node_iterator diffuseNodeIterator = diffuseNodeChildren.begin();
										diffuseNodeIterator != diffuseNodeChildren.end();
										diffuseNodeIterator++)
									{
										pugi::xml_node diffuseNode = *diffuseNodeIterator;
										std::string diffuseNodeName = diffuseNode.name();
										if (diffuseNodeName == "r")
											light->diffuse.x =
											std::stof(diffuseNode.child_value());

										if (diffuseNodeName == "g")
											light->diffuse.y =
											std::stof(diffuseNode.child_value());

										if (diffuseNodeName == "b")
											light->diffuse.z =
											std::stof(diffuseNode.child_value());

										if (diffuseNodeName == "w")
											light->diffuse.w =
											std::stof(diffuseNode.child_value());
									}
								}
								if (lightNodeName == "specular")
								{
									pugi::xml_object_range<pugi::xml_node_iterator>
										sepcularNodeChildren = lightNode.children();
									for (pugi::xml_node_iterator sepcularNodeIterator = sepcularNodeChildren.begin();
										sepcularNodeIterator != sepcularNodeChildren.end();
										sepcularNodeIterator++)
									{
										pugi::xml_node sepcularNode = *sepcularNodeIterator;
										std::string sepcularNodeName = sepcularNode.name();
										if (sepcularNodeName == "r")
											light->specular.x =
											std::stof(sepcularNode.child_value());

										if (sepcularNodeName == "g")
											light->specular.y =
											std::stof(sepcularNode.child_value());

										if (sepcularNodeName == "b")
											light->specular.z =
											std::stof(sepcularNode.child_value());

										if (sepcularNodeName == "w")
											light->specular.w =
											std::stof(sepcularNode.child_value());
									}
								}
								if (lightNodeName == "atten")
								{
									pugi::xml_object_range<pugi::xml_node_iterator>
										attenNodeChildren = lightNode.children();
									for (pugi::xml_node_iterator attenNodeIterator = attenNodeChildren.begin();
										attenNodeIterator != attenNodeChildren.end();
										attenNodeIterator++)
									{
										pugi::xml_node attenNode = *attenNodeIterator;
										std::string attenNodeName = attenNode.name();
										if (attenNodeName == "x")
											light->atten.x =
											std::stof(attenNode.child_value());

										if (attenNodeName == "y")
											light->atten.y =
											std::stof(attenNode.child_value());

										if (attenNodeName == "z")
											light->atten.z =
											std::stof(attenNode.child_value());

										if (attenNodeName == "w")
											light->atten.w =
											std::stof(attenNode.child_value());
									}
								}
								if (lightNodeName == "direction")
								{
									pugi::xml_object_range<pugi::xml_node_iterator>
										directionNodeChildren = lightNode.children();
									for (pugi::xml_node_iterator directionNodeIterator = directionNodeChildren.begin();
										directionNodeIterator != directionNodeChildren.end();
										directionNodeIterator++)
									{
										pugi::xml_node directionNode = *directionNodeIterator;
										std::string directionNodeName = directionNode.name();
										if (directionNodeName == "x")
											light->direction.x =
											std::stof(directionNode.child_value());

										if (directionNodeName == "y")
											light->direction.y =
											std::stof(directionNode.child_value());

										if (directionNodeName == "z")
											light->direction.z =
											std::stof(directionNode.child_value());

										if (directionNodeName == "w")
											light->direction.w =
											std::stof(directionNode.child_value());
									}
								}
								if (lightNodeName == "param1")
								{
									pugi::xml_object_range<pugi::xml_node_iterator>
										param1NodeChildren = lightNode.children();
									for (pugi::xml_node_iterator param1NodeIterator = param1NodeChildren.begin();
										param1NodeIterator != param1NodeChildren.end();
										param1NodeIterator++)
									{
										pugi::xml_node param1Node = *param1NodeIterator;
										std::string param1NodeName = param1Node.name();
										if (param1NodeName == "x")
											light->param1.x =
											std::stof(param1Node.child_value());

										if (param1NodeName == "y")
											light->param1.y =
											std::stof(param1Node.child_value());

										if (param1NodeName == "z")
											light->param1.z =
											std::stof(param1Node.child_value());

										if (param1NodeName == "w")
											light->param1.w =
											std::stof(param1Node.child_value());
									}
								}
								if (lightNodeName == "param2")
								{
									pugi::xml_object_range<pugi::xml_node_iterator>
										param2NodeChildren = lightNode.children();
									for (pugi::xml_node_iterator param2NodeIterator = param2NodeChildren.begin();
										param2NodeIterator != param2NodeChildren.end();
										param2NodeIterator++)
									{
										pugi::xml_node param2Node = *param2NodeIterator;
										std::string param2NodeName = param2Node.name();
										if (param2NodeName == "x")
											light->param2.x =
											std::stof(param2Node.child_value());

										if (param2NodeName == "y")
											light->param2.y =
											std::stof(param2Node.child_value());

										if (param2NodeName == "z")
											light->param2.z =
											std::stof(param2Node.child_value());

										if (param2NodeName == "w")
											light->param2.w =
											std::stof(param2Node.child_value());
									}
								}
								if (lightNodeName == "type")
								{
									int lightType = std::stoi(lightNode.child_value());
									if (lightType == 1)
										light->lightType = Light::LIGHT_TYPE::POINT_LIGHT;
									if (lightType == 2)
										light->lightType = Light::LIGHT_TYPE::SPOT_LIGHT;
									if (lightType == 3)
										light->lightType = Light::LIGHT_TYPE::DIRECTIONAL_LIGHT;
								}
								if (lightNodeName == "range")
								{
									light->Range = std::stof(lightNode.child_value());
								}
								if (lightNodeName == "intensity")
								{
									light->Intensity = std::stof(lightNode.child_value());
								}
								if (lightNodeName == "mode")
								{
									light->Mode = std::stoi(lightNode.child_value());
								}
							}
							light->position = glm::vec4(gameObject->transform->position,1.f);
							light->gameObjectName = gameObject->name;
							gameObject->components.push_back(light);
							
							::g_pTheLightManager->AddNewLightInfo(light);
							::g_pTheLightManager->LoadLightUniformLocation(shaderID);
						}
						if (componentNodeName == "rigidbody")
						{
							RigidBody* rigidbody = new RigidBody();
							pugi::xml_object_range<pugi::xml_node_iterator>
								rigidbodyNodeChildren = componentNode.children();
							for (pugi::xml_node_iterator rigidbodyNodeIterator = rigidbodyNodeChildren.begin();
								rigidbodyNodeIterator != rigidbodyNodeChildren.end();
								rigidbodyNodeIterator++)
							{
								pugi::xml_node rigidbodyNode = *rigidbodyNodeIterator;
								std::string rigidbodyNodeName = rigidbodyNode.name();
								if (rigidbodyNodeName == "acceleration")
								{
									pugi::xml_object_range<pugi::xml_node_iterator>
										accelerationNodeChildren = rigidbodyNode.children();
									for (pugi::xml_node_iterator accelerationNodeIterator = accelerationNodeChildren.begin();
										accelerationNodeIterator != accelerationNodeChildren.end();
										accelerationNodeIterator++)
									{
										pugi::xml_node accelerationNode = *accelerationNodeIterator;
										std::string accelerationNodeName = accelerationNode.name();
										if (accelerationNodeName == "x")
											rigidbody->acceleration.x =
											std::stof(accelerationNode.child_value());
										if (accelerationNodeName == "y")
											rigidbody->acceleration.y =
											std::stof(accelerationNode.child_value());
										if (accelerationNodeName == "z")
											rigidbody->acceleration.z =
											std::stof(accelerationNode.child_value());
									}
								}
								if (rigidbodyNodeName == "mass")
								{
									rigidbody->mass =
										std::stof(rigidbodyNode.child_value());
								}
							}
							gameObject->components.push_back(rigidbody);
						}
						if (componentNodeName == "boxcollider")
						{
							BoxCollider* boxCollider = new BoxCollider();
							sModelDrawInfo modelDrawInfo;
							if (!this->mainVAOManager->FindDrawInfoByModelName("boxcollider", modelDrawInfo))
							{
								if (this->LoadPlyFiles(boxCollider->box_model_path, modelDrawInfo))
									this->mainVAOManager->LoadModelIntoVAO("boxcollider", modelDrawInfo, shaderID);
							}
							pugi::xml_object_range<pugi::xml_node_iterator>
								boxColliderNodeChildren = componentNode.children();
							for (pugi::xml_node_iterator boxColliderNodeIterator = boxColliderNodeChildren.begin();
								boxColliderNodeIterator != boxColliderNodeChildren.end();
								boxColliderNodeIterator++)
							{
								pugi::xml_node boxColliderNode = *boxColliderNodeIterator;
								std::string boxColliderNodeName = boxColliderNode.name();
								if (boxColliderNodeName == "minposition")
								{
									pugi::xml_object_range<pugi::xml_node_iterator>
										minpositionNodeChildren = boxColliderNode.children();
									for (pugi::xml_node_iterator minpositionNodeIterator = minpositionNodeChildren.begin();
										minpositionNodeIterator != minpositionNodeChildren.end();
										minpositionNodeIterator++)
									{
										pugi::xml_node minpositionNode = *minpositionNodeIterator;
										std::string minpositionNodeName = minpositionNode.name();
										if (minpositionNodeName == "x")
											boxCollider->minPosition.x =
											std::stof(minpositionNode.child_value());
										if (minpositionNodeName == "y")
											boxCollider->minPosition.y =
											std::stof(minpositionNode.child_value());
										if (minpositionNodeName == "z")
											boxCollider->minPosition.z =
											std::stof(minpositionNode.child_value());
									}
								}
								if (boxColliderNodeName == "maxposition")
								{
									pugi::xml_object_range<pugi::xml_node_iterator>
										maxpositionNodeChildren = boxColliderNode.children();
									for (pugi::xml_node_iterator maxpositionNodeIterator = maxpositionNodeChildren.begin();
										maxpositionNodeIterator != maxpositionNodeChildren.end();
										maxpositionNodeIterator++)
									{
										pugi::xml_node maxpositionNode = *maxpositionNodeIterator;
										std::string maxpositionNodeName = maxpositionNode.name();
										if (maxpositionNodeName == "x")
											boxCollider->maxPosition.x =
											std::stof(maxpositionNode.child_value());
										if (maxpositionNodeName == "y")
											boxCollider->maxPosition.y =
											std::stof(maxpositionNode.child_value());
										if (maxpositionNodeName == "z")
											boxCollider->maxPosition.z =
											std::stof(maxpositionNode.child_value());
									}
								}
							}
							gameObject->components.push_back(boxCollider);
						}
					}
				}
				if (nodeName == "meshobject")
				{
					MeshObject* meshObject = new MeshObject();
					for (pugi::xml_node_iterator meshNodeIterator = gameObjectNode.children().begin();
						meshNodeIterator != gameObjectNode.children().end();
						meshNodeIterator++)
					{
						pugi::xml_node meshNode = *meshNodeIterator;
						std::string meshNodeName = meshNode.name();
						if (meshNodeName == "name")
							meshObject->meshName = meshNode.child_value();
						if (meshNodeName == "path")
						{
							meshObject->path = meshNode.child_value();
						}
						if (meshNodeName == "nuselight")
						{
							int nuselight = std::stoi(meshNode.child_value());
							if (nuselight)
								meshObject->bDoNotLight = true;
							else meshObject->bDoNotLight = false;
						}
						if (meshNodeName == "usergba")
						{
							int usergba = std::stoi(meshNode.child_value());
							if (usergba)
								meshObject->bUse_RGBA_color = true;
							else meshObject->bUse_RGBA_color = false;
						}
						if (meshNodeName == "wireframe")
						{
							int wireframe = std::stoi(meshNode.child_value());
							if (wireframe)
								meshObject->isWireframe = true;
							else meshObject->isWireframe = false;
						}
						if (meshNodeName == "rgba")
						{
							for (pugi::xml_node_iterator rgbaNodeIterator
								= meshNode.children().begin();
								rgbaNodeIterator != meshNode.children().end();
								rgbaNodeIterator++)
							{
								pugi::xml_node rgbaNode = *rgbaNodeIterator;
								std::string rgbaNodeName = rgbaNode.name();
								if (rgbaNodeName == "r")
									meshObject->RGBA_color.r = std::stof(rgbaNode.child_value());
								if (rgbaNodeName == "g")
									meshObject->RGBA_color.g = std::stof(rgbaNode.child_value());
								if (rgbaNodeName == "b")
									meshObject->RGBA_color.b = std::stof(rgbaNode.child_value());
								if (rgbaNodeName == "a")
									meshObject->RGBA_color.a = std::stof(rgbaNode.child_value());
							}
						}
					}
					sModelDrawInfo modelDrawInfo;
					if (this->LoadPlyFiles(meshObject->path, modelDrawInfo))
					{
						pVAOManager->LoadModelIntoVAO(meshObject->meshName, modelDrawInfo, shaderID);
						meshObject->minPoint = modelDrawInfo.minValues;
						meshObject->maxPoint = modelDrawInfo.maxValues;
						meshObject->halfExtent = (meshObject->maxPoint - meshObject->minPoint) / 2.f;
						meshObject->centerPoint = meshObject->minPoint + meshObject->halfExtent;
						gameObject->meshObject = meshObject;
					}
					else
					{
						if (pVAOManager->FindDrawInfoByModelName(meshObject->meshName, modelDrawInfo))
						{
							meshObject->minPoint = modelDrawInfo.minValues;
							meshObject->maxPoint = modelDrawInfo.maxValues;
							meshObject->halfExtent = (meshObject->maxPoint - meshObject->minPoint) / 2.f;
							meshObject->centerPoint = meshObject->minPoint + meshObject->halfExtent;
							gameObject->meshObject = meshObject;
						}
					}
				}
				if (nodeName == "children")
				{
					pugi::xml_object_range<pugi::xml_node_iterator>
						childrenNodesChildren = gameObjectNode.children();
					for (pugi::xml_node_iterator childrenNodesIterator = childrenNodesChildren.begin();
						childrenNodesIterator != childrenNodesChildren.end();
						childrenNodesIterator++)
					{
						pugi::xml_node childrenNode = *childrenNodesIterator;
						std::string childrenNodeName = childrenNode.name();
						if (childrenNodeName == "name")
						{
							childrenNames.push_back(childrenNode.child_value());
						}
					}
				}
			}
			if (gameObject->name == "EDITOR_CAMERA")
			{
				SceneEditor::EDITOR_CAMERA = gameObject;
				mainCamera = EDITOR_CAMERA;
				continue;
			}
			if (childrenNames.size() > 0)
			{
				for (int p = 0; p < childrenNames.size(); p++)
				{
					GameObject* childobject = this->GetGameObjectByName(childrenNames[p]);
					if (childobject != nullptr)
					{
						childobject->parent = gameObject;
						gameObject->children.push_back(childobject);
					}
				}
			}
			list_GameObjects.push_back(gameObject);
		}
	}

	player->player = GetGameObjectByName("Camera");
	player->bullet = GetGameObjectByName("bullet");
	return true;
}

bool SceneEditor::SaveSceneFile()
{
	pugi::xml_document doc;
	pugi::xml_node saveNode = doc.append_child("scene");
	pugi::xml_node sceneNameNode = saveNode.append_child("name");
	sceneNameNode.append_child(pugi::node_pcdata).set_value(this->sceneName.c_str());
	pugi::xml_node gameobjectNode = saveNode.append_child("gameobject");
	pugi::xml_node nameNode = gameobjectNode.append_child("name");
	nameNode.append_child(pugi::node_pcdata).set_value("EDITOR_CAMERA");
	pugi::xml_node componentNode = gameobjectNode.append_child("components");
	pugi::xml_node transformNode = componentNode.append_child("transform");
	pugi::xml_node positionNode = transformNode.append_child("position");
	{
		pugi::xml_node xNode = positionNode.append_child("x");
		xNode.append_child(pugi::node_pcdata).set_value(
			std::to_string(EDITOR_CAMERA->transform->position.x).c_str());
		pugi::xml_node yNode = positionNode.append_child("y");
		yNode.append_child(pugi::node_pcdata).set_value(
			std::to_string(EDITOR_CAMERA->transform->position.y).c_str());
		pugi::xml_node zNode = positionNode.append_child("z");
		zNode.append_child(pugi::node_pcdata).set_value(
			std::to_string(EDITOR_CAMERA->transform->position.z).c_str());
	}
	pugi::xml_node rotationNode = transformNode.append_child("rotation");
	{
		pugi::xml_node xNode = rotationNode.append_child("x");
		xNode.append_child(pugi::node_pcdata).set_value(
			std::to_string(EDITOR_CAMERA->transform->rotation.x).c_str());
		pugi::xml_node yNode = rotationNode.append_child("y");
		yNode.append_child(pugi::node_pcdata).set_value(
			std::to_string(EDITOR_CAMERA->transform->rotation.y).c_str());
		pugi::xml_node zNode = rotationNode.append_child("z");
		zNode.append_child(pugi::node_pcdata).set_value(
			std::to_string(EDITOR_CAMERA->transform->rotation.z).c_str());
	}
	pugi::xml_node scaleNode = transformNode.append_child("scale");
	{
		pugi::xml_node xNode = scaleNode.append_child("x");
		xNode.append_child(pugi::node_pcdata).set_value(
			std::to_string(EDITOR_CAMERA->transform->scale.x).c_str());
		pugi::xml_node yNode = scaleNode.append_child("y");
		yNode.append_child(pugi::node_pcdata).set_value(
			std::to_string(EDITOR_CAMERA->transform->scale.y).c_str());
		pugi::xml_node zNode = scaleNode.append_child("z");
		zNode.append_child(pugi::node_pcdata).set_value(
			std::to_string(EDITOR_CAMERA->transform->scale.z).c_str());
	}

	for (int i = 0; i < this->list_GameObjects.size(); i++)
	{
		pugi::xml_node gameobjectNode = saveNode.append_child("gameobject");
		pugi::xml_node nameNode = gameobjectNode.append_child("name");
		nameNode.append_child(pugi::node_pcdata).set_value(this->list_GameObjects[i]->name.c_str());
		pugi::xml_node enabledNode = gameobjectNode.append_child("enabled");
		if (list_GameObjects[i]->enabled)
			enabledNode.append_child(pugi::node_pcdata).set_value("1");
		else enabledNode.append_child(pugi::node_pcdata).set_value("0");
		pugi::xml_node componentNode = gameobjectNode.append_child("components");
		pugi::xml_node transformNode = componentNode.append_child("transform");
		pugi::xml_node positionNode = transformNode.append_child("position");
		{
			pugi::xml_node xNode = positionNode.append_child("x");
			xNode.append_child(pugi::node_pcdata).set_value(
				std::to_string(this->list_GameObjects[i]->transform->position.x).c_str());
			pugi::xml_node yNode = positionNode.append_child("y");
			yNode.append_child(pugi::node_pcdata).set_value(
				std::to_string(this->list_GameObjects[i]->transform->position.y).c_str());
			pugi::xml_node zNode = positionNode.append_child("z");
			zNode.append_child(pugi::node_pcdata).set_value(
				std::to_string(this->list_GameObjects[i]->transform->position.z).c_str());
		}
		pugi::xml_node rotationNode = transformNode.append_child("rotation");
		{
			pugi::xml_node xNode = rotationNode.append_child("x");
			xNode.append_child(pugi::node_pcdata).set_value(
				std::to_string(this->list_GameObjects[i]->transform->rotation.x).c_str());
			pugi::xml_node yNode = rotationNode.append_child("y");
			yNode.append_child(pugi::node_pcdata).set_value(
				std::to_string(this->list_GameObjects[i]->transform->rotation.y).c_str());
			pugi::xml_node zNode = rotationNode.append_child("z");
			zNode.append_child(pugi::node_pcdata).set_value(
				std::to_string(this->list_GameObjects[i]->transform->rotation.z).c_str());
		}
		pugi::xml_node scaleNode = transformNode.append_child("scale");
		{
			pugi::xml_node xNode = scaleNode.append_child("x");
			xNode.append_child(pugi::node_pcdata).set_value(
				std::to_string(this->list_GameObjects[i]->transform->scale.x).c_str());
			pugi::xml_node yNode = scaleNode.append_child("y");
			yNode.append_child(pugi::node_pcdata).set_value(
				std::to_string(this->list_GameObjects[i]->transform->scale.y).c_str());
			pugi::xml_node zNode = scaleNode.append_child("z");
			zNode.append_child(pugi::node_pcdata).set_value(
				std::to_string(this->list_GameObjects[i]->transform->scale.z).c_str());
		}
		for (int j = 0; j < this->list_GameObjects[i]->components.size(); j++)
		{
			iComponent* component = this->list_GameObjects[i]->components[j];
			if (component->componentType == "camera")
			{
				Camera* camera = (Camera*)component;
				pugi::xml_node cameraNode = componentNode.append_child("camera");
				pugi::xml_node priorityNode = cameraNode.append_child("priority");
				priorityNode.append_child(pugi::node_pcdata).set_value(std::to_string(camera->priority).c_str());
			}
			if (component->componentType == "light")
			{
				Light* light = (Light*)component;
				pugi::xml_node lightNode = componentNode.append_child("light");
				pugi::xml_node diffuseNode = lightNode.append_child("diffuse");
				{
					pugi::xml_node rNode = diffuseNode.append_child("r");
					rNode.append_child(pugi::node_pcdata).set_value(std::to_string(light->diffuse.r).c_str());
					
					pugi::xml_node gNode = diffuseNode.append_child("g");
					gNode.append_child(pugi::node_pcdata).set_value(std::to_string(light->diffuse.g).c_str());
					
					pugi::xml_node bNode = diffuseNode.append_child("b");
					bNode.append_child(pugi::node_pcdata).set_value(std::to_string(light->diffuse.b).c_str());
					
					pugi::xml_node wNode = diffuseNode.append_child("w");
					wNode.append_child(pugi::node_pcdata).set_value(std::to_string(light->diffuse.w).c_str());
				}
				pugi::xml_node specularNode = lightNode.append_child("specular");
				{
					pugi::xml_node rNode = specularNode.append_child("r");
					rNode.append_child(pugi::node_pcdata).set_value(std::to_string(light->specular.r).c_str());
					
					pugi::xml_node gNode = specularNode.append_child("g");
					gNode.append_child(pugi::node_pcdata).set_value(std::to_string(light->specular.g).c_str());
					
					pugi::xml_node bNode = specularNode.append_child("b");
					bNode.append_child(pugi::node_pcdata).set_value(std::to_string(light->specular.b).c_str());
					
					pugi::xml_node wNode = specularNode.append_child("w");
					wNode.append_child(pugi::node_pcdata).set_value(std::to_string(light->specular.w).c_str());
				}
				pugi::xml_node attenNode = lightNode.append_child("atten");
				{
					pugi::xml_node xNode = attenNode.append_child("x");
					xNode.append_child(pugi::node_pcdata).set_value(std::to_string(light->atten.x).c_str());
					
					pugi::xml_node yNode = attenNode.append_child("y");
					yNode.append_child(pugi::node_pcdata).set_value(std::to_string(light->atten.y).c_str());
					
					pugi::xml_node zNode = attenNode.append_child("z");
					zNode.append_child(pugi::node_pcdata).set_value(std::to_string(light->atten.z).c_str());
					
					pugi::xml_node wNode = attenNode.append_child("w");
					wNode.append_child(pugi::node_pcdata).set_value(std::to_string(light->atten.w).c_str());
				}
				pugi::xml_node param1Node = lightNode.append_child("param1");
				{
					pugi::xml_node xNode = param1Node.append_child("x");
					xNode.append_child(pugi::node_pcdata).set_value(std::to_string(light->param1.x).c_str());
					
					pugi::xml_node yNode = param1Node.append_child("y");
					yNode.append_child(pugi::node_pcdata).set_value(std::to_string(light->param1.y).c_str());
					
					pugi::xml_node zNode = param1Node.append_child("z");
					zNode.append_child(pugi::node_pcdata).set_value(std::to_string(light->param1.z).c_str());
					
					pugi::xml_node wNode = param1Node.append_child("w");
					wNode.append_child(pugi::node_pcdata).set_value(std::to_string(light->param1.w).c_str());
				}
				pugi::xml_node param2Node = lightNode.append_child("param2");
				{
					pugi::xml_node xNode = param2Node.append_child("x");
					xNode.append_child(pugi::node_pcdata).set_value(std::to_string(light->param2.x).c_str());
					
					pugi::xml_node yNode = param2Node.append_child("y");
					yNode.append_child(pugi::node_pcdata).set_value(std::to_string(light->param2.y).c_str());
					
					pugi::xml_node zNode = param2Node.append_child("z");
					zNode.append_child(pugi::node_pcdata).set_value(std::to_string(light->param2.z).c_str());
					
					pugi::xml_node wNode = param2Node.append_child("w");
					wNode.append_child(pugi::node_pcdata).set_value(std::to_string(light->param2.w).c_str());
				}
				pugi::xml_node typeNode = lightNode.append_child("type");
				typeNode.append_child(pugi::node_pcdata).set_value(std::to_string(light->lightType).c_str());
				pugi::xml_node rangeNode = lightNode.append_child("range");
				rangeNode.append_child(pugi::node_pcdata).set_value(std::to_string(light->Range).c_str());
				pugi::xml_node intensityNode = lightNode.append_child("intensity");
				intensityNode.append_child(pugi::node_pcdata).set_value(std::to_string(light->Intensity).c_str());
				pugi::xml_node modeNode = lightNode.append_child("mode");
				modeNode.append_child(pugi::node_pcdata).set_value(std::to_string(light->Mode).c_str());
			}
			if (component->componentType == "rigidbody")
			{
				RigidBody* rigidbody = (RigidBody*)component;
				pugi::xml_node cameraNode = componentNode.append_child("rigidbody");
				pugi::xml_node massNode = cameraNode.append_child("mass");
				massNode.append_child(pugi::node_pcdata).set_value(std::to_string(rigidbody->mass).c_str());
				pugi::xml_node accNode = cameraNode.append_child("acceleration");
				{
					pugi::xml_node xNode = accNode.append_child("x");
					xNode.append_child(pugi::node_pcdata).set_value(std::to_string(rigidbody->acceleration.x).c_str());

					pugi::xml_node yNode = accNode.append_child("y");
					yNode.append_child(pugi::node_pcdata).set_value(std::to_string(rigidbody->acceleration.y).c_str());

					pugi::xml_node zNode = accNode.append_child("z");
					zNode.append_child(pugi::node_pcdata).set_value(std::to_string(rigidbody->acceleration.z).c_str());
				}
			}
			if (component->componentType == "boxcollider")
			{
				BoxCollider* boxCollider = (BoxCollider*)component;
				pugi::xml_node boxColliderNode = componentNode.append_child("boxcollider");
				pugi::xml_node minPositionNode = boxColliderNode.append_child("minposition");
				{
					pugi::xml_node xNode = minPositionNode.append_child("x");
					xNode.append_child(pugi::node_pcdata).set_value(
						std::to_string(this->list_GameObjects[i]->meshObject->minPoint.x).c_str());
					pugi::xml_node yNode = minPositionNode.append_child("y");
					yNode.append_child(pugi::node_pcdata).set_value(
						std::to_string(this->list_GameObjects[i]->meshObject->minPoint.y).c_str());
					pugi::xml_node zNode = minPositionNode.append_child("z");
					zNode.append_child(pugi::node_pcdata).set_value(
						std::to_string(this->list_GameObjects[i]->meshObject->minPoint.z).c_str());
				}
				pugi::xml_node maxPositionNode = boxColliderNode.append_child("maxposition");
				{
					pugi::xml_node xNode = maxPositionNode.append_child("x");
					xNode.append_child(pugi::node_pcdata).set_value(
						std::to_string(this->list_GameObjects[i]->meshObject->maxPoint.x).c_str());
					pugi::xml_node yNode = maxPositionNode.append_child("y");
					yNode.append_child(pugi::node_pcdata).set_value(
						std::to_string(this->list_GameObjects[i]->meshObject->maxPoint.y).c_str());
					pugi::xml_node zNode = maxPositionNode.append_child("z");
					zNode.append_child(pugi::node_pcdata).set_value(
						std::to_string(this->list_GameObjects[i]->meshObject->maxPoint.z).c_str());
				}
			}
		}
		if (this->list_GameObjects[i]->children.size())
		{
			pugi::xml_node childrenNode = gameobjectNode.append_child("children");
			for (int k = 0; k < this->list_GameObjects[i]->children.size(); k++)
			{
				pugi::xml_node childNode = childrenNode.append_child("name");
				childNode.append_child(pugi::node_pcdata)
					.set_value(this->list_GameObjects[i]->children[k]->name.c_str());
			}
		}
		if (this->list_GameObjects[i]->meshObject != nullptr)
		{
			pugi::xml_node meshObjectNode = gameobjectNode.append_child("meshobject");
			pugi::xml_node meshObjectNameNode = meshObjectNode.append_child("name");
			meshObjectNameNode.append_child(pugi::node_pcdata).set_value(this->list_GameObjects[i]->meshObject->meshName.c_str());

			pugi::xml_node meshObjectPathNode = meshObjectNode.append_child("path");
			meshObjectPathNode.append_child(pugi::node_pcdata).set_value(this->list_GameObjects[i]->meshObject->path.c_str());
			
			pugi::xml_node meshObjectNuseLightNode = meshObjectNode.append_child("nuselight");
			if(this->list_GameObjects[i]->meshObject->bDoNotLight)
				meshObjectNuseLightNode.append_child(pugi::node_pcdata).set_value("1");
			else
				meshObjectNuseLightNode.append_child(pugi::node_pcdata).set_value("0");

			pugi::xml_node meshObjectUseRgbaNode = meshObjectNode.append_child("usergba");
			if(this->list_GameObjects[i]->meshObject->bUse_RGBA_color)
				meshObjectUseRgbaNode.append_child(pugi::node_pcdata).set_value("1");
			else
				meshObjectUseRgbaNode.append_child(pugi::node_pcdata).set_value("0");

			pugi::xml_node meshObjectRGBANode = meshObjectNode.append_child("rgba");
			pugi::xml_node rNode = meshObjectRGBANode.append_child("r");
			rNode.append_child(pugi::node_pcdata).set_value(
				std::to_string(this->list_GameObjects[i]->meshObject->RGBA_color.r).c_str());

			pugi::xml_node gNode = meshObjectRGBANode.append_child("g");
			gNode.append_child(pugi::node_pcdata).set_value(
				std::to_string(this->list_GameObjects[i]->meshObject->RGBA_color.g).c_str());

			pugi::xml_node bNode = meshObjectRGBANode.append_child("b");
			bNode.append_child(pugi::node_pcdata).set_value(
				std::to_string(this->list_GameObjects[i]->meshObject->RGBA_color.b).c_str());

			pugi::xml_node aNode = meshObjectRGBANode.append_child("a");
			aNode.append_child(pugi::node_pcdata).set_value(
				std::to_string(this->list_GameObjects[i]->meshObject->RGBA_color.a).c_str());

			pugi::xml_node wireframeNode = meshObjectNode.append_child("wireframe");
			if (this->list_GameObjects[i]->meshObject->isWireframe)
				wireframeNode.append_child(pugi::node_pcdata).set_value("1");
			else
				wireframeNode.append_child(pugi::node_pcdata).set_value("0");
		}
	}
	doc.save_file((sceneFileName).c_str());
	return true;
}
