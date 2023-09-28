#pragma once
/* !
@file    
@author	t.yeeann@digipen.edu
@date	28/9/2023


*//*__________________________________________________________________________*/
#include <Debug.h>
#include "Object.h"

class Physics : public Component
{
public:
	//centre position of bodies
	Physics() : Component() {};

	Vec2 Velocity;
	float Y_Acceleration;
	float Mass;

	virtual void Initialize() override
	{

	}

	virtual ComponentType TypeId() const override { return ComponentType::Physics; }
};
