#pragma once
/* !
@file	Event.h
@author	Tay Sen Chuan
@date	27/10/2023

This file contains the declaration for the Event component class
*//*__________________________________________________________________________*/
#include "Component.h"

class Event : public Component {
public:
	Event() : Component() {};
	virtual void Initialize() override {}


	int linked_event;

	virtual ComponentType TypeId() const override { return ComponentType::Event; }
};