#pragma once

#include <Object.h>

typedef void (*InitScript)(Object*);
typedef void (*UpdateScript)(Object*);
typedef void (*ShutdownScript)(Object*);

class LogicScript
{
public:
	LogicScript(const InitScript& init, const UpdateScript& update, const ShutdownScript& shutdown);
	~LogicScript();

private:
	InitScript init;
	UpdateScript update;
	ShutdownScript shutdown;
};