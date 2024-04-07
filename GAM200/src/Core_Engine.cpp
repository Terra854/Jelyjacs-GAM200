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
#include "Utils.h"
#include "Assets Manager/asset_manager.h"
#include <Scenes.h>
#include <Camera.h>
#include <SceneManager.h>
#include <cmath> 
#include <Gizmo.h>

CoreEngine* CORE = NULL;

#if defined(DEBUG) | defined(_DEBUG)
EngineHud hud;
#endif
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

	start_coord = Vec2(-960.0f, -540.0f);
	end_coord = Vec2(960.0f, 540.0f);

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
* - Checks how long each system updates itself and saves it for the performance viewer
* - Use microseconds for more accuracy
*******************************************************************************/

// Variables that will be needed by Update and GameLoop
long long start_system_time, end_system_time;

#if defined(DEBUG) | defined(_DEBUG)
void Update(ISystems* sys)
{
	start_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	sys->Update();
	end_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	level_editor->SetSystemElapsedTime(sys->SystemName(), (double)(end_system_time - start_system_time) / 1000000.0);
	level_editor->AddTotalTime((double)(end_system_time - start_system_time) / 1000000.0);
}
#endif

/********************************************************************************
* Game Loop
* - Initialising Variables for FPS Calculation
* - When Game is Active
* - Calculate FPS and Update Systems
********************************************************************************/
void CoreEngine::GameLoop()
{
	// For fixed dt
	double accumulator = 0.0;

	audio->setupSound();
#if defined(DEBUG) | defined(_DEBUG)

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
	long selectedObjectID = -1;

	/* For dragging objects into the viewport */
	bool DraggingPrefabIntoViewport = false;

	// For dragging objects in level editor
	static Vec2 offset(NAN, NAN);
	static bool object_being_moved = false;

	static bool object_being_moved_x = false;
	static bool object_being_moved_y = false;
#else
	SceneManager::LoadScene("opening_cutscene.json");
#endif

	// Game Loop
	while (game_active)
	{
		auto m_BeginFrame = std::chrono::system_clock::now();

#if defined(DEBUG) | defined(_DEBUG)	
		hud.NewGuiFrame(0);
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

			Update(Systems["Window"]);

			// End rendering into imgui window 

			glBindFramebuffer(GL_FRAMEBUFFER, 0); // Back to rendering to the main window

			Update(Systems["LevelEditor"]);

			// Display the game inside the ImGui window
			ImGui::Begin("Game Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			ImVec2 windowSize = ImGui::GetWindowSize();

			Vec2 viewportStartPos;
			if (windowSize.y < (windowSize.x / 16.f * 9.f))
			{
				viewportStartPos = Vec2((windowSize.x - (windowSize.y * 16.f / 9.f)) / 2.f, 0.f);
				viewportDisplaySize = Vec2(windowSize.y * 16.f / 9.f, windowSize.y);
			}
			else
			{
				viewportStartPos = Vec2(0.f, (windowSize.y - ((windowSize.x - 20.f) / 16.f * 9.f)) / 2.f);
				viewportDisplaySize = Vec2(windowSize.x, (windowSize.x / 16.f * 9.f));
			}

			// Get the top-left position of the viewport window
			ImVec2 viewportPos = ImGui::GetWindowPos();

			// Estimate the height of the title bar
			float titleBarHeight = 0.f;

			// Translate the ImGui-relative coordinates to application window-relative coordinates
			ImVec2 viewport_min = ImVec2(viewportStartPos.x + viewportPos.x, viewportStartPos.y + viewportPos.y + titleBarHeight);
			ImVec2 viewport_max = ImVec2(viewport_min.x + viewportDisplaySize.x, viewport_min.y + viewportDisplaySize.y);

			// Render the viewport
			ImGui::SetCursorPos(ImVec2(viewportStartPos.x, viewportStartPos.y));
			ImGui::Image((void*)(intptr_t)level_editor_texture, ImVec2(viewportDisplaySize.x, viewportDisplaySize.y), ImVec2(0, 1), ImVec2(1, 0));
			
			// Calculate the mouse position relative to the game world
			ImVec2 clickPos = ImGui::GetMousePos();
			ImVec2 relativePos(clickPos.x - viewport_min.x, clickPos.y - viewport_min.y);
			ImVec2 displayPos(relativePos.x / viewportDisplaySize.x * (float)window->width, (float)window->height - (relativePos.y / viewportDisplaySize.y * (float)window->height));
			ImVec2 openGlDisplayCoord((displayPos.x - ((float)window->width / 2.f)) / camera2D->scale.x, (displayPos.y - ((float)window->height / 2.f)) / camera2D->scale.y);

			// Camera is stationary, it's the scene that is moving, so inverse pos
			// Also, need to divide camera coord by 2 
			ImVec2 gameWorldPos((openGlDisplayCoord.x - (camera2D->position.x * (float)window->width / 2.f)), (openGlDisplayCoord.y - (camera2D->position.y * (float)window->height / 2.f)));

			//Accept drag and drop of game prefabs into the game scene
			if (ImGui::BeginDragDropTarget())
			{
				DraggingPrefabIntoViewport = true; // Make sure we are not accidentally dragging any objects in the viewport

				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Game object"))
				{
					const std::pair<std::string, Object*>* object = (const std::pair<std::string, Object*>*)payload->Data;

					Object* createdObj = objectFactory->cloneObject(object->second);
					createdObj->SetPrefab(object->second);

					objectFactory->assignIdToObject(createdObj);
					SceneManager::CalculateLevelSize();

					objectFactory->AddToLayer(static_cast<int>(sceneManager->layers.size() - 1), createdObj);
					Transform* objTransform = (Transform*)createdObj->GetComponent(ComponentType::Transform);
					Body* objBody = (Body*)createdObj->GetComponent(ComponentType::Body);

					if (objTransform != nullptr)
					{
						objTransform->Position.x = (float)std::round(gameWorldPos.x);
						objTransform->Position.y = (float)std::round(gameWorldPos.y);
					}

					if (objBody != nullptr)
					{
						RecalculateBody(objTransform, objBody);
					}

					level_editor->selected = true;
					level_editor->selectedNum = (int)createdObj->GetId();
					selectedObjectID = static_cast<long>(createdObj->GetId());
				}
				ImGui::EndDragDropTarget();
			}
			else {
				DraggingPrefabIntoViewport = false;
			}

			static Transform XGizmo, YGizmo;
			static float RGizmo_Angle, Initial_Rotation;
			static Vec2 initialScale, initialScaleFactor;

			// Dragging the selected object in the viewport (only when the engine is paused)
			if (input::IsPressedRepeatedlyDelayed(KEY::mouseL, 0.1f) && level_editor->selected == true && !DraggingPrefabIntoViewport && engine->isPaused()) {
				Object* object;
				if (level_editor->selectedNum >= 0)
					object = objectFactory->getObjectWithID(static_cast<long>(level_editor->selectedNum));
				else
					object = AssetManager::prefabById(static_cast<long>(level_editor->selectedNum));

				Transform* objTransform = static_cast<Transform*>(object->GetComponent(ComponentType::Transform));
				Body* objBody = (Body*)object->GetComponent(ComponentType::Body);

				if (objTransform) {

					if (gizmo.GetType() == GizmoType::Translate || gizmo.GetType() == GizmoType::Scale) {
						/* Gizmo check for Scale and Translate*/
						if (!object_being_moved_x && !object_being_moved_y)
						{

							if (isObjectClicked(gizmo.getX(), gameWorldPos))
								object_being_moved_x = true;
							else if (isObjectClicked(gizmo.getY(), gameWorldPos))
								object_being_moved_y = true;

							// Offset to account for mouse not being in the center of the selected object
							if (isnan(offset.x))
								offset = Vec2(gameWorldPos.x - objTransform->Position.x, gameWorldPos.y - objTransform->Position.y);

							initialScale = objTransform->Scale;
							initialScaleFactor = gameWorldPos;
						}

						/* X Gizmo */
						if (object_being_moved_x) {
							if (gizmo.GetType() == GizmoType::Translate)
								objTransform->Position.x = (float)std::round(gameWorldPos.x - offset.x);
							else if (gizmo.GetType() == GizmoType::Scale) {
								objTransform->Scale.x = (float)std::round((Vec2(gameWorldPos).x - initialScaleFactor.x + 100.f) / 100.f * initialScale.x);
							}
						}

						/* Y Gizmo */
						if (object_being_moved_y) {
							if (gizmo.GetType() == GizmoType::Translate)
								objTransform->Position.y = (float)std::round(gameWorldPos.y - offset.y);
							else if (gizmo.GetType() == GizmoType::Scale)
								objTransform->Scale.y = (float)std::round((Vec2(gameWorldPos).y - initialScaleFactor.y + 100.f) / 100.f * initialScale.y);
						}

						if (object_being_moved_x || object_being_moved_y) {
							if (gizmo.GetType() == GizmoType::Translate)
								ImGui::SetTooltip("x: %.6f\ny: %.6f", objTransform->Position.x, objTransform->Position.y);
							else if (gizmo.GetType() == GizmoType::Scale)
								ImGui::SetTooltip("Width: %.6f\nHeight: %.6f", objTransform->Scale.x, objTransform->Scale.y);
						}
					}
					/* R Gizmo */
					else if (gizmo.GetType() == GizmoType::Rotate) {
						if (gizmo.IsRGizmoClicked(gameWorldPos) && !gizmo.IsRGizmoActive()) {
							gizmo.SetRGizmoActive(true);
							RGizmo_Angle = calculateAngle(objTransform->Position, Vec2(gameWorldPos));
							Initial_Rotation = objTransform->Rotation;
						}

						if (gizmo.IsRGizmoActive()) {
							objTransform->Rotation = (float)((int)(Initial_Rotation + (calculateAngle(objTransform->Position, Vec2(gameWorldPos)) - RGizmo_Angle)));

							if (objTransform->Rotation > 360.0f)
								objTransform->Rotation -= 360.f;
							else if (objTransform->Rotation < 0.0f)
								objTransform->Rotation += 360.f;

							ImGui::SetTooltip("Rotation: %.6f", objTransform->Rotation);

							std::cout << "Initial_Rotation: " << Initial_Rotation << std::endl;
							std::cout << "RGizmo_Angle: " << RGizmo_Angle << std::endl;
							std::cout << "calculateAngle: " << calculateAngle(objTransform->Position, Vec2(gameWorldPos)) << std::endl;
							std::cout << "Angle result: " << calculateAngle(objTransform->Position, Vec2(gameWorldPos)) - RGizmo_Angle << std::endl;
							std::cout << "Angle: " << objTransform->Rotation << std::endl;
						}
					}

					// Recalculate body if it exists
					if (objBody != nullptr)
					{
						RecalculateBody(objTransform, objBody);
					}
				}
			}
			// Select object in the viewport
			else if (ImGui::IsItemClicked() && !isObjectClicked(gizmo.getX(), gameWorldPos) && !isObjectClicked(gizmo.getY(), gameWorldPos) && !gizmo.IsRGizmoClicked(gameWorldPos)) {
				std::cout << "################################################################" << std::endl;
				std::cout << "ClickPos " << clickPos.x << ", " << clickPos.y << std::endl;
				std::cout << "ViewportMin " << viewport_min.x << ", " << viewport_min.y << std::endl;
				std::cout << "ViewportMax " << viewport_max.x << ", " << viewport_max.y << std::endl;
				std::cout << "ClickPos relative to viewport " << relativePos.x << ", " << relativePos.y << std::endl;
				std::cout << "DisplaySize " << viewportDisplaySize.x << ", " << viewportDisplaySize.y << std::endl;
				std::cout << "Translated ClickPos (in terms of opengl display) " << displayPos.x << ", " << displayPos.y << std::endl;
				std::cout << "Translated ClickPos (in terms of opengl display coord) " << openGlDisplayCoord.x << ", " << openGlDisplayCoord.y << std::endl;
				std::cout << "Translated ClickPos (in terms of game world) " << gameWorldPos.x << ", " << gameWorldPos.y << std::endl;

				for (auto& l : SceneManager::layers) {
					if (l.second.first.isVisible) {
						for (auto& object : l.second.second) {
							Transform* objTransform = static_cast<Transform*>(object->GetComponent(ComponentType::Transform));

							if (isObjectClicked(objTransform, gameWorldPos))
							{
								level_editor->selected = true;
								level_editor->selectedNum = (int)object->GetId();
								selectedObjectID = object->GetId();
							}
						}
					}
				}
			}
			else {
				offset = Vec2(NAN, NAN);
				object_being_moved = false;
				object_being_moved_x = false;
				object_being_moved_y = false;
				gizmo.SetRGizmoActive(false);
			}


			// Deselect the object
			if (input::IsPressed(KEY::mouseR) && level_editor->selected == true)
			{
				level_editor->selected = false;
				level_editor->selectedNum = -1;
			}

			//Rotate the object
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

			ImGui::End(); // End Game Viewport

			// Gizmo controls window (hide when game is running)
			if (level_editor->selected && level_editor->selectedNum >= 0 && engine->isPaused()) {

				Object* selectObj = objectFactory->getObjectWithID(static_cast<long>(level_editor->selectedNum));
				Transform* selectObjTransform = static_cast<Transform*>(selectObj->GetComponent(ComponentType::Transform));

				if (selectObjTransform) {

					// Calculate location of the window
					Vec2 vecnum1(
						(selectObjTransform->Position.x + (camera2D->position.x * (float)window->width / 2.f)),
						(selectObjTransform->Position.y - (selectObjTransform->Scale.y / 2.f) + (camera2D->position.y * (float)window->height / 2.f))
					);
					Vec2 vecnum2(
						(vecnum1.x * camera2D->scale.x) + ((float)window->width / 2),
						(vecnum1.y * camera2D->scale.y) + ((float)window->height / 2)
					);

					Vec2 vecnum3(
						vecnum2.x / (float)window->width * viewportDisplaySize.x,
						((float)window->height - vecnum2.y) / (float)window->height * viewportDisplaySize.y
					);

					Vec2 vecnum4(
						vecnum3.x + viewport_min.x,
						vecnum3.y + viewport_min.y
					);

					Vec2 gizmoControlButtonPos = vecnum4 + Vec2(-90.f, 10.f);

					ImGui::SetNextWindowPos(gizmoControlButtonPos.ToImVec2());
					ImGui::SetNextWindowSize(ImVec2(180.f, 60.f));
					ImGui::Begin("Gizmo Controls", nullptr, ImGuiWindowFlags_NoResize);

					if (ImGui::Button("Scale")) {
						gizmo.SetType(GizmoType::Scale);
					}
					ImGui::SameLine();
					if (ImGui::Button("Rotate")) {
						gizmo.SetType(GizmoType::Rotate);
					}
					ImGui::SameLine();
					if (ImGui::Button("Translate")) {
						gizmo.SetType(GizmoType::Translate);
					}

					ImGui::End();
				}
			}

			hud.GuiRender();
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, 0); // Render direct to window

			Update(Systems["Graphics"]);
			Update(Systems["Window"]);
			hud.GuiRender();
		}
		glfwSwapBuffers(window->ptr_window);

		level_editor->ClearAll();
#else
		Systems["Graphics"]->Update();
		Systems["Window"]->Update();
		glfwSwapBuffers(window->ptr_window);
#endif
		// FPS Calculation
		auto m_EndFrame = std::chrono::system_clock::now();
		dt = std::chrono::duration<float>(m_EndFrame - m_BeginFrame).count();			// Delta Time
		core_fps = 1.f / dt;															// FPS
		window->fps = core_fps;
		accumulator += dt;																// Accumulator

		numofsteps = 0;
		while (accumulator >= Get_Fixed_DT())											// Fixed Time Step
		{
			accumulator -= Get_Fixed_DT();
			numofsteps++;
		}
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