#pragma once
#include "../inc_opengl.h"
#include <string>
#include "../Physics/Vector3.h"
#include "../Physics/PhysicsObject.h"
#include "../Physics/PhysicsSystem.h"
#include "../GameObject/GameObject.h"
#include "../SceneEditor.h"
#include "../Physics/PhysicsDebugRenderer.h"
class PhysicsSimulation
{
public:
	PhysicsSimulation();
	~PhysicsSimulation();
	SceneEditor* mainSceneEditor;
	GameObject* m_Aircraft;

	void Initialize(int DemoId);
	void Update(GLFWwindow* window,float dt);
	void Destroy();

	void Update(double dt);
	void PhysicsUpdate(double dt);
	void Render();
private:
	void PrepareDemo();
	void LoadStaticModelToOurAABBEnvironment(
		const std::string& filepath, 
		const Vector3& position,
		float scale = 1.f);

	PhysicsSystem m_PhysicsSystem;
	std::vector<GameObject*> m_AircraftBoundingBoxes;
	std::map<int, GameObject*> m_PartialGameObjects;
	PhysicsDebugRenderer* m_PhysicsDebugRenderer;
	GameObject* m_Environment;
	bool canMoveForward = true;
	bool canMoveBackward = true;
	bool canMoveUpward = true;
	bool canMoveDownward = true;
	bool canMoveRight = true;
	bool canMoveLeft = true;
	Light* frontlight;
	Light* backlight;
	Light* rightlight;
	Light* leftlight;
	Light* uplight;
	Light* downlight;
};

