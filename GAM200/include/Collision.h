#pragma once
/* !
@file
@author	t.yeeann@digipen.edu
@date	28/9/2023


*//*__________________________________________________________________________*/
#include <Debug.h>

#include "Circle.h"
#include "Line.h"
#include "Vec2.h"
#include "AABB.h"
#include "components/Body.h"


const int COLLISION_NONE = 0; //0000
const int COLLISION_LEFT = 1; //0001
const int COLLISION_RIGHT = 2; //0010
const int COLLISION_TOP = 4; //0100
const int COLLISION_BOTTOM = 8; //1000

class Rectangular;

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

	bool Check_AABB_AABB(const AABB& aabb1,
		const Vec2& vel1,
		const AABB& aabb2,
		const Vec2& vel2,
		float dt);

	int Check_Rect_Rect(Rectangular* rect1, const Rectangular* rect2);
}