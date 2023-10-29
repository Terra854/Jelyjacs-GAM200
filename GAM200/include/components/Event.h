#pragma once
#include "Component.h"

class Event : public Component {
public:
	Event() : Component() {};
	virtual ComponentType TypeId() const override { return ComponentType::Event; }
};