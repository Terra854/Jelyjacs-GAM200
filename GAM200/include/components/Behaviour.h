#pragma once

#include <Object.h>

class Behaviour : public Component
{
public:
	Behaviour() : Component(), behaviour_index{0} {}

	//~Behaviour() = default;
	virtual void Initialize() override {}
	void SetIndex(const unsigned int& index);
	unsigned int& GetIndex() noexcept;

	virtual ComponentType TypeId() const override { return ComponentType::Behaviour; }
protected:
	unsigned int behaviour_index;
};