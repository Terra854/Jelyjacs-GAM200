#pragma once
/* !
@file
@author
@date	28/9/2023


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
	virtual void Serialize(Serialization& stream) override
	{
		streamGet( stream , Position.x);
		streamGet(stream, Position.y);
		streamGet(stream, Scale_x);
		streamGet(stream, Scale_y);
		streamGet(stream, Rotation);
	}

	virtual ComponentType TypeId() const override { return ComponentType::Transform; }
};