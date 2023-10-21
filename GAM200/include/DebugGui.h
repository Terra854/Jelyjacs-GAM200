#pragma once

#include <Debug.h>

#include "Interface_System.h"


class DebugGui : public ISystems
{
public:
	//Constructor to initialise pointer
	// It's broken, will be purged in the future
	virtual void Initialize() {};
	void Update();
	virtual std::string SystemName() { return "DebugGui"; }
};