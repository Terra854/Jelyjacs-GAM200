#pragma once
/* !
@file
@author	
@date	28/9/2023


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

	virtual void Initialize() override;

	virtual Shape GetShape() override;
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