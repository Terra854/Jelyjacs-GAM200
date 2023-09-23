#pragma once
#include <Debug.h>
#include "Object.h"

class Physics : public Component
{
public:
	//centre position of bodies
	Physics() : Component() {};

	float X_Velocity;
	float X_Acceleration;
	float Y_Velocity;
	float Y_Acceleration;
	float Mass;

	virtual void Initialize() override
	{

	}

	virtual ComponentType TypeId() const override { return ComponentType::Physics; }
};
