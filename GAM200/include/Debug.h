#pragma once

// This will tell you where exactly the memory leaks come from on exiting the program
// Limited to debug builds only and if the object is directly created inside this project

#if defined(DEBUG) | defined(_DEBUG)
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define _CRTDBG_MAP_ALLOC
#endif