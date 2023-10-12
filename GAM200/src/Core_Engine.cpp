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
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <Imgui/imgui_impl_opengl3.h>
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
	core_fps = 60;
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

/********************************************************************************
* Debug Update Function
* - Same as Update with additional debugging features
* - Loop through all the systems and check the duration of each system update
* - Use microseconds for more accuracy
********************************************************************************/
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

/********************************************************************************
* Game Loop
* - Initialising Variables for FPS Calculation
* - When Game is Active
* - Calculate FPS and Update Systems
********************************************************************************/
void CoreEngine::GameLoop() 
{
	// FPS Variables
	auto invFpsLimit = std::chrono::round<std::chrono::system_clock::duration>(std::chrono::duration<double>{ 1. / static_cast<double>(core_fps) }); // 1/60
	auto m_BeginFrame = std::chrono::system_clock::now();
	auto m_EndFrame = m_BeginFrame + invFpsLimit;
	auto prev_time_in_seconds = std::chrono::time_point_cast<std::chrono::seconds>(m_BeginFrame);

	// Console Debug Instructions
	std::cout << "########################################################" << std::endl;
	std::cout << "Press F to print out frametime performance information" << std::endl;
	std::cout << "for the current frame" << std::endl;
	//std::cout << "game window is the active window first and then press P" << std::endl;
	std::cout << "########################################################" << std::endl;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window->ptr_window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();

	// Game Loop
	while (game_active)
	{
		// (Your code calls glfwPollEvents())
		// ...
		// Start the Dear ImGui frame
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow(); // Show demo window! :)

		ImGui::Begin("Test");
		ImGui::Text("Testing text");
		ImGui::End();

		// Toggle Button to Display Debug Information on Console
		input::IsPressed(KEY::f) ? Debug_Update() : Update();

		auto time_in_seconds = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());
		++core_fps;

		// Delay until set frame time
		std::this_thread::sleep_until(m_EndFrame);

		// Update delta_time every second
		if (time_in_seconds > prev_time_in_seconds)
		{
			window->fps = core_fps;				// For printing FPS on Window Title
			dt = 1.f / core_fps;
			core_fps = 0;
			prev_time_in_seconds = time_in_seconds;
		}
		
		// Rendering
// (Your code clears your framebuffer, renders your other stuff etc.)
		ImGui::Render();
		
		//int display_w, display_h;
		//glfwGetFramebufferSize(window->ptr_window, &display_w, &display_h);
		//glViewport(0, 0, display_w, display_h);
		//glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		//glClear(GL_COLOR_BUFFER_BIT);
		
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(window->ptr_window);
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

