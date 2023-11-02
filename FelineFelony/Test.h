#pragma once
#include "Object.h"
#include "LogicScript.h"

class Test : public LogicScript{
public:
	Test(std::string name) : LogicScript(name) {}

	void Start(Object* obj);
	void Update(Object* obj);
	void End(Object* obj);
} 

