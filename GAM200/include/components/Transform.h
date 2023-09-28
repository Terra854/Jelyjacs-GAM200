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
#include "../../src/Assets Manager/serialization.h"
class Transform : public Component
{
public:
	//centre position of bodies
	Transform()	: Component(){};

	Vec2 Position;
	Vec2 PrevPosition;
	float Scale_x;
	float Scale_y;
	float Rotation;
	Mat3 Matrix;

	virtual void Initialize() override
	{

	}

	virtual ComponentType TypeId() const override { return ComponentType::Transform; }
};