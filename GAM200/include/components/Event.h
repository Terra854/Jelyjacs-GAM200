#pragma once
#include "Component.h"

class Event : public Component {
public:
	Event() : Component() {};
	virtual void Initialize() override {}


	int linked_event;

	virtual ComponentType TypeId() const override { return ComponentType::Event; }
};