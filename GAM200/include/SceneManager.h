#pragma once
#include <Factory.h>

class SceneManager : public ISystems {
public:
	SceneManager() {}
	~SceneManager();
	virtual void Initialize() {}
	virtual void Update() {}
	static void PlayScene();
	static void PauseScene();
	static void RestartScene();
	static void ClearInitialObjectMap(bool deleteObjects);
	static bool IsInitialObjectMapEmpty() { return initialObjectMap.empty(); };
	virtual std::string SystemName() { return "SceneManager"; }

	friend class LevelEditor;

private:
	static Factory::objectIDMap initialObjectMap;
};

extern SceneManager* sceneManager;