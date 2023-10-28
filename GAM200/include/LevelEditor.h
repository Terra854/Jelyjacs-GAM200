#pragma once

#include <Debug.h>

#include "Interface_System.h"
#include <map>


class LevelEditor : public ISystems
{
public:
	void ShowDockSpace(bool* p_open);
	void DebugUniformGrid();
	void DebugPerformanceViewer();
	void ObjectProperties();
	void ListOfObjects();
	//Constructor to initialise pointer
	// It's broken, will be purged in the future
	virtual void Initialize();
	void Update();
	virtual std::string SystemName() { return "LevelEditor"; }

	void SetSystemElapsedTime(std::string name, double time) { System_elapsed_time[name] = time; }
	//void ClearSystemElapsedTime() { System_elapsed_time.clear(); }
	void SetTotalTime(double time) { total_time = time; }
	void AddTotalTime(double time) { total_time += time; }
	void ClearAll() { System_elapsed_time.clear();  total_time = 0.0; }
private:
	std::map<std::string, double> System_elapsed_time;
	double total_time;
};

extern LevelEditor* level_editor; // declared in LevelEditor.cpp