#pragma once
#include <Debug.h>
#include "Component.h"

class Event : public Component
{
public:
	Event();
	void Initialize() override;

	int linked_event;

	void runevent();

	virtual ComponentType TypeId() const override { return ComponentType::Event; }
};

