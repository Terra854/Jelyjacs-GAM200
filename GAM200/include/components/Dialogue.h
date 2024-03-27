#pragma once
#include <Debug.h>

#include "Object.h"

class Dialogue : public Component
{
public:
	Dialogue();
	void Initialize() override;
	virtual ComponentType TypeId() const override { return ComponentType::Dialogue; }
	std::string dialogue_line;
private:
	std::vector<std::string::size_type> sequence;
};