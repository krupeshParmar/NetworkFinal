#pragma once

#include "PhysicsMath.h"
#include "Shapes.h"
#include "../GameObject/GameObject.h"

int TestSphereSphere(const Vector3& posA, float radiusA, const Vector3& posB, float radiusB);
float SqDistPointAABB(Vector3 p, AABB b);
int TestSphereAABB(const Vector3 &center, float radius, AABB b);
int TestRayAABB(const Ray& ray, AABB b);
int TestRaySphere(const Point& p, const Vector3& d, const Point& center, float radius);
Point ClosestPtPointTriangle(Vector3 p, Vector3 a, Vector3 b, Vector3 c);
int TestSphereTriangle(const Vector3& center, float radius, Vector3 a, Vector3 b, Vector3 c, Vector3& p);