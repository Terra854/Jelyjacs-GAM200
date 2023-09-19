#pragma once

#include "Circle.h"
#include "Line.h"
#include "Vec2.h"
#include "AABB.h"

const int COLLISION_LEFT = 0x00000001; //0001
const int COLLISION_RIGHT = 0x00000002; //0010
const int COLLISION_TOP = 0x00000004; //0100
const int COLLISION_BOTTOM = 0x00000008; //1000

namespace Collision {
	bool Check_Circle_Line(const Circle& circle,
		const Vec2& ptEnd,
		const Line& lineSeg,
		Vec2& interPt,
		Vec2& normalAtCollision,
		float& interTime);

	bool Check_Circle_LineEdge(bool withinBothLines,
		const Circle& circle,
		const Vec2& ptEnd,
		const Line& lineSeg,
		Vec2& interPt,
		Vec2& normalAtCollision,
		float& interTime);

	bool Check_AABB_Line(const AABB& aabb,
		const Vec2& ptEnd,
		const Line& lineSeg,
		Vec2& interPt,
		Vec2& normalAtCollision,
		float& interTime);

	int Check_Rect_Line(float PosX,
		float PosY,
		float scaleX,
		float scaleY,
		const Line& line);

	bool Check_AABB_AABB(const AABB& aabb1,
		const Vec2& vel1,
		const AABB& aabb2,
		const Vec2& vel2);

	void Response_Object_Line(const Vec2& ptInter,
		const Vec2& normal,
		Vec2& ptEnd,
		Vec2& reflected);
}