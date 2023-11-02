#pragma once

#include <Object.h>

class Behaviour : public Component
{
public:
	Behaviour() : Component(), behaviour_index{0} {}

	//~Behaviour() = default;
	virtual void Initialize() override {}
	void SetIndex(const int& index);
	int& GetIndex() noexcept;

	virtual ComponentType TypeId() const override { return ComponentType::Behaviour; }

	int behaviour_index;
	std::string behaviour_name;
	Object* obj;
};