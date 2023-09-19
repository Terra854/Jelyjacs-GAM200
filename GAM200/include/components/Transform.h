#pragma once

#include "Vec2.h"
#include "Composition.h"

class Transform : public GameComponent
{
public:
	//centre position of bodies
	Vec2 Position;
	float Scale;
	Vec2 PrevPosition;
	float X_Velocity;
	float X_Acceleration;
	float Y_Velocity;
	float Y_Acceleration;
	float Mass;
	Mat3 Matrix;
	virtual void Initialize()
	{

	}
};