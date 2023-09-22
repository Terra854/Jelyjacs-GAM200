#pragma once
#include <Debug.h>

// Core Engine that manage all systems in game. Update and destroying them.
#include "Interface_System.h"
#include <vector>  // Remove after adding this header somewhere else
#include <message.h>
#include <unordered_map>


class CoreEngine {
public:
	// Constructor to initiate variables to 0.
	CoreEngine();
	~CoreEngine();

	// Initialize all the system
	void Initialize();
	// Main Game Loop - looping all the system updates
	void GameLoop();
	// Add the system into the map
	void AddSystem(ISystems* sys);
	// Loop for systems to send messages to each other.
	void DeleteSystem();
	void Broadcast(Message *msg);

private:
	// Map of Systems
	std::unordered_map<std::string, ISystems*> Systems;
	// For frame time
	unsigned last_update;
	// checking if game is active
	bool game_active;
};

// To quit the game
class MessageQuit : public Message {
public:
	MessageQuit() : Message(MessageID::Quit) {};
};

extern CoreEngine* Core;