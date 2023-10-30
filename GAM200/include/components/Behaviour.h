#pragma once

#include <Component.h>

class Behaviour : public Component
{
public:
	Behaviour() : behaviour_index{ 0 } {}
	~Behaviour() = default;
	void SetIndex(const unsigned int& index);
	inline unsigned int& GetIndex() noexcept;

private:
	unsigned int behaviour_index;
};