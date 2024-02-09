#pragma once
/* !
@file	Physics.h
@author	Tan Yee Ann
@date	28/9/2023

This file contains the declaration for the Physics component class
*//*__________________________________________________________________________*/
#include <Debug.h>
#include "Object.h"

class Physics : public Component
{
public:
	Physics() : Component() {};

	Vec2 Velocity;
	bool AffectedByGravity = true;
	float Mass = 1.f;
	float Force = 0.f;
	bool AbleToPushObjects = false;

	virtual void Initialize() override {}

	// Returns name of the component
	virtual ComponentType TypeId() const override { return ComponentType::Physics; }
};
