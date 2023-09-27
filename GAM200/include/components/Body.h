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
	Body() : Component() {};
	virtual Shape GetShape() = 0;
	virtual void Initialize() override {};
	virtual ComponentType TypeId() const override { return ComponentType::Body; }
};

// Rectangle Object
class Rectangular : public Body
{
public:
	Rectangular() : Body() {}
	Rectangular(float _w, float _h) : Body(), width(_w), height(_h) {
		Initialize();
	}
	AABB aabb;
	float width, height;
	int collision_flag;

	virtual void Initialize() override {
		Object* o = GetOwner();
		if (o->GetComponent(ComponentType::Transform) != nullptr) {
			Vec2 pos = ((Transform*)o->GetComponent(ComponentType::Transform))->Position;
			aabb.min = pos - Vec2(width / 2, height / 2);
			aabb.max = pos + Vec2(width / 2, height / 2);
		}
	};

	virtual Shape GetShape() override
	{
		return Shape::Rectangle;
	}
};

// Circle Object
class Circular : public Body
{
public:
	Circular() : Body() {}
	Circular(float radius) : Body() {
		circle.radius = radius;
	}
	Circle circle;

	virtual void Initialize() override {
		Object* o = GetOwner();
		if (o->GetComponent(ComponentType::Transform) != nullptr) {
			Vec2 pos = ((Transform*)o->GetComponent(ComponentType::Transform))->Position;
			circle.center = pos;
		}
	};

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
