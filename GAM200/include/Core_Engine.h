#pragma once
/* !
@file	Core_Engine.h
@author Luke Goh
@date	28/9/2023

This file contains the declaration for the Core Engine class
*//*__________________________________________________________________________*/
#include <Debug.h>

// Core Engine that manage all systems in game. Update and destroying them.
#include "Interface_System.h"
#include <message.h>
#include <unordered_map>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <Imgui/imgui_impl_opengl3.h>

enum Axis
{
	X,
	Y
};

class CoreEngine 
{
public:
	// Constructor to initiate variables to 0.
	CoreEngine();
	~CoreEngine();

	// Initialize all the system
	void Initialize();

	// Update all the System and FPS
	//void Update();

	// Debug Update function
	//void Debug_Update();

	// Main Game Loop - looping all the system updates
	void GameLoop();

	// Add the system into the map
	void AddSystem(ISystems* sys);

	// Delete all the system from the map
	void DeleteSystem();

	// Broadcast messages to system
	// Acts as a MessageRelay for Core Engine
	void Broadcast(Message_Handler *msg);

	void createObject(float posX, float posY, std::string objectName);

	int convertGridToWorldPos(int gridPos, Axis axis);
	int convertMousePosToGridPos(Axis axis);
	bool checkIfMouseIsWithinGrid(int leftX, int rightX, int topY, int bottomY);

	float GetDt() { return dt; }
	float Get_FPS() { return core_fps; }
	float Get_Fixed_DT() { return fixed_dt; }

	// Check if the game is paused
	void setPause() { paused = !paused; }
	bool isPaused() { return paused; }

	std::string loaded_level, loaded_filename;

	friend class LevelEditor; // For displaying performance info
	
private:
	// Map of Systems (Container)
	std::unordered_map<std::string, ISystems*> Systems;

	// FPS
	float core_fps;

	float fixed_dt;

	// DT
	float dt;

	// Checking if game is active
	bool game_active;

	bool paused;
};

extern CoreEngine* engine;