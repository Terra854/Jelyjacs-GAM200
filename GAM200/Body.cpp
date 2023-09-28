#pragma once
/* !
@file
@author
@date	28/9/2023


*//*__________________________________________________________________________*/
#include "../include/components/Body.h"


	void Rectangular::Initialize() 
	{
		Object* o = GetOwner();
		if (o->GetComponent(ComponentType::Transform) != nullptr) {
			Vec2 pos = ((Transform*)o->GetComponent(ComponentType::Transform))->Position;
			aabb.min = pos - Vec2(width / 2, height / 2);
			aabb.max = pos + Vec2(width / 2, height / 2);
		}
	}

	Shape Rectangular::GetShape() 
	{
		return Shape::Rectangle;
	}


	void Circular::Initialize() 
	{
		Object* o = GetOwner();
		if (o->GetComponent(ComponentType::Transform) != nullptr) {
			Vec2 pos = ((Transform*)o->GetComponent(ComponentType::Transform))->Position;
			circle.center = pos;
		}
	}

	Shape Circular::GetShape() 
	{
		return Shape::Circle;
	}

	Shape Lines::GetShape() 
	{
		return Shape::Line;
	}
