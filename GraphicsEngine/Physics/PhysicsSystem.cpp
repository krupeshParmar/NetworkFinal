#include "PhysicsSystem.h"
#include "PhysicsUtils.h"
#include "../System/Components/RigidBody.h"
#include "../System/Components/BoxCollider.h"
#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

PhysicsSystem::PhysicsSystem() {
}

PhysicsSystem::~PhysicsSystem() {
	for (int i = 0; i < m_PhysicsObjects.size(); i++) {
		delete m_PhysicsObjects[i];
	}
	m_PhysicsObjects.clear();
	for (int i = 0; i < m_GameObjects.size(); i++) {
		delete m_GameObjects[i];
	}
	m_GameObjects.clear();

	// delete[] m_Particles[0];		Can check if this works (Not sure)
	//  -> edit: Nope, use this for array of pointers
}

PhysicsObject* PhysicsSystem::CreatePhysicsObject(const Vector3& position, iShape* shape) {
	PhysicsObject* physicsObject = new PhysicsObject(position);
	physicsObject->pShape = shape;
	m_PhysicsObjects.push_back(physicsObject);
	return physicsObject;
}

void PhysicsSystem::AddSpring(Spring* spring) {
	m_Springs.push_back(spring);
}

void PhysicsSystem::UpdateStep(float duration) {
	size_t numPhysicsObjects = m_PhysicsObjects.size();

	if (numPhysicsObjects == 0)
		return;

	for (int i = 0; i < numPhysicsObjects; i++) {
		/* Gravity */
		m_PhysicsObjects[i]->ApplyForce(Vector3(0, -9.81f, 0));
	}

	for (int i = 0; i < m_Springs.size(); i++) {
		m_Springs[i]->ApplyForces(duration);
	}

	for (int i = 0; i < numPhysicsObjects; i++) {
		m_PhysicsObjects[i]->Integrate(duration);

	}

	/*
	// Detect collisions
	PhysicsObject *physObjA, *physObjB;

	iShape *shapeA, *shapeB;

	bool collision;
	for (int i = 0; i < numPhysicsObjects - 1; i++) {
		physObjA = m_PhysicsObjects[i];
		shapeA = physObjA->pShape;

		for (int j = i + 1; j < numPhysicsObjects; j++) {
			physObjB = m_PhysicsObjects[j];
			shapeB = physObjB->pShape;

			collision = CollisionTest(physObjA->position, shapeA, physObjB->position, shapeB);

			if (collision) {
				if (physObjA->m_IsStatic == false)
				{
					physObjA->position.y = physObjA->prevPosition.y;
					////physObjA->KillAllForces();
					physObjA->velocity.y = 0.0f;


					// Bounce:
					//physObjA->velocity = Vector3(0.0f) - physObjA->velocity * 0.8f;
				}

				if (physObjB->m_IsStatic == false)
				{
					physObjB->position.y = physObjB->prevPosition.y;
					//physObjB->KillAllForces();
					physObjB->velocity.y = 0.0f;

					//physObjB->velocity = Vector3(0.0f) - physObjB->velocity * 0.8f;
				}
			}
		}
	}
	*/

	for (int i = 0; i < numPhysicsObjects; i++) {
		m_PhysicsObjects[i]->KillAllForces();
	}
}

void PhysicsSystem::Update(float time)
{
	for (GameObject* gameObject : m_GameObjects)
	{
		//Get rigidbody and boxCollider of gameObject
		RigidBody* rigidBody = nullptr;
		BoxCollider* boxCollider = nullptr;
		for (int i = 0; i < gameObject->components.size(); i++)
		{
			if (gameObject->components[i]->componentType == "rigidbody")
			{
				rigidBody = (RigidBody*)gameObject->components[i];
			}
			if (gameObject->components[i]->componentType == "boxcollider")
			{
				boxCollider = (BoxCollider*)gameObject->components[i];
			}
		}
		if (!rigidBody)
			continue;

		// There might be some rigidbody related task that can be done
		// however, for collision we do need the box collider
		if (!boxCollider)
			continue;
		for (GameObject* testGameObject : m_GameObjects)
		{
			if (gameObject == testGameObject)
				continue;

			RigidBody* testRigidBody = nullptr;
			BoxCollider* testBoxCollider = nullptr;
			for (int i = 0; i < testGameObject->components.size(); i++)
			{
				if (testGameObject->components[i]->componentType == "rigidbody")
				{
					testRigidBody = (RigidBody*)testGameObject->components[i];
				}
				if (testGameObject->components[i]->componentType == "boxcollider")
				{
					testBoxCollider = (BoxCollider*)testGameObject->components[i];
				}
			}

			if (!testRigidBody)
				continue;

			// There might be some rigidbody related task that can be done
			// however, for collision we do need the box collider
			if (!testBoxCollider)
				continue;
				/*a.minX <= b.maxX &&
				a.maxX >= b.minX &&
				a.minY <= b.maxY &&
				a.maxY >= b.minY &&
				a.minZ <= b.maxZ &&
				a.maxZ >= b.minZ*//*
			std::cout << "A min: " << a_min.x << ", " << a_min.y << ", " << a_min.z << std::endl;
			std::cout << "A max: " << a_max.x << ", " << a_max.y << ", " << a_max.z << std::endl;
			std::cout << "B min: " << b_min.x << ", " << b_min.y << ", " << b_min.z << std::endl;
			std::cout << "B max: " << b_max.x << ", " << b_max.y << ", " << b_max.z << std::endl;
			*/

			if ( CollisionTest(gameObject,testGameObject) )
			{
				gameObject->OnCollisionEnter(testGameObject);
			}
		}
	}
}

bool PhysicsSystem::ClickedTest(const glm::vec2& pointA, const int& width, const int& height)
{
	
	return false;
}

bool PhysicsSystem::CollisionTest(const GameObject* gameObject, const GameObject* testGameObject)
{
	glm::vec3 a_min = gameObject->meshObject->minPoint + gameObject->transform->position;
	glm::vec3 a_max = gameObject->meshObject->maxPoint + gameObject->transform->position;
	glm::vec3 b_min = testGameObject->meshObject->minPoint + testGameObject->transform->position;
	glm::vec3 b_max = testGameObject->meshObject->maxPoint + testGameObject->transform->position;

	if (
		(a_min.x <= b_max.x &&
			a_max.x >= b_min.x) &&
		(a_min.y <= b_max.y &&
			a_max.y >= b_min.y) &&
		(a_min.z <= b_max.z &&
			a_max.z >= b_min.z)
		)
	{
		return true;
	}
	return false;
}

void PhysicsSystem::AddTriangleToAABBCollisionCheck(int hash, Triangle* triangle)
{
	m_AABBStructure[hash].push_back(triangle);
}

bool PhysicsSystem::RayCastClosest(Ray ray, GameObject** gameObject, std::vector<GameObject*> listOfGameObjects)
{
	GameObject* closestGameObject = nullptr;
	float closestDistance = FLT_MAX;

	for (GameObject* itGameObject: listOfGameObjects)
	{
		if (
			!itGameObject->enabled ||
			itGameObject->name == "sphere" ||
			itGameObject->name == "player" ||
			itGameObject->name == "Camera"
			)
			continue;
		float max[3] = {
				itGameObject->meshObject->maxPoint.x + itGameObject->transform->position.x,
				itGameObject->meshObject->maxPoint.y + itGameObject->transform->position.y,
				itGameObject->meshObject->maxPoint.z + itGameObject->transform->position.z
		};
		float min[3] = {
					itGameObject->meshObject->minPoint.x + itGameObject->transform->position.x,
					itGameObject->meshObject->minPoint.y + itGameObject->transform->position.y,
					itGameObject->meshObject->minPoint.z + itGameObject->transform->position.z
		};
		AABB aabb(min, max);
		/*if (TestRaySphere(ray.origin,ray.direction,itGameObject->transform->position + itGameObject->meshObject->centerPoint, .5f))
		{
			float distance = glm::distance(ray.origin, itGameObject->transform->position);
			if (closestDistance > distance)
			{
				closestGameObject = itGameObject;
				closestDistance = distance;
			}
		}*/

		if (TestRayAABB(ray, aabb))
		{
			float distance = glm::distance(ray.origin, itGameObject->transform->position);
			if (closestDistance > distance)
			{
				closestGameObject = itGameObject;
				closestDistance = distance;
			}
		}
	}
	*gameObject = closestGameObject;
	return closestGameObject != nullptr;
}

bool PhysicsSystem::CollisionTest(const Vector3& posA, iShape* shapeA, const Vector3& posB, iShape* shapeB)
{
	bool detectedCollision = false;

	// ShapeA AABB Collision test 
	if (shapeA->GetType() == SHAPE_TYPE_AABB)
	{
		if (shapeB->GetType() == SHAPE_TYPE_AABB) {}

		else if (shapeB->GetType() == SHAPE_TYPE_SPHERE) {
			detectedCollision = CollisionTest(posB, dynamic_cast<Sphere*>(shapeB), posA, dynamic_cast<AABB*>(shapeA));
		}

		else if (shapeB->GetType() == SHAPE_TYPE_TRIANGLE) {}
	}


	// ShapeA Sphere collision tests
	else if (shapeA->GetType() == SHAPE_TYPE_SPHERE)
	{
		if (shapeB->GetType() == SHAPE_TYPE_AABB) {
			detectedCollision = CollisionTest(posA, dynamic_cast<Sphere*>(shapeA), posB, dynamic_cast<AABB*>(shapeB));
		}

		else if (shapeB->GetType() == SHAPE_TYPE_SPHERE) {
			detectedCollision = CollisionTest(posA, dynamic_cast<Sphere*>(shapeA), posB, dynamic_cast<Sphere*>(shapeB));
		}

		else if (shapeB->GetType() == SHAPE_TYPE_TRIANGLE) {
			detectedCollision = CollisionTest(posA, dynamic_cast<Sphere*>(shapeA), posB, dynamic_cast<Triangle*>(shapeB));
		}
	}


	// ShapeA Triangle collision tests
	else if (shapeA->GetType() == SHAPE_TYPE_TRIANGLE)
	{
		if (shapeB->GetType() == SHAPE_TYPE_AABB) {}

		else if (shapeB->GetType() == SHAPE_TYPE_SPHERE) {
			detectedCollision = CollisionTest(posB, dynamic_cast<Sphere*>(shapeB), posA, dynamic_cast<Triangle*>(shapeA));
		}

		else if (shapeB->GetType() == SHAPE_TYPE_TRIANGLE) {}
	}


	// ShapeA is unknown shape...
	else
	{
		// what is this!?!?!
	}

	return detectedCollision;
}

bool PhysicsSystem::CollisionTest(const Vector3& posA, Sphere* a, const Vector3& posB, Sphere* b)
{
	return TestSphereSphere(posA + a->Center, a->Radius, posB + b->Center, b->Radius);
}

bool PhysicsSystem::CollisionTest(const Vector3 &posA, Sphere* a, const Vector3& posB, AABB* b)
{
	return TestSphereAABB(posA + a->Center, a->Radius, *b);
}

bool PhysicsSystem::CollisionTest(const Vector3 &posA, Sphere* a, const Vector3& posB, Triangle* b)
{
	Point unused;
	return TestSphereTriangle(posA + a->Center, a->Radius, posB + (*b).A, posB + (*b).B, posB + (*b).C, unused);
}
