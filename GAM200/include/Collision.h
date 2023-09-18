#pragma once

#include "Circle.h"
#include "Line.h"
#include "Vec2.h"
#include "AABB.h"

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
		float& interTime,
		bool& checkLineEdges);

	bool Check_AABB_AABB(const AABB& aabb1,
		const Vec2& vel1,
		const AABB& aabb2,
		const Vec2& vel2);

	void Response_Object_Line(const Vec2& ptInter,
		const Vec2& normal,
		Vec2& ptEnd,
		Vec2& reflected);
}