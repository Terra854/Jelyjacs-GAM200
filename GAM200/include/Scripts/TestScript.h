#pragma once

#include "Object.h"
#include "GameLogic.h"

class TestScript : public LogicScript {
public:
	TestScript(std::string);
	~TestScript() = default;

	void Start(Object *obj);
	void Update(Object * obj);
	void Shutdown(Object *obj);
};


