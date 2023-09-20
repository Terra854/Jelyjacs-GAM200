#pragma once

#include "Vec2.h"
#include "Composition.h"
#include "../../src/Assets Manager/serialization.h"
class Transform : public GameComponent
{
public:
	//centre position of bodies
	Vec2 Position;
	float Scale;
	float Rotation;
	Vec2 PrevPosition;
	float X_Velocity;
	float X_Acceleration;
	float Y_Velocity;
	float Y_Acceleration;
	float Mass;
	Mat3 Matrix;
	virtual void Initialize() override
	{

	}
	virtual void Serialize(Serialization& stream) override
	{
		streamGet( stream , Position.x);
		streamGet(stream, Position.y);
		streamGet(stream, Scale);
		streamGet(stream, Rotation);
	}
};