#pragma once
#include "Component.h"

class Event : public Component {
public:
	Event() : Component() {};
	int linked_event = 1;
	virtual ComponentType TypeId() const override { return ComponentType::Event; }
};