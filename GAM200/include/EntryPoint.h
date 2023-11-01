#pragma once

#ifdef GAM200_PLATFORM_WINDOWS

	extern Application* CreateApplication();
	int main() {
		auto app = CreateApplication();
		app->Run();
		delete app;
	}
#endif // GAM200_PLATFORM_WINDOWS
