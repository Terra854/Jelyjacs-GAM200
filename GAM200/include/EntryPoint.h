/* !
@file EntryPoint.h
@author Luke Goh
@date	3/11/2023

This file contains the main function that is used to run the game
*//*__________________________________________________________________________*/
#pragma once

#ifdef GAM200_PLATFORM_WINDOWS
	
	extern Application* CreateApplication(); // To be defined in CLIENT
	int main() { // Entry point
		auto app = CreateApplication();
		app->Run();
		delete app;
	}
#endif // GAM200_PLATFORM_WINDOWS
