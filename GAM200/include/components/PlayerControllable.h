#pragma once
/* !
@file	PlayerControllable.h
@author	Tan Yee Ann (t.yeeann@digipen.edu)
@date	28/9/2023

This file contains the declaration for the PlayerControllable component class
*//*__________________________________________________________________________*/
#include "Component.h"

// Nothing for now, just letting the system know that this object is
// controlled by the player
class PlayerControllable : public Component {
public:
	PlayerControllable() : Component() {};
	virtual ComponentType TypeId() const override { return ComponentType::PlayerControllable; }
};