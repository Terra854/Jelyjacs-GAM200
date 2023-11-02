/* !
@file Core_Engine.cpp
@author Luke Goh
@date	28/9/2023

This file contains the definitions of the functions that are part of the Core Engine
*//*__________________________________________________________________________*/
#include <Precompile.h>
#include<GLWindow.h>
#include <Debug.h>
#include "Core_Engine.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <input.h>
#include <map>
#include <thread>
#include "EngineHud.h"
#include <Object.h>
#include <Factory.h>
#include <Collision.h>
#include <LevelEditor.h>
#include <sstream>
#include "Font.h"
#include <PhysicsSystem.h>
#include <glapp.h>
#include "GameHud.h"

CoreEngine* CORE = NULL;
EngineHud hud;
GameHud gamehud;
ImVec4 clear_color;

/******************************************************************************
* Default Constructor
* - Initialise Class Variables and Extern Class Pointer
*******************************************************************************/
CoreEngine::CoreEngine()
{
	fixed_dt = 60;
	core_fps = fixed_dt;
	dt = 1.f / core_fps;
	game_active = true;
	CORE = this;
	paused = true;
}
/******************************************************************************
* Destructor
* - Deallocate all the systems
*******************************************************************************/
CoreEngine::~CoreEngine()
{
	DeleteSystem();
}

/******************************************************************************
* Initialize
* - Initializing all the Systems
*******************************************************************************/
void CoreEngine::Initialize()
{
	// Initialize all the Systems
	std::cout << "Initialising " << Systems["Window"]->SystemName() << std::endl;
	Systems["Window"]->Initialize(); // Must initialize Window first
	for (const std::pair<std::string, ISystems*>& sys : Systems)
	{ // Then initialize all other systems
		if (sys.first != "Window")
		{ // Window already initialized, do not do it again
			// printing system name for debugging purposes
			std::cout << "Initialising " << sys.second->SystemName() << std::endl;
			sys.second->Initialize();
		}
	}
	std::cout << "Number of game objects(pre): " << objectFactory->NumberOfObjects() << "\n";

	std::cout << "Number of game objects(post): " << objectFactory->NumberOfObjects() << "\n";

	clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
}

/******************************************************************************
* Update
* - Update all the Systems
* - Checks how long each system updates itself and saves it for the performance viewer
* - Use microseconds for more accuracy
*******************************************************************************/

// Variables that will be needed by Update and GameLoop
long long start_system_time, end_system_time;

void Update(ISystems* sys)
{
	start_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	sys->Update();
	end_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	//elapsed_time[sys->SystemName()] = (double)(end_system_time - start_system_time) / 1000000.0;
	//total_time += (double)(end_system_time - start_system_time) / 1000000.0;
	level_editor->SetSystemElapsedTime(sys->SystemName(), (double)(end_system_time - start_system_time) / 1000000.0);
	level_editor->AddTotalTime((double)(end_system_time - start_system_time) / 1000000.0);
}

/********************************************************************************
* Game Loop
* - Initialising Variables for FPS Calculation
* - When Game is Active
* - Calculate FPS and Update Systems
********************************************************************************/
void CoreEngine::GameLoop()
{
	int numOfBoxes = (int) (editor_grid->num.x * editor_grid->num.y);
	std::cout << "Number of boxes " << numOfBoxes << std::endl;
	std::vector<int> boxesFilled(numOfBoxes, 0);
	std::cout << boxesFilled.capacity();
	// FPS Variables
	int numofsteps = 0;
	double accumulator = 0.0;

	ImGuiIO& io = hud.StartGui();

	bool debug_gui_active = true;
	bool show_performance_viewer = true;

	/* Level Editor */

	GLuint level_editor_fb, level_editor_texture;

	// Create the framebuffer
	glGenFramebuffers(1, &level_editor_fb);
	glBindFramebuffer(GL_FRAMEBUFFER, level_editor_fb);

	// Create the texture to which we'll render
	glGenTextures(1, &level_editor_texture);
	glBindTexture(GL_TEXTURE_2D, level_editor_texture);

	// Set up the texture with high resolution
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window->width, window->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	// Set texture parameters, including mipmap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Attach the texture to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, level_editor_texture, 0);

	// Generate mipmaps after rendering to the high-resolution texture
	glBindTexture(GL_TEXTURE_2D, level_editor_texture);
	glGenerateMipmap(GL_TEXTURE_2D);

	/* End Level Editor */
	GLuint tileset = app->setup_texobj("Asset/Picture/TileSheet.png");

	// Game Loop

	gamehud.Initialize();

	while (game_active)
	{
		
		auto m_BeginFrame = std::chrono::system_clock::now();
		hud.NewGuiFrame(0);
		input::Update();

		// Toggle Button to Display Debug Information in IMGui
		if (input::IsPressed(KEY::f)) { show_performance_viewer = !show_performance_viewer; }

		//show_performance_viewer ? Debug_Update() : Update();


		for (const std::pair<std::string, ISystems*>& sys : Systems)
		{
			if (sys.first != "Window" && sys.first != "Graphics" && sys.first != "LevelEditor")
			{
				Update(sys.second);
			}
		}
		
		/***************************************************************************************************************************************
		
		
														Drawing
		
		
		*****************************************************************************************************************************************/
		if (input::IsPressed(KEY::p)) { debug_gui_active = !debug_gui_active; }
		if (debug_gui_active) {

			
			
			// For rendering into imgui window 
			glBindFramebuffer(GL_FRAMEBUFFER, level_editor_fb);

			Update(Systems["Graphics"]);
			
			gamehud.Update();
			gamehud.Draw();

			Update(Systems["Window"]);
			//editor_grid->drawleveleditor();

			glBindFramebuffer(GL_FRAMEBUFFER, 0); // Back to rendering to the main window
			// End rendering into imgui window 

			Update(Systems["LevelEditor"]);

			// Display the game inside the ImGui window
			ImGui::Begin("Game Viewport");
			ImVec2 windowSize = ImGui::GetWindowSize();

			ImVec2 displaySize;
			if (windowSize.y < (windowSize.x / 16.f * 9.f)) {
				ImGui::SetCursorPos(ImVec2((windowSize.x - (windowSize.y * 16.f / 9.f)) / 2.f, 20.f));
				displaySize = ImVec2(windowSize.y * 16.f / 9.f, windowSize.y - 30.f);
			}
			else {
				ImGui::SetCursorPos(ImVec2(10.f, (windowSize.y - ((windowSize.x - 20.f) / 16.f * 9.f)) / 2.f));
				displaySize = ImVec2(windowSize.x, (windowSize.x / 16.f * 9.f) - 30.f);
			}

			//ImVec2 displaySize = ImVec2(windowSize.x, windowSize.y - 40.f);
			ImGui::Image((void*)(intptr_t)level_editor_texture, displaySize, ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();

			if (show_tileset) {
				ImGui::Begin("Tileset", &show_tileset);
				ImGui::SetCursorPos({ 0, 0 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.090909f, 0.111111f), ImVec2(0.181818f, 0.222222f));
				ImGui::SetCursorPos({ 64, 0 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.181818f, 0.111111f), ImVec2(0.272727f, 0.222222f));
				ImGui::SetCursorPos({ 128, 0 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.272727f, 0.111111f), ImVec2(0.363636f, 0.222222f));
				ImGui::SetCursorPos({ 0, 64 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.090909f, 0.222222f), ImVec2(0.181818f, 0.333333f));
				ImGui::SetCursorPos({ 128, 64 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.272727f, 0.222222f), ImVec2(0.363636f, 0.333333f));
				ImGui::SetCursorPos({ 0, 128 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.090909f, 0.333333f), ImVec2(0.181818f, 0.444444f));
				ImGui::SetCursorPos({ 64, 128 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.181818f, 0.333333f), ImVec2(0.272727f, 0.444444f));
				ImGui::SetCursorPos({ 128, 128 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.272727f, 0.333333f), ImVec2(0.363636f, 0.444444f));
				ImGui::SetCursorPos({ 0, 192 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.090909f, 0.444444f), ImVec2(0.181818f, 0.555555f));
				ImGui::SetCursorPos({ 64, 192 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.181818f, 0.444444f), ImVec2(0.272727f, 0.555555f));
				ImGui::SetCursorPos({ 128, 192 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.272727f, 0.444444f), ImVec2(0.363636f, 0.555555f));
				ImGui::SetCursorPos({ 0, 256 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.090909f, 0.555555f), ImVec2(0.181818f, 0.666666f));
				ImGui::SetCursorPos({ 64, 256 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.181818f, 0.555555f), ImVec2(0.272727f, 0.666666f));
				ImGui::SetCursorPos({ 128, 256 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.272727f, 0.555555f), ImVec2(0.363636f, 0.666666f));
				ImGui::SetCursorPos({ 0, 320 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.090909f, 0.666666f), ImVec2(0.181818f, 0.777777f));
				ImGui::SetCursorPos({ 64, 320 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.181818f, 0.666666f), ImVec2(0.272727f, 0.777777f));
				ImGui::SetCursorPos({ 128, 320 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.272727f, 0.666666f), ImVec2(0.363636f, 0.777777f));
				ImGui::SetCursorPos({ 0, 384 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.090909f, 0.777777f), ImVec2(0.181818f, 0.888888f));
				ImGui::SetCursorPos({ 64, 384 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.181818f, 0.777777f), ImVec2(0.272727f, 0.888888f));
				ImGui::SetCursorPos({ 128, 384 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.272727f, 0.777777f), ImVec2(0.363636f, 0.888888f));
				ImGui::SetCursorPos({ 192, 192 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.363636f, 0.444444f), ImVec2(0.454545f, 0.555555f));
				ImGui::SetCursorPos({ 256, 192 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.454545f, 0.444444f), ImVec2(0.545454f, 0.555555f));
				ImGui::SetCursorPos({ 192, 256 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.363636f, 0.555555f), ImVec2(0.454545f, 0.666666f));
				ImGui::SetCursorPos({ 256, 256 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.454545f, 0.555555f), ImVec2(0.545454f, 0.666666f));
				ImGui::SetCursorPos({ 320, 128 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.545454f, 0.333333f), ImVec2(0.636363f, 0.444444f));
				ImGui::SetCursorPos({ 320, 192 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.545454f, 0.444444f), ImVec2(0.636363f, 0.555555f));
				ImGui::SetCursorPos({ 320, 256 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.545454f, 0.555555f), ImVec2(0.636363f, 0.666666f));
				ImGui::SetCursorPos({ 192, 320 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.363636f, 0.666666f), ImVec2(0.454545f, 0.777777f));
				ImGui::SetCursorPos({ 256, 320 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.454545f, 0.666666f), ImVec2(0.545454f, 0.777777f));
				ImGui::SetCursorPos({ 320, 320 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.545454f, 0.666666f), ImVec2(0.636363f, 0.777777f));
				ImGui::SetCursorPos({ 448, 0 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.727272f, 0.111111f), ImVec2(0.818181f, 0.222222f));
				ImGui::SetCursorPos({ 512, 0 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.818181f, 0.111111f), ImVec2(0.909090f, 0.222222f));
				ImGui::SetCursorPos({ 576, 0 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.909090f, 0.111111f), ImVec2(1.f, 0.222222f));
				ImGui::SetCursorPos({ 448, 64 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.727272f, 0.222222f), ImVec2(0.818181f, 0.333333f));
				ImGui::SetCursorPos({ 512, 64 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.818181f, 0.222222f), ImVec2(0.909090f, 0.333333f));
				ImGui::SetCursorPos({ 448, 128 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.727272f, 0.333333f), ImVec2(0.818181f, 0.444444f));
				ImGui::SetCursorPos({ 512, 128 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.818181f, 0.333333f), ImVec2(0.909090f, 0.444444f));
				ImGui::SetCursorPos({ 448, 192 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.727272f, 0.444444f), ImVec2(0.818181f, 0.555555f));
				ImGui::SetCursorPos({ 512, 192 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.818181f, 0.444444f), ImVec2(0.909090f, 0.555555f));
				ImGui::SetCursorPos({ 448, 256 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.727272f, 0.555555f), ImVec2(0.818181f, 0.666666f));
				ImGui::SetCursorPos({ 512, 256 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.818181f, 0.555555f), ImVec2(0.909090f, 0.666666f));
				ImGui::SetCursorPos({ 448, 320 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.727272f, 0.666666f), ImVec2(0.818181f, 0.777777f));
				ImGui::SetCursorPos({ 512, 320 });
				ImGui::Image((void*)(intptr_t)tileset, ImVec2(64.0f, 64.0f), ImVec2(0.818181f, 0.666666f), ImVec2(0.909090f, 0.777777f));
				ImGui::End();
			}

			hud.GuiRender(io);
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, 0); // Render direct to window

			Update(Systems["Graphics"]);
			gamehud.Update();
			gamehud.Draw();
			Update(Systems["Window"]);
			hud.GuiRender(io);
		}

		level_editor->ClearAll();
		

		// FPS Calculation
		auto prev_time_in_seconds = std::chrono::time_point_cast<std::chrono::seconds>(m_BeginFrame);
		auto time_in_seconds = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());

		auto m_EndFrame = std::chrono::system_clock::now();
		dt = std::chrono::duration<float>(m_EndFrame - m_BeginFrame).count();			// Delta Time
		core_fps = 1.f / dt;															// FPS
		window->fps = core_fps;
		accumulator += dt;																// Accumulator
		while (accumulator >= fixed_dt)													// Fixed Time Step - for physics
		{
			accumulator -= fixed_dt;
			numofsteps++;
		}
		
		// Update delta_time every second
		if (time_in_seconds > prev_time_in_seconds)
		{
			prev_time_in_seconds = time_in_seconds;
		}
	}
	glDeleteFramebuffers(1, &level_editor_fb);
	glDeleteTextures(1, &level_editor_texture);
}

/********************************************************************************
* AddSystem
* - Add the system into the system container
********************************************************************************/
void CoreEngine::AddSystem(ISystems* sys)
{
	Systems[sys->SystemName()] = sys;
}
/********************************************************************************
* DeleteSystem
* - Delete all the systems from the system container
********************************************************************************/
void CoreEngine::DeleteSystem()
{
	for (const std::pair<std::string, ISystems*>& sys : Systems)
		delete sys.second;
}

/********************************************************************************
* Broadcast
* - Broadcast Messages to all the systems
********************************************************************************/
void CoreEngine::Broadcast(Message_Handler* msg)
{
	// Set Game_mode to 0 to stop loop
	if (msg->GetMessage() == MessageID::Quit)
	{
		game_active = false;
	}
	// Loop Messaging System
	for (const std::pair<std::string, ISystems*>& sys : Systems)
	{
		sys.second->MessageRelay(msg);
	}
}