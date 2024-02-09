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
std::vector<std::pair<std::string, std::pair<bool, std::vector<Object*>>>> SceneManager::layers;
std::multimap<std::string, int> SceneManager::initialLayer;

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
	LoadSceneFromJson(filepath.c_str());
}


/******************************************************************************
	PlayScene
	- Used to start or resume the scene
*******************************************************************************/
void SceneManager::PlayScene() {
	// Do not play scene if Finn or Spark isn't inside the level
	if (objectFactory->FindObject("Finn") == nullptr || objectFactory->FindObject("Spark") == nullptr)
		return;

	if (engine->isPaused()) {
		if (initialObjectMap.empty()) {
			for (const std::pair<int, Object*>& p : objectFactory->objectMap) {
				initialObjectMap[p.first] = objectFactory->cloneObject(p.second);
			}

			for (auto& l : SceneManager::layers) {
				for (auto& object : l.second.second) {
					initialLayer.insert(std::make_pair(l.first, object->GetId()));
				}
			}
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

	// Loop through the initialLayer multimap to refill the layers
	for (auto& p : initialLayer) {
		int layerNum = objectFactory->GetLayerNum(p.first);

		if (layerNum == -1) {
			layerNum = objectFactory->CreateLayer(p.first, true);
		}

		objectFactory->AddToLayer(layerNum, objectFactory->getObjectWithID(p.second));
	}

	// The data inside initialLayer is not needed anymore now that the reset is complete
	initialLayer.clear();
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
		SetFont(FONT::AldrichRegular);

		float textscale{};
		textscale = static_cast<float>((3 / 4.0 * window->width) / window->width);

		DrawText(frametext, -static_cast<float>(find_width(frametext, FONT::AldrichRegular)) / 2 * textscale, -static_cast<float>(window->height / 8) * 3, textscale);
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





