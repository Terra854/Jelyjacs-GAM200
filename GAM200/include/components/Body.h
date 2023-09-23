#pragma once
#include <Debug.h>

#include "Object.h"
#include "Collision.h"
#include "Transform.h"

enum class shape
{
	rect,
	circle,
	line
};
// Body Component
class Body : public Component
{
public:
	Body(ComponentType ID)
		:Component(ID)
	{};
	virtual shape GetShape() = 0;
};


// Collision Check on Body
class Rectangular : public Body
{
public:
	Rectangular(ComponentType ID)
		:Body(ID)
	{};
	AABB aabb;
	virtual shape GetShape() override
	{
		return shape::rect;
	}
};

class Circlular : public Body
{
public:
	Circlular(ComponentType ID)
		:Body(ID)
	{};
	Circle cirlce;
	virtual shape GetShape() override
	{
		return shape::circle;
	}
};

class lines : public Body
{
public:
	lines(ComponentType ID)
		:Body(ID)
	{};
	Line line;
	virtual shape GetShape() override
	{
		return shape::line;
	}
};
