#pragma once

#include "PhysicsMath.h"
#include <glm/vec3.hpp>

#include <memory>

enum ShapeType {
	SHAPE_TYPE_SPHERE,
	SHAPE_TYPE_AABB,
	SHAPE_TYPE_TRIANGLE
};

class iShape {
public:
	virtual ShapeType GetType() const = 0;
};

class Sphere : public iShape {
public:
	Sphere(const Point& center, float radius)
		: Center(center)
		, Radius(radius) 
	{ }

	virtual ~Sphere() 
	{ }

	virtual ShapeType GetType() const override {
		return SHAPE_TYPE_SPHERE;
	}

	Point Center;
	float Radius;
};

class AABB : public iShape {
public:
	AABB(float min[3], float max[3]) {
		memcpy(&(Min[0]), &(min[0]), 3 * sizeof(float));
		memcpy(&(Max[0]), &(max[0]), 3 * sizeof(float));
	}
	virtual ~AABB() {
	}

	virtual ShapeType GetType() const override {
		return SHAPE_TYPE_AABB;
	}

	// x, y, z will be indexed as 0, 1, 2 respectively
	float Min[3];
	float Max[3];
};

class Triangle : public iShape {
public:
	Triangle(Point a, Point b, Point c)
		: A(a), B(b), C(c){ }
	virtual ~Triangle() { }

	virtual ShapeType GetType() const override {
		return SHAPE_TYPE_TRIANGLE;
	}

	Point A;
	Point B;
	Point C;

};

class Ray {
public:
	Ray(const glm::vec3& p, const glm::vec3& d)
		: origin(p), direction(d) {
		
		float m = 
			sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
		direction.x /= m;
		direction.y /= m;
		direction.z /= m;
	}
	glm::vec3 origin;
	glm::vec3 direction;
};