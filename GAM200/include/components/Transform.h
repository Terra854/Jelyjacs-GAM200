#pragma once
/* !
@file	Transform.h
@author	Yeo Jia Ming
@date	28/9/2023

This file contains the declarations and implementations for the transform component class.
Transform contains the information needed for graphics to render the object
*//*__________________________________________________________________________*/
#include <Debug.h>

#include "Vec2.h"
#include "Object.h"
class Transform : public Component
{
public:
	//centre position of bodies
	Transform()	: Component(){};

	Vec2 Position;
	Vec2 PrevPosition;
	Vec2 Scale;
	float Rotation{};

	virtual void Initialize() override
	{

	}

	virtual ComponentType TypeId() const override { return ComponentType::Transform; }
};