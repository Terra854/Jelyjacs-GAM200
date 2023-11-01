#include "LogicScript.h"
#include "GameLogic.h"


LogicScript::LogicScript(const InitScript& init, const UpdateScript& update, const ShutdownScript& shutdown)
	: init_script(init), update_script(update), shutdown_script(shutdown) {
	Logic->AddBehaviour(this);
}

LogicScript::~LogicScript() {
	//Logic->RemoveBehaviour(this);
}