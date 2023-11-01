#pragma once

#ifdef GAM200_PLATFORM_WINDOWS
	#ifdef GAM200_BUILD_DLL
		#define GAM200_API __declspec(dllexport)
	#else
		#define GAM200_API __declspec(dllimport)
	#endif // GAM200_BUILD_DLL
#else
	#error GAM200 only supports Windows!
#endif // GAM200_PLATFORM_WINDOWS
