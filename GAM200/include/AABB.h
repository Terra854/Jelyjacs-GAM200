#pragma once

#include "Vec2.h"

struct AABB
{
	Vec2 min; // Bottom Left
	Vec2 max; // Top Right

	/*
		P3------P2
		|        |
		|        |
		P0------P1
	*/

	Vec2 P0() const { return min; } // Bottom Left
	Vec2 P1() const { return { max.x, min.y }; } // Bottom Right
	Vec2 P2() const { return max; } // Top Right
	Vec2 P3() const { return { min.x, max.y }; } // Top Left
};