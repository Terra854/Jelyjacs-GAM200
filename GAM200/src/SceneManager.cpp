/* !
@file	SceneManager.cpp
@author Tan Yee Ann
@date	26/11/2023

This file contains the definitions of the functions that manages the game scene
*//*__________________________________________________________________________*/
#include <SceneManager.h>
#include <Core_Engine.h>
#include <GameLogic.h>
#include <Audio.h>
#include <Scenes.h>
#include "Assets Manager/asset_manager.h"

SceneManager* sceneManager;
Factory::objectIDMap SceneManager::initialObjectMap;
std::vector<std::pair<std::string, std::pair<bool, std::vector<Object*>>>> SceneManager::layers;
std::multimap<std::string, int> SceneManager::initialLayer;

/******************************************************************************
	Destructor for SceneManager
*******************************************************************************/
SceneManager::~SceneManager(){
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

	for (const auto& frame : cutscene)
	{
		GLuint frametexture = frame.first;
		std::string frametext = frame.second;

		// frametexture is the jpg and frametext is the text
		// Draw the img and font here along with the click test to proceed to the next frame




	}



}





