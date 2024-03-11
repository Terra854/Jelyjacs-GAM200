#pragma once
/* !
@file	Circle.h
@author	Tan Yee Ann (t.yeeann@digipen.edu)
@date	28/9/2023

This file contains the Circle class that represents a circular object for collision
detection purposes
*//*__________________________________________________________________________*/
#include <Debug.h>

#include "Vec2.h"

struct Circle
{
	Circle() : center({ 0.0f, 0.0f }), radius(0.0f) {}
	Circle(Vec2 _center, float _radius) : center(_center), radius(_radius) {}
	Vec2 center;
	float radius;
};