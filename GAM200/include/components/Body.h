#pragma once
/* !
@file	Body.h
@author	Yeo Jia Ming
@date	28/9/2023

This file contains declarations for the body component class
Body, as a shape contains the hitbox collision outline of the object.
*//*__________________________________________________________________________*/
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

	bool collision_response = true; // Set to false for moving platforms

	// Record down the grids the object is in. 1st is width, 2nd is height
	std::vector<std::pair<int, int>> inGrid;
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

	int collision_flag = 0;
	
	// Get link to the object that is colliding with this object
	Object* top_collision = nullptr;    // get from the other object's bottom y
	Object* bottom_collision = nullptr; // get from the other object's top y
	Object* left_collision = nullptr;   // get from the other object's right x
	Object* right_collision = nullptr;  // get from the other object's left x

	virtual void Initialize() override;

	virtual Shape GetShape() override;

	void ResetCollisionFlags() {
		collision_flag = 0;
		top_collision = nullptr;
		bottom_collision = nullptr;
		left_collision = nullptr;
		right_collision = nullptr;
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

		virtual void Initialize() override;

		virtual Shape GetShape() override;
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
	virtual Shape GetShape() override;
};