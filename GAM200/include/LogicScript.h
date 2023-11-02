#pragma once

#include <Object.h>

class LogicScript
{
public:
	LogicScript() {};
	LogicScript(std::string name);
	~LogicScript();
	
	virtual void Start(Object* obj) {};
	virtual void Update(Object* obj) {};
	virtual void Shutdown(Object* obj) {};
};