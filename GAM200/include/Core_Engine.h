#pragma once
/* !
@file Core_Engine.h
@author Luke Goh
@date	28/9/2023

This file contains the declaration for the Core Engine class
*//*__________________________________________________________________________*/
#include <Debug.h>

// Core Engine that manage all systems in game. Update and destroying them.
#include "Interface_System.h"
#include <message.h>
#include <unordered_map>


class CoreEngine {
public:
	// Constructor to initiate variables to 0.
	CoreEngine();
	~CoreEngine();

	// Initialize all the system
	void Initialize();

	// Update all the System and FPS
	void Update(const float& dt);

	// Debug Update function
	void Debug_Update(const float& dt);

	// Main Game Loop - looping all the system updates
	void GameLoop();

	// Add the system into the map
	void AddSystem(ISystems* sys);

	// Delete all the system from the map
	void DeleteSystem();

	// Broadcast messages to system
	// Acts as a MessageRelay for Core Engine
	void Broadcast(Message_Handler *msg);

	
private:
	// Map of Systems (Container)
	std::unordered_map<std::string, ISystems*> Systems;

	// FPS
	unsigned core_fps;

	// Checking if game is active
	bool game_active;
};

extern CoreEngine* engine;