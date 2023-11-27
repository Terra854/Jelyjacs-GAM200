#pragma once
/* !
@file	Collision.h
@author	Tan Yee Ann
@date	28/9/2023

This file contains the declarations of functions that are defined in Collision.cpp
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

	extern std::vector<std::vector<std::vector<Object*>>> uniform_grid;

	bool Check_AABB_AABB(const AABB& aabb1,
		const Vec2& vel1,
		const AABB& aabb2,
		const Vec2& vel2,
		float dt);

	void Check_Rect_Rect(Rectangular* rect1, Rectangular* rect2);
}