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
#include <DebugGui.h>
#include <sstream>
#include "Font.h"
#include <PhysicsSystem.h>
#include <glapp.h>

CoreEngine* CORE = NULL;
EngineHud hud;
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
*  - Update Windows and Graphics Last
*******************************************************************************/
/*
void CoreEngine::Update()
{
	for (const std::pair<std::string, ISystems*>& sys : Systems)
	{
		if (sys.first != "Window" && sys.first != "Graphics")
		{
			sys.second->Update();
		}
	}
	Systems["Window"]->Update();
	Systems["Graphics"]->Update();
}
*/

/******************************************************************************
* Update
* - Update all the Systems
* - Checks how long each system updates itself and saves it for the performance viewer
* - Use microseconds for more accuracy
*******************************************************************************/

// Variables that will be needed by Update and GameLoop
long long start_system_time, end_system_time;
//std::map<std::string, double> elapsed_time;
//double total_time = 0.0;

void Update(ISystems* sys)
{
	start_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	sys->Update();
	end_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	//elapsed_time[sys->SystemName()] = (double)(end_system_time - start_system_time) / 1000000.0;
	//total_time += (double)(end_system_time - start_system_time) / 1000000.0;

	debug_gui->SetSystemElapsedTime(sys->SystemName(), (double)(end_system_time - start_system_time) / 1000000.0);
	debug_gui->AddTotalTime((double)(end_system_time - start_system_time) / 1000000.0);
}


/********************************************************************************
* Debug Update Function
* - Same as Update with additional debugging features
* - Loop through all the systems and check the duration of each system update
* - Use microseconds for more accuracy
********************************************************************************/
/*
void CoreEngine::Debug_Update()
{
	float time = GetDt();
	long long start_system_time, end_system_time;
	std::map<std::string, double> elapsed_time;
	double total_time = 0.0;
	std::cout << "########################################################################################" << std::endl;

	for (const std::pair<std::string, ISystems*>& sys : Systems)
	{
		if (sys.first != "Window" && sys.first != "Graphics")
		{
			start_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			sys.second->Update();
			std::cout << sys.second->SystemName() << " is updating" << std::endl;
			end_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			elapsed_time[sys.second->SystemName()] = (double)(end_system_time - start_system_time) / 1000000.0;
			total_time += (double)(end_system_time - start_system_time) / 1000000.0;
		}
	}

	Systems["Window"]->Update();
	Systems["Graphics"]->Update();

	std::cout << "########################################################################################" << std::endl;

	// Output to console for now, will plan to display ingame when the engine can render fonts
	for (std::pair<std::string, double> p : elapsed_time)
		std::cout << p.first << " system completed it's update in " << std::fixed << std::setprecision(6) << p.second << " seconds (" << p.second / total_time * 100.0 << "%)" << std::endl;

	std::cout << "Total time taken for this frame: " << std::fixed << std::setprecision(6) << total_time << " seconds." << std::endl;
	std::cout << "Frame Rate is " << 1.0f / dt << " FPS" << std::endl;
	std::cout << "########################################################################################" << std::endl;

}
*/

/********************************************************************************
* Game Loop
* - Initialising Variables for FPS Calculation
* - When Game is Active
* - Calculate FPS and Update Systems
********************************************************************************/
void CoreEngine::GameLoop()
{
	// FPS Variables
	int numofsteps = 0;
	double accumulator = 0.0;

	//auto invFpsLimit = std::chrono::round<std::chrono::system_clock::duration>(std::chrono::duration<double>{ 1. / fixed_dt }); // 1/60
	//std::chrono::time_point<std::chrono::system_clock> m_EndFrame;
	

	// Console Debug Instructions
	//std::cout << "########################################################" << std::endl;
	//std::cout << "Press F to print out frametime performance information" << std::endl;
	//std::cout << "for the current frame" << std::endl;
	//std::cout << "game window is the active window first and then press P" << std::endl;
	//std::cout << "########################################################" << std::endl;

	ImGuiIO& io = hud.StartGui();

	bool debug_gui_active = true;
	bool show_performance_viewer = true;
	bool objectProperties = false;
	bool tempstorage = 1;
	float pos_x = 0;
	float pos_y = 0;

	float xPos = 0;
	float yPos = 0;

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

	/* End Level Editor */

	// Game Loop
	while (game_active)
	{
		
		auto m_BeginFrame = std::chrono::system_clock::now();
		hud.NewGuiFrame(0);

		// Toggle Button to Display Debug Information in IMGui
		if (input::IsPressed(KEY::f)) { show_performance_viewer = !show_performance_viewer; }

		//show_performance_viewer ? Debug_Update() : Update();

		for (const std::pair<std::string, ISystems*>& sys : Systems)
		{
			if (sys.first != "Window" && sys.first != "Graphics" && sys.first != "DebugGui")
			{
				//start_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
				Update(sys.second);
				//sys.second->Update();
				//end_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
				//elapsed_time[sys.second->SystemName()] = (double)(end_system_time - start_system_time) / 1000000.0;
				//total_time += (double)(end_system_time - start_system_time) / 1000000.0;
			}
		}
		/***************************************************************************************************************************************
		
		
														Drawing
		
		
		*****************************************************************************************************************************************/
		//start_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		
		

		if (input::IsPressed(KEY::p)) { debug_gui_active = !debug_gui_active; }
		if (debug_gui_active) {

			

			// Generate mipmaps after rendering to the high-resolution texture
			glBindTexture(GL_TEXTURE_2D, level_editor_texture);
			glGenerateMipmap(GL_TEXTURE_2D);

			// End rendering into imgui window 

			Update(Systems["DebugGui"]);

			// Display the game inside the ImGui window
			ImGui::SetNextWindowSize(ImVec2(640, 420), ImGuiCond_Always);
			ImGui::Begin("Game Runtime (This for the level editor)");
			ImVec2 windowSize = ImGui::GetWindowSize();
			ImVec2 displaySize = ImVec2(windowSize.x, windowSize.y - 40.f);
			ImGui::Image((void*)(intptr_t)level_editor_texture, displaySize, ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();

			ImGui::Begin("Level editor");
			if (ImGui::CollapsingHeader("Create object")) {
				ImGui::InputFloat("Input x position of object", &xPos);
				ImGui::InputFloat("Input y position of object", &yPos);
				//ImGui::ImageButton("../Asset/Textures/mapbox.png", ImVec2(50, 50));
				if (ImGui::Button("Create box"))
				{
					createObject(xPos, yPos, "../Asset/Objects/mapbox.json");
				}
			}
			//ImGui::End();

			//ImGui::SetNextWindowPos(ImVec2(200, 200), ImGuiCond_Once);
			if (ImGui::CollapsingHeader("Current Object List")) {
				ImGui::Text("Number of game objects in level: %d", objectFactory->NumberOfObjects());
				char objectPropertiesName[32];
				static int selected = -1;
				if (ImGui::BeginTable("split", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
				{
					for (size_t i = 0; i < objectFactory->NumberOfObjects(); i++)
					{
						ImGui::TableNextColumn();
						Object* object = objectFactory->getObjectWithID(static_cast<int>(i));
						char buf[32];
						if (object->GetName().empty())
							sprintf_s(buf, "%d) Object", static_cast<int>(i));
						else
							sprintf_s(buf, "%d) %s", static_cast<int>(i), object->GetName().c_str());

						// Creating button for each object
						if (ImGui::Selectable(buf, selected == static_cast<int>(i))) {
							selected = static_cast<int>(i);
							strcpy_s(objectPropertiesName, buf);
							if (objectProperties == true) { tempstorage = 1; }
							objectProperties = true;

						}
					}
					ImGui::EndTable();
				}
				if (objectProperties) {
					Object* object = objectFactory->getObjectWithID(selected);
					ComponentType componentsarr[20];
					int size;
					const char* componentNames[] = { "Transform", "Texture", "Body", "Physics", "PlayerControllable" };
					ImGui::SetNextWindowPos(ImVec2(300, 40), ImGuiCond_Once);
					ImGui::Begin(objectPropertiesName, &objectProperties);

					ImGui::Text("Object ID: %d", object->GetId());
					ImGui::Text("Object Name: %s", object->GetName().c_str());
					ImGui::Text("Number of components: %d", object->GetNumComponents());
					object->getKeysToArray(componentsarr, size);
					for (int i = 0; i < size; i++)
					{
						ImGui::Text("Component ID: %s", componentNames[static_cast<int>(object->GetComponent(componentsarr[i])->TypeId()) - 1]);
					}
					Transform* tran_pt = static_cast<Transform*>(object->GetComponent(ComponentType::Transform));
					// Not working
					/*
					Vec2 botleft = { tran_pt->Position.x - tran_pt->Scale_x / 2, tran_pt->Position.y - tran_pt->Scale_y / 2 };
					Vec2 topright = { tran_pt->Position.x + tran_pt->Scale_x / 2, tran_pt->Position.y + tran_pt->Scale_y / 2 };
					app->drawline({ botleft.x,botleft.y }, { botleft.x,topright.y });
					app->drawline({ botleft.x,topright.y }, { topright.x,topright.y });
					app->drawline({ topright.x,topright.y }, { topright.x,botleft.y });
					app->drawline({ topright.x,botleft.y }, { botleft.x,botleft.y });
					*/
					ImGui::Text("Object Position:");
					ImGui::Text("x = %.2f, y = %.2f", tran_pt->Position.x, tran_pt->Position.y);
					if (tempstorage) {
						pos_x = tran_pt->Position.x;
						pos_y = tran_pt->Position.y;
						tempstorage = 0;
					}
					ImGui::Text("Change Object X-Axis");
					ImGui::SliderFloat("##", &tran_pt->Position.x, -960.f, 960.f);

					ImGui::Text("Change Object Y-Axis");
					ImGui::SliderFloat("##", &tran_pt->Position.y, -540.f, 540.f);
					if (ImGui::Button("Revert")) {
						tran_pt->Position = { pos_x, pos_y };
					}
					/*
					if (ImGui::Button("Delete Object")) {
						objectFactory->destroyObject(objectFactory->getObjectWithID(object->GetId()));
						objectProperties = false;
					}
					*/
					if (object->GetComponent(ComponentType::Body) != nullptr)
						RecalculateBody(tran_pt, static_cast<Body*>(object->GetComponent(ComponentType::Body)));
					ImGui::End();

				}
				else {
					tempstorage = 1;
					pos_x = 0;
					pos_y = 0;
				}
			}
			ImGui::End();

			

			// For rendering into imgui window 
			glBindFramebuffer(GL_FRAMEBUFFER, level_editor_fb);

			Update(Systems["Graphics"]);
			DrawText("Testing Font", 500, 200, 1);
			Update(Systems["Window"]);

			glBindFramebuffer(GL_FRAMEBUFFER, 0); // Back to rendering to the main window
			hud.GuiRender(io);
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, 0); // Render direct to window

			Update(Systems["Graphics"]);
			DrawText("Testing Font", 500, 200, 1);
			Update(Systems["Window"]);
			hud.GuiRender(io);
		}

		debug_gui->ClearAll();
		

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
			//std::cout << "FPS: " << core_fps << std::endl;
			//std::cout << "DT: " << dt << std::endl;
			prev_time_in_seconds = time_in_seconds;
		}
	

		// Updating Frame Times
		//m_BeginFrame = m_EndFrame;
		//m_EndFrame = m_BeginFrame + invFpsLimit;
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

void CoreEngine::createObject(float posX, float posY, std::string objectName)
{
	Object* testingObject = objectFactory->createObject(objectName);
	long testingObjectID = testingObject->GetId();
	Transform* tran_pt = static_cast<Transform*>((objectFactory->getObjectWithID(testingObjectID))->GetComponent(ComponentType::Transform));
	tran_pt->Position.x = posX;
	tran_pt->Position.y = posY;
}