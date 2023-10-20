/* !
@file Core_Engine.cpp
@author Luke Goh
@date	28/9/2023

This file contains the definitions of the functions that are part of the Core Engine
*//*__________________________________________________________________________*/
#include <Precompile.h>
#include <GLWindow.h>
#include <GL/glew.h>
#include <Debug.h>
#include "Core_Engine.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <input.h>
#include <map>
#include <thread>
#include "EngineHud.h"
#include <GameStateManager.h>
#include <GameStateList.h>
#include <Object.h>
#include <Factory.h>
#include <Collision.h>
#include "stb_image.h"

CoreEngine* CORE = NULL;
EngineHud hud;
ImVec4 clear_color;

bool CoreEngine::LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
	// Load from file
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	// Upload pixels into texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	return true;
}

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
	GSM_Initialize(GS_TEST);
	GSM_Update();
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

	//auto invFpsLimit = std::chrono::round<std::chrono::system_clock::duration>(std::chrono::duration<double>{ 1. / fixed_dt }); // 1/60
	//std::chrono::time_point<std::chrono::system_clock> m_EndFrame;
	

	// Console Debug Instructions
	//std::cout << "########################################################" << std::endl;
	//std::cout << "Press F to print out frametime performance information" << std::endl;
	//std::cout << "for the current frame" << std::endl;
	//std::cout << "game window is the active window first and then press P" << std::endl;
	//std::cout << "########################################################" << std::endl;

	ImGuiIO& io = hud.StartGui();

	bool show_performance_viewer = true;
	int xPos = 0;
	int yPos = 0;

	// Game Loop
	while (current != GS_QUIT)
	{
		auto m_BeginFrame = std::chrono::system_clock::now();
		hud.NewGuiFrame();

		// Toggle Button to Display Debug Information in IMGui
		if (input::IsPressed(KEY::f)) { show_performance_viewer = !show_performance_viewer; }

		//show_performance_viewer ? Debug_Update() : Update();

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

		ImGui::SetNextWindowSize(ImVec2(500, 220));

		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
		ImGui::Begin("DEBUG: Performance Viewer");

		for (std::pair<std::string, double> p : elapsed_time)
			ImGui::Text("%s system completed it's update in %.6f seconds (%.2f%%)", p.first.c_str(), p.second, (p.second / total_time * 100.0));

		ImGui::Text("############################################################");
		ImGui::Text("Total time taken for this frame: %.6f seconds.", total_time);
		ImGui::Text("Frame Rate is: %.6f FPS", core_fps);

		ImGui::End();

		ImGui::Begin("Level editor");

		ImGui::InputInt("Input x position of object", &xPos);
		ImGui::InputInt("Input y position of object", &yPos);
		if (ImGui::Button("Create box"))
		{
			createObject(xPos, yPos, "../Asset/Objects/mapbox.json");
		}
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(200, 200), ImGuiCond_Once);
		ImGui::Begin("Game objects");
		ImGui::Text("Number of game objects in level: %d", objectFactory->NumberOfObjects());
		for (size_t i = 0; i < objectFactory->NumberOfObjects(); i++)
		{
			Object* object = objectFactory->getObjectWithID(i);

			if (!object->GetName().empty())
				ImGui::Text("%s", object->GetName().c_str());
			else
				ImGui::Text("Object number %d", i);
		}
		ImGui::End();

		int image_width = 0;
		int image_height = 0;
		GLuint image_texture = 0;
		bool ret = LoadTextureFromFile("../Asset/Picture/MyImage01.jpg", &image_texture, &image_width, &image_height);
		IM_ASSERT(ret);

		ImGui::Begin("OpenGL Texture Text");
		ImGui::Text("pointer = %p", image_texture);
		ImGui::Text("size = %d x %d", image_width, image_height);
		ImGui::Image((void*)(intptr_t)image_texture, ImVec2(image_width, image_height));
		ImGui::End();

		elapsed_time.clear();
		total_time = 0.0;

		hud.GuiRender(io);

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
			std::cout << "FPS: " << core_fps << std::endl;
			std::cout << "DT: " << dt << std::endl;
			prev_time_in_seconds = time_in_seconds;
		}
	

		// Updating Frame Times
		//m_BeginFrame = m_EndFrame;
		//m_EndFrame = m_BeginFrame + invFpsLimit;
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
		current = GS_QUIT;
		game_active = false;
	}
	// Loop Messaging System
	for (const std::pair<std::string, ISystems*>& sys : Systems)
	{
		sys.second->MessageRelay(msg);
	}
}

void CoreEngine::createObject(int posX, int posY, std::string objectName)
{
	Object* testingObject = objectFactory->createObject(objectName);
	long testingObjectID = testingObject->GetId();
	Transform* tran_pt = static_cast<Transform*>((objectFactory->getObjectWithID(testingObjectID))->GetComponent(ComponentType::Transform));
	tran_pt->Position.x = posX;
	tran_pt->Position.y = posY;
}