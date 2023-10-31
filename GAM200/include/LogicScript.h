#pragma once

#include <Object.h>
#include <GameLogic.h>

typedef void (*InitScript)(Object*);
typedef void (*UpdateScript)(Object*);
typedef void (*ShutdownScript)(Object*);

class LogicScript
{
public:
	LogicScript(const InitScript& init, const UpdateScript& update, const ShutdownScript& shutdown)
		: init_script(init), update_script(update), shutdown_script(shutdown) {
		//Logic->AddBehaviour(this);
	}
	//~LogicScript();

	InitScript init_script;
	UpdateScript update_script;
	ShutdownScript shutdown_script;
};