#pragma once
/* !
@file	LevelEditor.h
@author Tan Yee Ann (t.yeeann@digipen.edu)
@date	2/11/2023

This file contains the declarations for the LevelEditor system class
*//*__________________________________________________________________________*/

#if defined(DEBUG) | defined(_DEBUG)

#include <Debug.h>
#include <Factory.h>
#include "Vec2.h"
#include "Interface_System.h"
#include <map>


class LevelEditor : public ISystems
{
public:
	void DebugUniformGrid();
	void DebugPerformanceViewer();
	void ObjectProperties();
	void ListOfObjects();
	void DisplaySelectedTexture();
	void AssetList();
	void PlayPauseGame();
	//void ClearLevelEditorObjectMap(bool deleteObjects);
	//Constructor to initialise pointer
	
	LevelEditor();
	~LevelEditor();

	void LoadLevelPanel();

	void SaveAsDialog();

	void UpdateAllObjectInstances(Object* object);

	void LoadedLevelProperties();

	void RemoveDependentScene(std::string filename);

	virtual void Initialize();
	void Update();

	// Returns name of the component
	virtual std::string SystemName() { return "LevelEditor"; }

	void SetSystemElapsedTime(std::string name, double time) { System_elapsed_time[name] = time; }
	void SetTotalTime(double time) { total_time = time; }
	void AddTotalTime(double time) { total_time += time; }
	void ClearAll() { System_elapsed_time.clear();  total_time = 0.0; }

	int selectedNum = -1; // Negative values for prefabs, positive values for objects
	bool selected = false;

private:
	std::map<std::string, double> System_elapsed_time;
	double total_time;

	// When the play button os pressed, the level editor will make a copy of the initial
	// state of the level so that the user can reset it later
	Factory::objectIDMap initialObjectMap;

	void LE_InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0);
	void LE_InputInt(const char* label, int* v);
	void LE_InputFloat(const char* label, float* v);
	void LE_InputFloat2(const char* label, float* v);
	void LE_InputTextMultiline(const char* label, char* buf, size_t buf_size);
	void DeleteSound(std::string audioType, int audio_num);
	void AddTexture();
	void AddAnimation();
	void AddAudio();
	static std::string OpenFileDialog(int type);
};

extern LevelEditor* level_editor; // declared in LevelEditor.cpp

#endif