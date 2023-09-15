#pragma once


// Core Engine that manage all systems in game. Update and destroying them.
#include "Interface_System.h"
#include <vector>  // Remove after adding this header somewhere else


class CoreEngine {
public:
	CoreEngine();
	~CoreEngine();

	void GameLoop();
	void AddSystem(ISystems* sys);
	void DestroySystem();
	//void BroadcastMessage(Message *m);
	void Initialize();

private:
	std::vector<ISystems*> Systems;
	unsigned last_update;
	bool game_active;
};


extern CoreEngine* Core;