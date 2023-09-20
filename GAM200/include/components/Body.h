#pragma once
#include <Debug.h>

#include "Composition.h"
#include "Collision.h"
#include "Transform.h"

enum class shape
{
	rect,
	circle,
	line
};

class Body : public GameComponent
{
public:
	virtual shape GetShape() = 0;
};

class Rectangular : public Body
{
public:
	AABB aabb;
	virtual shape GetShape() override
	{
		return shape::rect;
	}
};

class Circlular : public Body
{
public:
	Circle cirlce;
	virtual shape GetShape() override
	{
		return shape::circle;
	}
};

class lines : public Body
{
public:
	Line line;
	virtual shape GetShape() override
	{
		return shape::line;
	}
};
