#include <SceneManager.h>
#include <Core_Engine.h>
#include <GameLogic.h>
#include <Audio.h>

SceneManager* sceneManager;
Factory::objectIDMap SceneManager::initialObjectMap;

SceneManager::~SceneManager(){
	ClearInitialObjectMap(true);
}

void SceneManager::PlayScene() {
	if (engine->isPaused()) {
		if (initialObjectMap.empty()) {
			for (const std::pair<int, Object*>& p : objectFactory->objectMap) {
				initialObjectMap[p.first] = objectFactory->cloneObject(p.second);
			}
		}
		engine->setPause();
	}

}

void SceneManager::PauseScene() {
	if (!engine->isPaused())
		engine->setPause();

	audio->stopWalking();
}

void SceneManager::RestartScene() {
	audio->stopWalking();

	objectFactory->destroyAllObjects();

	for (const std::pair<int, Object*>& p : initialObjectMap) {
		objectFactory->assignIdToObject(p.second);
	}

	ClearInitialObjectMap(false);

	Logic->playerObj = objectFactory->FindObject("Finn");
}

/******************************************************************************
ClearLevelEditorObjectMap
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
