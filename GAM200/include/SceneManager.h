#pragma once
/* !
@file	SceneManager.h
@author Tan Yee Ann (t.yeeann@digipen.edu)
@date	26/11/2023

This file contains the declarations for the SceneManager system class
*//*__________________________________________________________________________*/
#include <Factory.h>
#include "Scenes.h"

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
	static void PlayCutScene(std::string);

	// Returns name of the component
	virtual std::string SystemName() { return "SceneManager"; }

	friend class LevelEditor;
	friend class Factory;
	friend class GLApp;
	friend class CoreEngine;
	friend void ::SaveScene(std::string);

private:
	static Factory::objectIDMap initialObjectMap;
	static std::vector<std::pair<std::string, std::pair<bool, std::vector<Object*>>>> layers;
	static std::multimap<std::string, int> initialLayer;
};

extern SceneManager* sceneManager;