#pragma once
#include "LogicScript.h"
#include <iostream>

class Test : public LogicScript{
public:
	Test(std::string name);

	virtual void Start(Object* obj);
	virtual void Update(Object* obj);
	virtual void Shutdown(Object* obj);
}; 



