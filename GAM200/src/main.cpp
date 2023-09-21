#include <Core_Engine.h>
#include <Factory.h>
#include <glapp.h>
#include <GLWindow.h>
#include <physics.h>
#include <iostream>
//#include <chrono>
//#include <thread>
#include <debug.h>
#include <GameLogic.h>

int main() {
	
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	CoreEngine* engine = new CoreEngine();
	GameLogic* logic = new GameLogic();
	GLWindow* windows = new GLWindow();
	GameObjectFactory* factory = new GameObjectFactory();
	Physics* physics = new Physics();
	GLApp* graphics = new GLApp();                                         

	engine->AddSystem(windows->SystemName(), windows);                  
	engine->AddSystem(logic->SystemName(), logic);
	engine->AddSystem(factory->SystemName(), factory);
	engine->AddSystem(physics->SystemName(), physics);
	engine->AddSystem(graphics->SystemName(), graphics);                 

	engine->Initialize();
	windows->ActivateWindow();                                                    // @GuoChen Window Class nid this function
	windows->print_specs();
	engine->GameLoop();
	//const int targetFPS = 60; // Your desired FPS
	//const std::chrono::milliseconds frameDuration(1000 / targetFPS);
	//auto frameStart = std::chrono::high_resolution_clock::now();

	/*
	auto frameEnd = std::chrono::high_resolution_clock::now();
	auto frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);

	if (frameTime < frameDuration) {
		 std::this_thread::sleep_for(frameDuration - frameTime);
	}
	*/
	engine->DeleteSystem();
	return 0;
}
