#pragma once
#include <string>
#include <Object.h>
class LogicScript
{
public:
	LogicScript() {};
	LogicScript(std::string name);
	~LogicScript();
	
	virtual void Start(Object* obj) = 0 ;
	virtual void Update(Object* obj) = 0;
	virtual void Shutdown(Object* obj) = 0;
};