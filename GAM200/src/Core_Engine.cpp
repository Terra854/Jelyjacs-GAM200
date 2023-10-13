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

/*
#include "ImGui/imgui.h";
#include "ImGui/imgui_impl_glfw.h";
#include "ImGui/imgui_impl_opengl3.h";
*/
CoreEngine* CORE = NULL;
ImVec4 clear_color;

/******************************************************************************
* Default Constructor
* - Initialise Class Variables and Extern Class Pointer
*******************************************************************************/
CoreEngine::CoreEngine()
{
	fixed_dt = 60;
	core_fps = fixed_dt;
	dt = 1.f / static_cast<float>(core_fps);
	game_active = true;
	CORE = this;
}
/******************************************************************************
* Destructor
* - Deallocate all the systems
*******************************************************************************/
CoreEngine::~CoreEngine()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	DeleteSystem();
}

/******************************************************************************
* Initialize
* - Initializing all the Systems
*******************************************************************************/
void CoreEngine::Initialize()
{
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
std::map<std::string, double> elapsed_time;
double total_time = 0.0;

void Update(ISystems* sys)
{
	start_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	sys->Update();
	end_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	elapsed_time[sys->SystemName()] = (double)(end_system_time - start_system_time) / 1000000.0;
	total_time += (double)(end_system_time - start_system_time) / 1000000.0;
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

	auto invFpsLimit = std::chrono::round<std::chrono::system_clock::duration>(std::chrono::duration<double>{ 1. / fixed_dt }); // 1/60
	auto m_BeginFrame = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> m_EndFrame;
	auto prev_time_in_seconds = std::chrono::time_point_cast<std::chrono::seconds>(m_BeginFrame);

	// Console Debug Instructions
	//std::cout << "########################################################" << std::endl;
	//std::cout << "Press F to print out frametime performance information" << std::endl;
	//std::cout << "for the current frame" << std::endl;
	//std::cout << "game window is the active window first and then press P" << std::endl;
	//std::cout << "########################################################" << std::endl;

	ImGuiIO& io = StartGui();

	bool show_performance_viewer = true;

	// Game Loop
	while (game_active)
	{
		// (Your code calls glfwPollEvents())
		// ...
		// Start the Dear ImGui frame

		NewGuiFrame();

		// Toggle Button to Display Debug Information in IMGui
		if (input::IsPressed(KEY::f)) { show_performance_viewer = !show_performance_viewer; }

		//show_performance_viewer ? Debug_Update() : Update();

		float time = GetDt();

		for (const std::pair<std::string, ISystems*>& sys : Systems)
		{
			if (sys.first != "Window" && sys.first != "Graphics")
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
		Update(Systems["Graphics"]);
		//Systems["Graphics"]->Update();
		//end_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		//elapsed_time[Systems["Graphics"]->SystemName()] = (double)(end_system_time - start_system_time) / 1000000.0;
		//total_time += (double)(end_system_time - start_system_time) / 1000000.0;

		//start_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		Update(Systems["Window"]);
		//Systems["Window"]->Update();
		//end_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		//elapsed_time[Systems["Window"]->SystemName()] = (double)(end_system_time - start_system_time) / 1000000.0;
		//total_time += (double)(end_system_time - start_system_time) / 1000000.0;

		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
		ImGui::Begin("Help");
		ImGui::Text("Sample text");
		ImGui::End();

		ImGui::SetNextWindowSize(ImVec2(500, 220));

		ImGui::SetNextWindowPos(ImVec2(0, 50), ImGuiCond_Once);
		ImGui::Begin("Performance Viewer");

		for (std::pair<std::string, double> p : elapsed_time)
			ImGui::Text("%s system completed it's update in %.6f seconds (%.2f%%)", p.first.c_str(), p.second, (p.second / total_time * 100.0));

		ImGui::Text("############################################################");
		ImGui::Text("Total time taken for this frame: %.6f seconds.", total_time);
		ImGui::Text("Frame Rate is: %.6f FPS", dt * 3600.f);

		ImGui::End();

		elapsed_time.clear();
		total_time = 0.0;


		// Rendering
// (Your code clears your framebuffer, renders your other stuff etc.)

		GuiRender(io);

		// FPS Calculation
		//auto time_in_seconds = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());
		m_EndFrame = std::chrono::system_clock::now();
		dt = std::chrono::duration<float>(m_EndFrame - m_BeginFrame).count();			// Delta Time
		core_fps = dt * 3600.f;															// FPS
		window->fps = core_fps;
		accumulator += dt;																// Accumulator
		while (accumulator >= fixed_dt)													// Fixed Time Step - for physics
		{
			accumulator -= fixed_dt;
			numofsteps++;
		}
		/*
		// Update delta_time every second
		if (time_in_seconds > prev_time_in_seconds)
		{
			std::cout << "FPS: " << dt << std::endl;
			prev_time_in_seconds = time_in_seconds;
		}
		*/

		// Updating Frame Times
		m_BeginFrame = m_EndFrame;
		m_EndFrame = m_BeginFrame + invFpsLimit;
	}
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

ImGuiIO& CoreEngine::StartGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window->ptr_window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();
	return io;
}

void CoreEngine::NewGuiFrame(bool showDemo)
{
	glfwPollEvents();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	if (showDemo)
	{
		ImGui::ShowDemoWindow(); // Show demo window! :)
	}
}

void CoreEngine::GuiRender(ImGuiIO& io)
{
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

	glfwSwapBuffers(window->ptr_window);
}