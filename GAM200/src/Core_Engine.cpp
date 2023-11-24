/* !
@file Core_Engine.cpp
@author Luke Goh
@date	28/9/2023

This file contains the definitions of the functions that are part of the Core Engine
*//*__________________________________________________________________________*/
#include <Precompile.h>
#include <GLWindow.h>
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
#include "Utils.h"
#include "Assets Manager/asset_manager.h"
#include <Scenes.h>
#include <Camera.h>

CoreEngine* CORE = NULL;

#if defined(DEBUG) | defined(_DEBUG)
EngineHud hud;
#endif
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

	level_size = Vec2(0.0f, 0.0f);
	start_coord = Vec2(0.0f, 0.0f);

#if defined(DEBUG) | defined(_DEBUG)
	paused = true;
#else
	paused = false;
#endif
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

#if defined(DEBUG) | defined(_DEBUG)
	hud.StartGui();
#endif

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


#if defined(DEBUG) | defined(_DEBUG)
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
#endif


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

#if defined(DEBUG) | defined(_DEBUG)
	int numOfBoxes = (int)(editor_grid->num.x * editor_grid->num.y);
	std::cout << "Number of boxes " << numOfBoxes << std::endl;
	std::vector<int> boxesFilled(numOfBoxes, 0);
	std::cout << boxesFilled.capacity();

	//ImGuiIO& io = hud.StartGui();

	bool debug_gui_active = true;
	bool show_performance_viewer = true;
	//bool objectProperties = false;
	//bool tempstorage = 1;
	//float pos_x = 0;
	//float pos_y = 0;

	//float xPos = 0;
	//float yPos = 0;

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
	long selectedObjectID = -1;

#endif
	// Game Loop
	gamehud.Initialize();

#if !(defined(DEBUG) | defined(_DEBUG))
	LoadScene("Asset/Levels/tutorial_level.json");
#endif

	// For dragging objects in level editor
	static Vec2 offset(NAN, NAN);
	static bool object_being_moved = false;

	while (game_active)
	{
		auto m_BeginFrame = std::chrono::system_clock::now();

#if defined(DEBUG) | defined(_DEBUG)	
		hud.NewGuiFrame(0);

		// Toggle Button to Display Debug Information in IMGui
		if (input::IsPressed(KEY::f)) { show_performance_viewer = !show_performance_viewer; }

		//show_performance_viewer ? Debug_Update() : Update();
#endif
		input::Update();

		for (const std::pair<std::string, ISystems*>& sys : Systems)
		{
			if (sys.first != "Window" && sys.first != "Graphics" && sys.first != "LevelEditor")
			{

#if defined(DEBUG) | defined(_DEBUG)	
				Update(sys.second);
#else
				sys.second->Update();
#endif
			}
		}
		
		/***************************************************************************************************************************************
		
		
														Drawing
		
		
		*****************************************************************************************************************************************/
#if defined(DEBUG) | defined(_DEBUG)
		if (input::IsPressed(KEY::p)) { debug_gui_active = !debug_gui_active; }
		if (debug_gui_active) {

			
			
			// For rendering into imgui window 
			glBindFramebuffer(GL_FRAMEBUFFER, level_editor_fb);

			Update(Systems["Graphics"]);
			
			gamehud.Update();
			gamehud.Draw();

			Update(Systems["Window"]);
			//editor_grid->drawleveleditor();
			
			// End rendering into imgui window 

			glBindFramebuffer(GL_FRAMEBUFFER, 0); // Back to rendering to the main window

			Update(Systems["LevelEditor"]);

			// Display the game inside the ImGui window
			//ImGui::SetNextWindowSize(ImVec2(640, 420), ImGuiCond_Always);
			ImGui::Begin("Game Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			ImVec2 windowSize = ImGui::GetWindowSize();

			ImVec2 displaySize, viewportStartPos;
			if (windowSize.y < (windowSize.x / 16.f * 9.f)) 
			{
				viewportStartPos = ImVec2((windowSize.x - (windowSize.y * 16.f / 9.f)) / 2.f, 0.f);
				displaySize = ImVec2(windowSize.y * 16.f / 9.f, windowSize.y );
			}
			else 
			{
				viewportStartPos = ImVec2(0.f, (windowSize.y - ((windowSize.x - 20.f) / 16.f * 9.f)) / 2.f);
				displaySize = ImVec2(windowSize.x, (windowSize.x / 16.f * 9.f) );
			}

			// Get the top-left position of the viewport window
			ImVec2 viewportPos = ImGui::GetWindowPos();

			// Estimate the height of the title bar
			//float titleBarHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();
			float titleBarHeight = 0.f;

			// Translate the ImGui-relative coordinates to application window-relative coordinates
			ImVec2 viewport_min = ImVec2(viewportStartPos.x + viewportPos.x, viewportStartPos.y + viewportPos.y + titleBarHeight);
			ImVec2 viewport_max = ImVec2(viewport_min.x + displaySize.x, viewport_min.y + displaySize.y);

			// Render the viewport
			ImGui::SetCursorPos(viewportStartPos);
			ImGui::Image((void*)(intptr_t)level_editor_texture, displaySize, ImVec2(0, 1), ImVec2(1, 0));

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Game object"))
				{
					const std::pair<std::string, Object*>* object = (const std::pair<std::string, Object*>*)payload->Data;

					Object* createdObj = objectFactory->cloneObject(object->second);
					objectFactory->assignIdToObject(createdObj);
					Transform* objTransform = (Transform*)createdObj->GetComponent(ComponentType::Transform);
					Body* objBody = (Body*)createdObj->GetComponent(ComponentType::Body);

					if (objTransform != nullptr)
					{
						ImVec2 objPos = convertMouseToGameViewportPos(displaySize);
						objTransform->Position.x = objPos.x;
						objTransform->Position.y = objPos.y;
					}

					if (objBody != nullptr)
					{
						RecalculateBody(objTransform, objBody);
					}
				}
				ImGui::EndDragDropTarget();
			}

			ImVec2 clickPos = ImGui::GetMousePos();
			ImVec2 relativePos(clickPos.x - viewport_min.x, clickPos.y - viewport_min.y);
			ImVec2 displayPos(relativePos.x / displaySize.x * 1920, 1080 - (relativePos.y / displaySize.y * 1080)); // Hardcoded to 1920x1080 for now, other resolutions bugged for now
			ImVec2 openGlDisplayCoord(displayPos.x - (1920 / 2), displayPos.y - (1080 / 2));

			// Camera is stationary, it's the scene that is moving, so inverse pos
			// Also, need to divide camera coord by 2 
			ImVec2 gameWorldPos((openGlDisplayCoord.x - (camera2D->position.x * 1920.f / 2.f)), (openGlDisplayCoord.y - (camera2D->position.y * 1080.f / 2.f)));

			if (ImGui::IsItemClicked()) {
				

				std::cout << "################################################################" << std::endl;
				std::cout << "ClickPos " << clickPos.x << ", " << clickPos.y << std::endl;
				std::cout << "ViewportMin " << viewport_min.x << ", " << viewport_min.y << std::endl;
				std::cout << "ViewportMax " << viewport_max.x << ", " << viewport_max.y << std::endl;
				std::cout << "ClickPos relative to viewport " << relativePos.x << ", " << relativePos.y << std::endl;
				std::cout << "DisplaySize " << displaySize.x << ", " << displaySize.y << std::endl;
				std::cout << "Translated ClickPos (in terms of opengl display) " << displayPos.x << ", " << displayPos.y << std::endl;
				std::cout << "Translated ClickPos (in terms of opengl display coord) " << openGlDisplayCoord.x << ", " << openGlDisplayCoord.y << std::endl;
				std::cout << "Translated ClickPos (in terms of game world) " << gameWorldPos.x << ", " << gameWorldPos.y << std::endl;
				
				for (size_t i = 1; i < objectFactory->NumberOfObjects(); i++)
				{
					Object* object = objectFactory->getObjectWithID(static_cast<long>(i));
					Transform* objTransform = static_cast<Transform*>(object->GetComponent(ComponentType::Transform));

					if (isObjectClicked(objTransform, gameWorldPos))
					{
						if (selectedObjectID != static_cast<long>(i)) {
							level_editor->selected = true;
							level_editor->selectedNum = (int)i;
							selectedObjectID = static_cast<long>(i);
						}

						else if (selectedObjectID == static_cast<long>(i)) {
							
						}
						
					}
				}
			}

			if (input::IsPressedRepeatedlyDelayed(KEY::mouseL, 0.1f) && level_editor->selected == true){
				Object* object;
				if (level_editor->selectedNum >= 0)
					object = objectFactory->getObjectWithID(static_cast<long>(level_editor->selectedNum));
				else
					object = AssetManager::prefabById(static_cast<long>(level_editor->selectedNum));

				Transform* objTransform = static_cast<Transform*>(object->GetComponent(ComponentType::Transform));
				Body* objBody = (Body*)object->GetComponent(ComponentType::Body);

				if (isObjectClicked(objTransform, gameWorldPos))
				{
					object_being_moved = true;
					
				}

				if (object_being_moved) {
					if (isnan(offset.x))
						offset = Vec2(gameWorldPos.x - objTransform->Position.x, gameWorldPos.y - objTransform->Position.y);

					std::cout << "Offset: " << offset << std::endl;

					objTransform->Position.x = gameWorldPos.x - offset.x;
					objTransform->Position.y = gameWorldPos.y - offset.y;

					std::cout << "objTransform->Position: " << objTransform->Position << std::endl;
				}

				if (objBody != nullptr)
				{
					RecalculateBody(objTransform, objBody);
				}
			}
			else {
				offset = Vec2(NAN, NAN);
				object_being_moved = false;
			}
			// The old and buggy version of selecting objects in the viewport
			// will be deleted at some point
			/*
			if (input::IsPressedRepeatedlyDelayed(KEY::mouseL, 0.1f) && level_editor->selected == true)
			{
				Object* object;
				if (level_editor->selectedNum >= 0)
					object = objectFactory->getObjectWithID(static_cast<long>(level_editor->selectedNum));
				else
					object = AssetManager::prefabById(static_cast<long>(level_editor->selectedNum));

				Transform* objTransform = static_cast<Transform*>(object->GetComponent(ComponentType::Transform));
				Body* objBody = (Body*)object->GetComponent(ComponentType::Body);
				ImVec2 mousePos = convertMouseToGameViewportPos(displaySize);
				if (isObjectClicked(objTransform, mousePos))
				{
					objTransform->Position.x = mousePos.x;
					objTransform->Position.y = mousePos.y;
				}

				if (objBody != nullptr)
				{
					RecalculateBody(objTransform, objBody);
				}
			}

			else if (input::IsPressed(KEY::mouseL))
			{
				for (size_t i = 1; i < objectFactory->NumberOfObjects(); i++)
				{
					Object* object = objectFactory->getObjectWithID(static_cast<long>(i));
					Transform* objTransform = static_cast<Transform*>(object->GetComponent(ComponentType::Transform));
					ImVec2 mousePos = convertMouseToGameViewportPos(displaySize);
					if (isObjectClicked(objTransform, mousePos))
					{
						level_editor->selected = true;
						level_editor->selectedNum = (int)i;
						selectedObjectID = static_cast<long>(i);
					}
				}
			}
			*/

			if (input::IsPressed(KEY::mouseR) && level_editor->selected == true)
			{
				level_editor->selected = false;
				level_editor->selectedNum = -1;
			}

			if (input::IsPressed(KEY::q) && level_editor->selected == true)
			{
				Object* object = objectFactory->getObjectWithID(static_cast<long>(level_editor->selectedNum));
				Transform* objTransform = static_cast<Transform*>(object->GetComponent(ComponentType::Transform));
				Body* objBody = (Body*)object->GetComponent(ComponentType::Body);

				objTransform->Rotation += 5.0f;

				if (objBody != nullptr)
				{
					RecalculateBody(objTransform, objBody);
				}
			}

			/*
			if (input::IsPressedRepeatedly(KEY::mouseL) && level_editor->selected == true)
			{
				Object* object = objectFactory->getObjectWithID(selectedObjectID);
				Transform* objTransform = static_cast<Transform*>(object->GetComponent(ComponentType::Transform));
				Body* objBody = static_cast<Body*>(object->GetComponent(ComponentType::Body));
				ImVec2 mousePos = convertMouseToGameViewportPos(displaySize);
				if (isObjectClicked(objTransform, mousePos) && objTransform != nullptr)
				{
					objTransform->Position.x = mousePos.x;
					objTransform->Position.y = mousePos.y;
				}

				if (objBody != nullptr)
				{
					RecalculateBody(objTransform, objBody);
				}
			}
			*/
			ImGui::End();
			/*

			ImGui::Begin("Level editor");
			ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
			if (ImGui::CollapsingHeader("Create object")) {
				ImGui::InputFloat("Input x position of object", &xPos);
				ImGui::InputFloat("Input y position of object", &yPos);
				//ImGui::ImageButton("Asset/Textures/mapbox.png", ImVec2(50, 50));
				if (ImGui::Button("Create box"))
				{
					createObject(xPos, yPos, "Asset/Objects/mapbox.json");
				}
			}

			if (ImGui::CollapsingHeader("Current Object List")) {
				ImGui::Text("Number of game objects in level: %d", objectFactory->NumberOfObjects());
				char objectPropertiesName[32];
				static int selected = -1;
				if (ImGui::BeginTable("split", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
				{
					for (size_t i = 0; i < objectFactory->NumberOfObjects(); i++)
					{
						
						if (objectFactory->getObjectWithID(i) == nullptr)
						{
							continue;
						}
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
				if (objectProperties) 
				{
					if (objectFactory->getObjectWithID(selected) == nullptr)
					{
						continue;
					}
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
					
					Vec2 botleft = { tran_pt->Position.x - tran_pt->Scale_x / 2, tran_pt->Position.y - tran_pt->Scale_y / 2 };
					Vec2 topright = { tran_pt->Position.x + tran_pt->Scale_x / 2, tran_pt->Position.y + tran_pt->Scale_y / 2 };
					app->drawline({ botleft.x,botleft.y }, { botleft.x,topright.y });
					app->drawline({ botleft.x,topright.y }, { topright.x,topright.y });
					app->drawline({ topright.x,topright.y }, { topright.x,botleft.y });
					app->drawline({ topright.x,botleft.y }, { botleft.x,botleft.y });
					
					ImGui::Text("Object Position:");
					ImGui::Text("x = %.2f, y = %.2f", tran_pt->Position.x, tran_pt->Position.y);
					if (tempstorage) {
						pos_x = tran_pt->Position.x;
						pos_y = tran_pt->Position.y;
						tempstorage = 0;
					}
					ImGui::Text("Change Object Position");
					ImGui::SliderFloat("X", &tran_pt->Position.x, -960.f, 960.f);
					ImGui::SliderFloat("Y", &tran_pt->Position.y, -540.f, 540.f);
					if (ImGui::Button("Revert")) {
						tran_pt->Position = { pos_x, pos_y };
					}
					if (ImGui::Button("Delete"))
					{
						objectFactory->destroyObject(object); 
						objectProperties = false;
					}
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

			ImGui::Begin("Game object creation");
			static int select = -1;
			for (int n = 0; n < 5; n++)
			{
				char buf[32];
				sprintf_s(buf, "Object %d", n);
				if (ImGui::Selectable(buf, select == n))
					select = n;
			}

			if (select > -1)
			{
				int leftXpos = -(editor_grid->box_size * editor_grid->num.x / 2);
				int rightXpos = (editor_grid->box_size * editor_grid->num.x / 2);
				int topYpos = (editor_grid->box_size * editor_grid->num.y / 2);
				int bottomYpos = -(editor_grid->box_size * editor_grid->num.y / 2);

				if (checkIfMouseIsWithinGrid(leftXpos, rightXpos, topYpos, bottomYpos))
				{
					int xpos = convertMousePosToGridPos(Axis::X);
					int ypos = convertMousePosToGridPos(Axis::Y);
					std::cout << "Mouse x position for grid: " << xpos << "\n";
					std::cout << "Mouse y position for grid: " << ypos << "\n";
					if (input::IsPressed(KEY::mouseL))
					{
						int xOffset = (xpos - leftXpos) / editor_grid->box_size;
						int yOffset = (ypos - topYpos) / editor_grid->box_size;
						if (boxesFilled[xOffset - (yOffset * editor_grid->num.y)] == 0)
						{
							createObject(-370 + (xOffset * editor_grid->box_size), 370 + (yOffset * editor_grid->box_size), "Asset/Objects/mapbox.json");
							boxesFilled[xOffset - (yOffset * editor_grid->num.y)] = 1;
						}

						std::cout << "Object will be placed at " << xpos << " and " << ypos << std::endl;
					}

					if (input::IsPressed(KEY::mouseR))
					{
						createObject(0, 0, "Asset/Objects/mapbox.json");
					}
				}
			}

			ImGui::End();
			*/

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

			hud.GuiRender();
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, 0); // Render direct to window

			Update(Systems["Graphics"]);
			gamehud.Update();
			gamehud.Draw();
			Update(Systems["Window"]);
			hud.GuiRender();
		}
		glfwSwapBuffers(window->ptr_window);

		level_editor->ClearAll();
#else
		Systems["Graphics"]->Update();
		gamehud.Update();
		gamehud.Draw();
		Systems["Window"]->Update();
		glfwSwapBuffers(window->ptr_window);
#endif

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

#if defined(DEBUG) | defined(_DEBUG)
	glDeleteFramebuffers(1, &level_editor_fb);
	glDeleteTextures(1, &level_editor_texture);
#endif
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

/*
int CoreEngine::convertGridToWorldPos(int gridPos, Axis axis)
{
	if (axis == Axis::X)
	{
		int xPos = gridPos + static_cast<int>(window->width / 2);
		return xPos;
	}
	else //axis == Axis::Y
	{
		int yPos = static_cast<int>(window->height / 2) - gridPos;
		return yPos;
	}
}
int CoreEngine::convertMousePosToGridPos(Axis axis)
{
	if (axis == Axis::X)
	{
		int xPos = static_cast<int>(input::GetMouseX()) - static_cast<int>(window->width / 2);
		return xPos;
	}
	else //axis == Axis::Y
	{
		int yPos = static_cast<int>(window->height / 2) - static_cast<int>(input::GetMouseY());
		return yPos;
	}
}

bool CoreEngine::checkIfMouseIsWithinGrid(int leftX, int rightX, int topY, int bottomY)
{
	int leftXPos = convertGridToWorldPos(leftX, Axis::X);
	int rightXPos = convertGridToWorldPos(rightX, Axis::X);
	int topYPos = convertGridToWorldPos(topY, Axis::Y);
	int bottomYPos = convertGridToWorldPos(bottomY, Axis::Y);

	if (input::GetMouseX() < (double)leftXPos)
	{
		std::cout << "Mouse too far left\n";
		return false;
	}

	if (input::GetMouseX() > (double)rightXPos)
	{
		std::cout << "Mouse too far right\n";
		return false;
	}

	if (input::GetMouseY() < (double)topYPos)
	{
		std::cout << "Mouse too far up\n";
		return false;
	}

	if (input::GetMouseY() > (double)bottomYPos)
	{
		std::cout << "Mouse too far down\n";
		return false;
	}

	return true;
}
*/