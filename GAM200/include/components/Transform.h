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
	float Scale_x;
	float Scale_y;
	float Rotation;
	Mat3 Matrix;

	virtual void Initialize() override
	{

	}

	/*virtual void JsonSerialize(JsonSerialization obj) override
	{
		obj.readFloat(Position.x, "Properties", "Position", "x");
		obj.readFloat(Position.y, "Properties", "Position", "y");

		obj.readFloat(Scale_x, "Properties", "Scale_x");
		obj.readFloat(Scale_y, "Properties", "Scale_y");
		obj.readFloat(Rotation, "Properties", "Rotation");
	}*/

	virtual ComponentType TypeId() const override { return ComponentType::Transform; }
};