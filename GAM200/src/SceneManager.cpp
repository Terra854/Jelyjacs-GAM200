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

SceneManager* sceneManager;
Factory::objectIDMap SceneManager::initialObjectMap;

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
