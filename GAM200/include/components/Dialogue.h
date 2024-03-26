#pragma once
#include <Debug.h>

#include "Object.h"

class Dialogue : public Component
{
public:
	Dialogue();
	void Initialize() override;
	virtual ComponentType TypeId() const override { return ComponentType::Dialogue; }
};