/* !
@file Application.cpp
@author Luke Goh
@date	3/11/2023

This file contains the class definitions that is used to run the game
*//*__________________________________________________________________________*/
#include "Application.h"
#include <iostream>
#include <Core_Engine.h>
#include <Factory.h>
#include <glapp.h>
#include <GLWindow.h>
#include <PhysicsSystem.h>
#include <LevelEditor.h>
#include <ThreadPool.h>
#include <SceneManager.h>

#include <debug.h>
#include <GameLogic.h>
#include <Audio.h>
#include <Font.h>
#include <Camera.h>
#include <../src/Assets Manager/asset_manager.h>

CoreEngine* engine; // Needed for Window System to tell the engine when to exit cause messaging system is not ready yet
ThreadPool* thread_pool;
Application::Application()
{
}

Application::~Application()
{
}

void Application::Init() {
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)

	// Enable run-time memory check for debug builds.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	// Copy the imgui configs

	// Check if the destination file already exists
	if (!std::filesystem::exists("imgui.ini")) {
		try {
			// Attempt to copy the file
			std::filesystem::copy_file("Asset/imgui-original.ini", "imgui.ini");
			std::cout << "imgui config copied successfully." << std::endl;
		}
		catch (std::filesystem::filesystem_error& e) {
			std::cerr << e.what() << std::endl;
		}
	}
	else {
		std::cout << "imgui config already exist, skip copying" << std::endl;
	}
#endif

	// Initialise Pointer to Systems
	engine = new CoreEngine();
	GLWindow* windows = new GLWindow();
	Factory* factory = new Factory();
	PhysicsSystem* physics = new PhysicsSystem();
	AssetManager* assetmanager = new AssetManager();
	sceneManager = new SceneManager();
	audio = new Audio(); // declared in Audio.h
	Logic = new GameLogic(); // declared in GameLogic.h
	FontSystem = new Font();
	Camera* camera = new Camera();
	GLApp* graphics = new GLApp();
	thread_pool = new ThreadPool();


	// Add System to the engine 
	engine->AddSystem(windows);
	engine->AddSystem(thread_pool);
	engine->AddSystem(audio);
	engine->AddSystem(assetmanager);
	engine->AddSystem(sceneManager);
	engine->AddSystem(Logic);
	engine->AddSystem(factory);
	engine->AddSystem(physics);
	engine->AddSystem(FontSystem);
	engine->AddSystem(camera);


#if defined(DEBUG) | defined(_DEBUG)
	// Level Editor
	level_editor = new LevelEditor();
	engine->AddSystem(level_editor);
#endif


	engine->AddSystem(graphics);												  // Graphics should always be last

	// Initialize and Start Game Loop
	engine->Initialize();
	//windows->ActivateWindow();
	windows->print_specs();
}
void Application::Run() {

	engine->GameLoop();

	// Free Engine
	delete engine;

}