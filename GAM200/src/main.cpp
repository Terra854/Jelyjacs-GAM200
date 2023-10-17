/* !
@file main.cpp
@author Luke Goh
@date	28/9/2023

This file contains the main function.
*//*__________________________________________________________________________*/
#include <Core_Engine.h>
#include <Factory.h>
#include <glapp.h>
#include <GLWindow.h>
#include <PhysicsSystem.h>
#include <iostream>
#include <debug.h>
#include <GameLogic.h>
#include <Audio.h>
#include <Camera.h>
#include <../src/Assets Manager/asset_manager.h>

CoreEngine* engine; // Needed for Window System to tell the engine when to exit cause messaging system is not ready yet 

int main() {
	
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	// Initialise Pointer to Systems
	engine = new CoreEngine();
	GLWindow* windows = new GLWindow();
	Factory* factory = new Factory();
	PhysicsSystem* physics = new PhysicsSystem();
	AssetManager* assetmanager = new AssetManager();
	audio = new Audio(); // declared in Audio.h
	GameLogic* logic = new GameLogic();
	Camera* camera = new Camera();
	GLApp* graphics = new GLApp();     

	
	// Add System to the engine 
	engine->AddSystem(windows);         
	engine->AddSystem(assetmanager);
	engine->AddSystem(logic);
	engine->AddSystem(factory);
	engine->AddSystem(physics);
	engine->AddSystem(audio);
	engine->AddSystem(camera);
	engine->AddSystem(graphics);												  // Graphics should always be last
	
	// Initialize and Start Game Loop
	engine->Initialize();
	windows->ActivateWindow();                                                   
	windows->print_specs();
	engine->GameLoop();

	// Free Engine
	delete engine;
	return 0;
}

