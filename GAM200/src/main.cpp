﻿#include <Core_Engine.h>
#include <Factory.h>
#include <glapp.h>
#include <GLWindow.h>
#include <physics.h>
#include <iostream>
//#include <chrono>
//#include <thread>

#include <debug.h>

GameObjectFactory* factory; // Need to be outside for physics component to access it

int main() {
	
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	CoreEngine* engine = new CoreEngine();
	GLWindow* windows = new GLWindow();
	factory = new GameObjectFactory();
	Physics* physics = new Physics();

	GLApp* graphics = new GLApp();                                          // @GuoChen your graphics systems nid to match this code
																			// @GuoChen Then declare ur window properties
	engine->AddSystem(windows->SystemName(), windows);                   // @GuoChen
	engine->AddSystem(graphics->SystemName(), graphics);                 // @GuoChen

	engine->AddSystem(factory->SystemName(), factory);
	engine->AddSystem(physics->SystemName(), physics);

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
	delete engine;
	return 0;
}
