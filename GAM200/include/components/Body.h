#pragma once
#include <Debug.h>

#include "Object.h"
#include "Collision.h"
#include "Transform.h"

enum class Shape
{
	Rectangle,
	Circle,
	Line
};
// Body Component
class Body : public Component
{
public:
	Body() : Component(){};
	virtual Shape GetShape() = 0;
	virtual ComponentType TypeId() const override { return ComponentType::Body; }
};

// Rectangle Object
class Rectangular : public Body
{
public:
	Rectangular() {}
	Rectangular(AABB _aabb) : Body(), aabb(_aabb) {}
	Rectangular(Vec2 min, Vec2 max) : Body() {
		aabb.min = min;
		aabb.max = max;
	}
	AABB aabb;
	virtual Shape GetShape() override
	{
		return Shape::Rectangle;
	}
};

// Circle Object
class Circular : public Body
{
public:
	Circular() {}
	Circular(Circle c) : Body(), circle(c)	{}
	Circular(Vec2 center, float radius) : Body() {
		circle.center = center;
		circle.radius = radius;
	}
	Circle circle;
	virtual Shape GetShape() override
	{
		return Shape::Circle;
	}
};

// Line Object
class Lines : public Body
{
public:
	Lines() {}
	Lines(Line _line) : Body(), line(_line) {}
	Lines(Vec2 pt0, Vec2 pt1) : Body(){
		line.SetPt0(pt0);
		line.SetPt1(pt1);
	}
	Line line;
	virtual Shape GetShape() override
	{
		return Shape::Line;
	}
};
