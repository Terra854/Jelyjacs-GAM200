/* !
@file EntryPoint.h
@author Luke Goh
@date	3/11/2023

This file contains the main function that is used to run the game
*//*__________________________________________________________________________*/
#pragma once
#include <windows.h>


#ifdef GAM200_PLATFORM_WINDOWS
	
	extern Application* CreateApplication(); // To be defined in CLIENT

	// Minimizes the game if it loses focus on fullscreen
	// such as when alt tabbed or ctrl+alt+delete
	// (Release build only)
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
		case WM_KILLFOCUS:
			// Minimize the window when focus is lost
			ShowWindow(hwnd, SW_MINIMIZE);
			break;
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		return 0;
	}

#if defined(DEBUG) | defined(_DEBUG)
	int main() { // Entry point

#else
	// WinMain instead of main to not show the Win32 console
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
