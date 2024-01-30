#pragma once
/* !
@file	SceneManager.h
@author Tan Yee Ann
@date	26/11/2023

This file contains the declarations for the SceneManager system class
*//*__________________________________________________________________________*/
#include <Factory.h>

class SceneManager : public ISystems {
public:
	SceneManager() {}
	~SceneManager();
	virtual void Initialize() {}
	virtual void Update() {}
	static void LoadScene(const std::string filepath);
	static void PlayScene();
	static void PauseScene();
	static void RestartScene();
	static void ClearInitialObjectMap(bool deleteObjects);
	static bool IsInitialObjectMapEmpty() { return initialObjectMap.empty(); };

	// Returns name of the component
	virtual std::string SystemName() { return "SceneManager"; }

	friend class LevelEditor;

private:
	static Factory::objectIDMap initialObjectMap;
};

extern SceneManager* sceneManager;