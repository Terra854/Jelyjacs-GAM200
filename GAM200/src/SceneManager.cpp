/* !
@file	SceneManager.cpp
@author Tan Yee Ann (t.yeeann@digipen.edu)
@date	26/11/2023

This file contains the definitions of the functions that manages the game scene
*//*__________________________________________________________________________*/
#include <SceneManager.h>
#include <Core_Engine.h>
#include <GameLogic.h>
#include <Audio.h>
#include <Scenes.h>
#include "Assets Manager/asset_manager.h"
#include "../include/Font.h"
#include "../include/input.h"

SceneManager* sceneManager;
Factory::objectIDMap SceneManager::initialObjectMap;
std::vector<std::pair<std::string, std::pair<LayerSettings, std::vector<Object*>>>> SceneManager::layers;
std::vector<std::pair<std::string, std::vector<int>>> SceneManager::initialLayer;
std::map<std::string, LayerSettings> SceneManager::initialLayerSettings;
std::vector<std::string> SceneManager::AdditionalScenesLoadedConcurrently;

/******************************************************************************
	Destructor for SceneManager
*******************************************************************************/
SceneManager::~SceneManager() {
	ClearInitialObjectMap(true);
}

/******************************************************************************
	LoadScene
	- Used to switch between scenes by clearing the current object map before
	  loading the new scene
*******************************************************************************/
void SceneManager::LoadScene(const std::string filepath) {
	objectFactory->destroyAllObjects();
	SceneManager::ClearInitialObjectMap(true);
	SceneManager::layers.clear();
	SceneManager::initialLayer.clear();
	SceneManager::initialLayerSettings.clear();
	LoadSceneFromJson(filepath.c_str());
}


/******************************************************************************
	PlayScene
	- Used to start or resume the scene
*******************************************************************************/
void SceneManager::PlayScene() {
	// Do not play scene if Finn or Spark isn't inside the level
	//if (objectFactory->FindObject("Finn") == nullptr || objectFactory->FindObject("Spark") == nullptr)
	//	return;

	if (engine->isPaused()) {
		if (initialObjectMap.empty()) {
			SceneManager::BackupInitialState();
		}
		engine->setPause();
	}

}

/******************************************************************************
	PauseScene
	- Used to pause the scene
*******************************************************************************/
void SceneManager::PauseScene() {
	if (!engine->isPaused())
		engine->setPause();
}

/******************************************************************************
	RestartScene
	- Used to restart the current scene. All objects will be cleared and the initial
	  state copied over
*******************************************************************************/
void SceneManager::RestartScene() {
	audio->restartBackgroundAudio();

	// Do not reset state if intial object map is empty
	if (!initialObjectMap.empty()) {
		objectFactory->destroyAllObjects();

		for (const std::pair<int, Object*>& p : initialObjectMap) {
			objectFactory->assignIdToObject(p.second);
		}

		ClearInitialObjectMap(false);
	}
	Logic->playerObj = objectFactory->FindObject("Finn");

	// Clear the layers, all the object pointers are invalid
	layers.clear();

	// Loop through the initialLayer vector to refill the layers
	for (auto& p : initialLayer) {
		/*
		int layerNum = objectFactory->GetLayerNum(p.first);

		if (layerNum == -1) {
			layerNum = objectFactory->CreateLayer(p.first, initialLayerSettings.front());
			initialLayerSettings.pop();
		}

		for (auto& objID : p.second)
			objectFactory->AddToLayer(layerNum, objectFactory->getObjectWithID(objID));
			*/

		layers.push_back(std::make_pair(p.first, std::pair<LayerSettings, std::vector<Object*>>(initialLayerSettings[p.first], std::vector<Object*>())));

		for (auto& objID : p.second) {
			layers.back().second.second.push_back(objectFactory->getObjectWithID(objID));
		}

	}

	// The data inside initialLayer is not needed anymore now that the reset is complete
	initialLayer.clear();
	initialLayerSettings.clear();

	Logic->Initialize();

	if (!engine->isPaused())
		SceneManager::BackupInitialState();
	}

/******************************************************************************
	ClearInitialObjectMap
	- Used to clear the object map used to backup the level's initial state

	@param deleteObjects - Whether to delete the objects itself. false is for only
						   if transferring the objects to the main object map located
						   in Factory
*******************************************************************************/
void SceneManager::ClearInitialObjectMap(bool deleteObjects) {

	if (deleteObjects) {
		Factory::objectIDMap::iterator it = initialObjectMap.begin();
		for (; it != initialObjectMap.end(); ++it)
		{
			delete it->second;
		}
	}

	initialObjectMap.clear();
}

void SceneManager::UnloadScene() {
	objectFactory->destroyAllObjects();
	SceneManager::ClearInitialObjectMap(true);
	SceneManager::layers.clear();
}

void SceneManager::PlayCutScene(std::string str)
{
	std::vector<std::pair<GLuint, std::string>> cutscene;
	cutscene = AssetManager::cutsceneval(str);

	double timer = 0.0;
	long long prevTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	for (const auto& frame : cutscene)
	{
		GLuint frametexture = frame.first;
		std::string frametext = frame.second;

		std::cout << "CUTSCENE PLAYING!\n";

		std::cout << frametexture << "FRAME PLAYING!\n";

		input::Update();

		glClearColor(0.11f, 0.094f, 0.161f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//GLApp::draw_texture(Vec2{ 0.f, 0.f }, Vec2{ static_cast<float>(window->width_init), static_cast<float>(window->height_init) }, 0, frametexture, false);
		GLApp::draw_texture(Vec2{ 0.f, 0.f }, Vec2{ static_cast<float>(window->width / 2), static_cast<float>(window->height / 2) }, 0, frametexture, false);
		SetFont("Aldrich-Regular");

		float textscale{};
		textscale = static_cast<float>((3 / 4.0 * window->width) / window->width);

		DrawText(frametext, -static_cast<float>(find_width(frametext, "Aldrich-Regular")) / 2 * textscale, -static_cast<float>(window->height / 8) * 3, textscale);
		DrawText("click anywhere to continue", -250, -500, 0.7f);
		glfwSwapBuffers(window->ptr_window);

		while (timer < 5.0) {

			long long newTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			timer += static_cast<double>(newTime - prevTime) / 1000000.0;
			prevTime = newTime;

			std::cout << timer << std::endl;

			input::Update();

			if (input::IsPressed(KEY::mouseL))
				timer = 5.0;

			if (input::IsPressed(KEY::esc)) // Skip cutscene
			{
				std::cout << "CUTSCENE SKIPPED!\n";
				return;
			}
			window->Update();
		}
		timer = 0.0;
		window->Update();

		//}

		//float time{};
		//while (time < 200)
		//{
		//	std::cout << frametexture << "FRAME PLAYING!\n";

		//	GLApp::draw_texture(Vec2{ 0,0 }, Vec2{ static_cast<float>(window->width_init), static_cast<float>(window->height_init) }, 0, frametexture, false);
		//	SetFont(FONT::AldrichRegular);
		//	DrawText(frametext, 0, -(window->height_init / 4.0), window->width_init / 2.0);


		//	if (input::IsPressed(KEY::esc)) // Skip cutscene
		//	{
		//		std::cout << "CUTSCENE SKIPPED!\n";
		//		return;
		//	}

		//	time += engine->GetDt();
		//}
	}
	glClearColor(0.11f, 0.094f, 0.161f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void SceneManager::BackupInitialState() {
	for (const std::pair<int, Object*>& p : objectFactory->objectMap) {
		initialObjectMap[p.first] = objectFactory->cloneObject(p.second);
	}

	for (auto& l : SceneManager::layers) {
		initialLayerSettings[l.first] = l.second.first;

		auto la = std::make_pair(l.first, std::vector<int>());

		for (auto& object : l.second.second) {
			la.second.push_back(object->GetId());
		}

		initialLayer.push_back(la);
	}
}



void SceneManager::CalculateLevelSize() {
		// Calculate the level size
	float maxX = 0, maxY = 0;

	Vec2 start_coord, end_coord;

	for (const auto& p : objectFactory->objectMap) {
		Object* obj = p.second;
		Rectangular* r = (Rectangular*)obj->GetComponent(ComponentType::Body);

		if (r) {
			start_coord = Vec2(r->aabb.min.x < start_coord.x ? r->aabb.min.x : start_coord.x, r->aabb.min.y < start_coord.y ? r->aabb.min.y : start_coord.y);
			end_coord = Vec2(r->aabb.max.x > end_coord.x ? r->aabb.max.x : end_coord.x, r->aabb.max.y > end_coord.y ? r->aabb.max.y : end_coord.y);
		}
	}

	engine->Set_Start_Coords(start_coord);
	engine->Set_End_Coords(end_coord);
}