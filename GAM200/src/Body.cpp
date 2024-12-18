/* !
@file	Body.cpp
@author	Yeo Jia Ming
@date	28/9/2023

This file contains implementations for Body component class.
Body, as a shape contains the hitbox collision outline of the object.
*//*__________________________________________________________________________*/
#include "../include/components/Body.h"

/******************************************************************************
	Initializes the Rectangle collision data
*******************************************************************************/
void Rectangular::Initialize()
{
	Object* o = GetOwner();
	if (o->GetComponent(ComponentType::Transform) != nullptr) {
		Vec2 pos = ((Transform*)o->GetComponent(ComponentType::Transform))->Position;
		aabb.min = pos - Vec2(width / 2, height / 2);
		aabb.max = pos + Vec2(width / 2, height / 2);
	}
}


/******************************************************************************
	Initializes the Circle collision data
*******************************************************************************/
void Circular::Initialize()
{
	Object* o = GetOwner();
	if (o->GetComponent(ComponentType::Transform) != nullptr) {
		Vec2 pos = ((Transform*)o->GetComponent(ComponentType::Transform))->Position;
		circle.center = pos;
	}
}

/******************************************************************************
	Returns the shape type
*******************************************************************************/

Shape Rectangular::GetShape()
{
	return Shape::Rectangle;
}

Shape Circular::GetShape()
{
	return Shape::Circle;
}

Shape Lines::GetShape()
{
	return Shape::Line;
}
