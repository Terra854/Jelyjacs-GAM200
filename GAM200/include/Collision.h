#pragma once
#include "Circle.h"
#include "Line.h"
#include "Vec2.h"

namespace Collision {
	int Check_Circle_Line(const Circle& circle,
		const Vec2& ptEnd,
		const Line& lineSeg,
		Vec2& interPt,
		Vec2& normalAtCollision,
		float& interTime,
		bool& checkLineEdges);

	int Check_Circle_LineEdge(bool withinBothLines,
		const Circle& circle,
		const Vec2& ptEnd,
		const Line& lineSeg,
		Vec2& interPt,
		Vec2& normalAtCollision,
		float& interTime);

	void Response_Circle_Line(const Vec2& ptInter,
		const Vec2& normal,
		Vec2& ptEnd,
		Vec2& reflected);
}