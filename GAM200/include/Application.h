/* !
@file Application.h
@author Luke Goh
@date	3/11/2023

This file contains the decalaration of the Application class that is used to run the game
*//*__________________________________________________________________________*/
#pragma once
#include "Core.h"


class GAM200_API Application {	// This class is used to run the game
public:
	Application();
	virtual ~Application();

	void Run();
};
 
// To be defined in CLIENT
Application* CreateApplication();