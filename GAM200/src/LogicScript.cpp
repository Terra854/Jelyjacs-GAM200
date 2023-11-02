#include "LogicScript.h"
#include "GameLogic.h"
#include <iostream>

LogicScript::LogicScript(std::string name) {
	Logic->AddBehaviour(name ,this);
	std::cout << "LogicScript created" << std::endl;
}

LogicScript::~LogicScript() {
	//Logic->RemoveBehaviour(this);
}