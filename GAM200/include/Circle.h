#pragma once
#include <Debug.h>

#include "Vec2.h"

struct Circle
{
	Circle() : center({ 0.0f, 0.0f }), radius(0.0f) {}
	Circle(Vec2 _center, float _radius) : center(_center), radius(_radius) {}
	Vec2 center;
	float radius;
};