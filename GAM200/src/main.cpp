#include <Core_Engine.h>
#include <Factory.h>
#include <glapp.h>
#include <GLWindow.h>
#include <PhysicsSystem.h>
#include <iostream>
//#include <chrono>
//#include <thread>
#include <debug.h>
#include <GameLogic.h>
#include <Audio.h>

CoreEngine* engine; // Needed for Window System to tell the engine when to exit cause messaging system is not ready yet 

int main() {
	
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	engine = new CoreEngine();
	GameLogic* logic = new GameLogic();
	GLWindow* windows = new GLWindow();
	Factory* factory = new Factory();
	PhysicsSystem* physics = new PhysicsSystem();
	Audio* audio = new Audio();
	GLApp* graphics = new GLApp();     
	

	engine->AddSystem(windows);                  
	engine->AddSystem(logic);
	engine->AddSystem(factory);
	engine->AddSystem(physics);
	engine->AddSystem(audio);
	engine->AddSystem(graphics);												  // Graphics should always be last
	

	engine->Initialize();
	windows->ActivateWindow();                                                   
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
	delete engine;
	return 0;
}
