#pragma once
/* !
@file	Debug.h
@author	t.yeeann@digipen.edu
@date	28/9/2023

This file contains preprocessor macros that when enabled, tell you where exactly
the memory leaks come from on exiting the program. This is only available for 
debug builds only.

Note that this will only be able to detect leaks for objects that is directly
created inside this project and even so, may not be able to map all heap allocations
*//*__________________________________________________________________________*/

#if defined(DEBUG) | defined(_DEBUG)
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define _CRTDBG_MAP_ALLOC
#endif