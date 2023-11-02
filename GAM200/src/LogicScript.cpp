#include "LogicScript.h"
#include "GameLogic.h"


LogicScript::LogicScript(std::string name) {
	Logic->AddBehaviour(name ,this);
}

LogicScript::~LogicScript() {
	//Logic->RemoveBehaviour(this);
}