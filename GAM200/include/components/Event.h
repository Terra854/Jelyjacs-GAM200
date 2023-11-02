#pragma once
/* !
@file	Event.h
@author	?
@date	02/11/2023

This file contains declarations for the body component class
Body, as a shape contains the hitbox collision outline of the object.
*//*__________________________________________________________________________*/
#include "Component.h"

class Event : public Component {
public:
	Event() : Component() {};
	virtual void Initialize() override {}


	int linked_event;

	virtual ComponentType TypeId() const override { return ComponentType::Event; }
};