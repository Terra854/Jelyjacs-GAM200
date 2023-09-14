#pragma once


// Core Engine that manage all systems in game. Update and destroying them.
#include "Base_System.h"
#include <vector>  // Remove after adding this header somewhere else


class CoreEngine {
public:
	CoreEngine();
	~CoreEngine();

	void GameLoop();
	void AddSystem(System_Class* sys);
	void DestroySystem();
	//void BroadcastMessage(Message *m);
	void Initialize();

private:
	std::vector<System_Class*> Systems;
	unsigned last_update;
	bool game_active;
};


extern CoreEngine* Core;