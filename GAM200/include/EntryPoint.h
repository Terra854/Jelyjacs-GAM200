/* !
@file EntryPoint.h
@author Luke Goh
@date	3/11/2023

This file contains the main function that is used to run the game
*//*__________________________________________________________________________*/
#pragma once


#ifdef GAM200_PLATFORM_WINDOWS
	
	extern Application* CreateApplication(); // To be defined in CLIENT
#if defined(DEBUG) | defined(_DEBUG)
	int main() { // Entry point

#else
#include <windows.h>
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {

		UNREFERENCED_PARAMETER(hInstance);
		UNREFERENCED_PARAMETER(hPrevInstance);
		UNREFERENCED_PARAMETER(lpCmdLine);
		UNREFERENCED_PARAMETER(nCmdShow);
#endif
		auto app = CreateApplication();
		app->Init();
		app->Run();
		delete app;
	}
#endif // GAM200_PLATFORM_WINDOWS
