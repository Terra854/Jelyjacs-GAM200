#pragma once

#include "Object.h"
#include "LogicScript.h"

class TestScript : public LogicScript {
public:
	TestScript();
	~TestScript();

	void Start();
	void Update();
	void End();
};

extern TestScript* testScript;

