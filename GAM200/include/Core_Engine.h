#pragma once
#include <Debug.h>

// Core Engine that manage all systems in game. Update and destroying them.
#include "Interface_System.h"
#include <vector>  // Remove after adding this header somewhere else
#include <message.h>
#include <unordered_map>


class CoreEngine {
public:
	CoreEngine();
	~CoreEngine();

	
	void GameLoop();
	// Add the system into the map
	void AddSystem(std::string SystemName, ISystems* sys);
	void BroadcastMessage(Message *msg);
	void Initialize();

private:
	std::unordered_map<std::string, ISystems*> Systems;
	unsigned last_update;
	bool game_active;
};

class MessageQuit : public Message {
public:
	MessageQuit() : Message(MessageID::Quit) {};
};

extern CoreEngine* Core;