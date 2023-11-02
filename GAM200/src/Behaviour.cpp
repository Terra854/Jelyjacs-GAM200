#include "../include/components/Behaviour.h"


void Behaviour::SetIndex(const int& index)
{
	behaviour_index = index;
}

int& Behaviour::GetIndex() noexcept
{
	return behaviour_index;
}
