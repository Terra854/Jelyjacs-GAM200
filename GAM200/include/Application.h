#pragma once
#include "Core.h"

class GAM200_API Application {
public:
	Application();
	virtual ~Application();

	void Run();
};
 
// To be defined in CLIENT
Application* CreateApplication();