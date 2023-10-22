#pragma once

#include <Debug.h>

#include "Interface_System.h"
#include <map>


class DebugGui : public ISystems
{
public:
	//Constructor to initialise pointer
	// It's broken, will be purged in the future
	virtual void Initialize();
	void Update();
	virtual std::string SystemName() { return "DebugGui"; }

	void SetSystemElapsedTime(std::string name, double time) { System_elapsed_time[name] = time; }
	//void ClearSystemElapsedTime() { System_elapsed_time.clear(); }
	void SetTotalTime(double time) { total_time = time; }
	void AddTotalTime(double time) { total_time += time; }
	void ClearAll() { System_elapsed_time.clear();  total_time = 0.0; }
private:
	std::map<std::string, double> System_elapsed_time;
	double total_time;
};

extern DebugGui* debug_gui; // declared in DebugGui.cpp