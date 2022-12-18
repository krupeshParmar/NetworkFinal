#pragma once

#include "PhysicsObject.h"

#include <map>
#include <vector>
#include "Spring.h"
#include "Shapes.h"
#include "../GameObject/GameObject.h"

class PhysicsSystem {
public:
	PhysicsSystem();
	~PhysicsSystem();
	std::vector<GameObject*> m_GameObjects;

	PhysicsObject* CreatePhysicsObject(const Vector3& position, iShape* shape);
	void AddSpring(Spring* spring);
	void UpdateStep(float duration);
	void Update(float time);
	void AddTriangleToAABBCollisionCheck(int hash, Triangle* triangle);

	const std::map<int, std::vector<Triangle*>>& GetAABBStructure() {
		return m_AABBStructure;
	}

	bool RayCastClosest(Ray ray,GameObject** gameObject, std::vector<GameObject*> listOfGameObjects);
	bool CollisionTest(const Vector3& posA, iShape* shapeA, const Vector3& posB, iShape* shapeB);
	bool CollisionTest(const Vector3& posA, Sphere* a, const Vector3& posB, Sphere* b);
	bool CollisionTest(const Vector3& posA, Sphere* a, const Vector3& posB, AABB* b);
	bool CollisionTest(const Vector3& posA, Sphere* a, const Vector3& posB, Triangle* b);
	bool ClickedTest(const glm::vec2& pointA, const int &width, const int& height);
	bool CollisionTest(const GameObject* gameObject, const GameObject* testGameObject);

	//bool CollisionTest(AABB* a, Triangle* b);		// Not implementing
	//bool CollisionTest(AABB* a, AABB* b);			// Not implementing
	//bool CollisionTest(Triangle* a, Triangle* b);	// Not implementing

private:
	std::vector<PhysicsObject*> m_PhysicsObjects;
	std::vector<Spring*> m_Springs;
	std::map<int, std::vector<Triangle*>> m_AABBStructure;
};