#pragma once

#include "Vec2.h"
#include "Composition.h"

class Transform : public GameComponent
{
public:
	//centre position of bodies
	Vec2 Position;
	Vec2 PrevPosition;
	float X_Velocity;
	float X_Acceleration;
	float Y_Velocity;
	float Y_Acceleration;
	float Mass;

	virtual void Initialize()
	{

	}
};