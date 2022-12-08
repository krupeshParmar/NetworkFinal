#pragma once

#include <map>
#include <vector>
#include <string>

#include "PhysicsDebugRenderer.h"
#include "PhysicsSystem.h"
#include "BoundingBox.h"
#include "Ball.h"

typedef glm::vec3 color;

//struct GameObjectData {
//	unsigned int ModelId;
//	unsigned int MaterialId;
//};

class SimulationView {
public:
	SimulationView();
	~SimulationView();

	void Initialize(int DemoId);
	void Destroy();

	void Update(double dt);
	void PhysicsUpdate(double dt);
	void Render();
private:
	void PrepareDemo();
	Ball* CreateBall(const Vector3& position, float scale);
	void CreateTree(const Vector3& position, float scale);
	void LoadStaticModelToOurAABBEnvironment(const std::string& filepath, const Vector3& position, float scale = 1.0f);
	//void AddGameDataToMap(GameObjectData& data);
	//GameObject* CreateObjectByType(const std::string& type);

	//std::map<std::string, GameObjectData> m_GameObjectDataMap;

	PhysicsSystem m_PhysicsSystem;
	Ball m_ControlledBall;

	BoundingBox m_BallBoundingBox;

	std::vector<Ball> m_Balls;

	unsigned int m_ShipModelId;
	PhysicsDebugRenderer* m_PhysicsDebugRenderer;

	GameObject* m_BigShipGamObject;
};
