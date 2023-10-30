#pragma once

#include <Component.h>

class Behaviour : public Component
{
public:
	void SetIndex(const unsigned int& index);
	inline unsigned int& GetIndex() noexcept;

private:
	unsigned int behaviour_index;
};