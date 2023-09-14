

#include "Core_Engine.h"
#include <chrono>


CoreEngine* CORE;
CoreEngine::CoreEngine() {
	last_update = 0;
	game_active = true;
	CORE = this;
}

CoreEngine::~CoreEngine(){}

void CoreEngine::GameLoop() {
	//Get the current time from chrono in milliseconds
	auto now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	unsigned current_time = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

	//Convert it to the time passed since the last frame (in seconds)
	float dt = (current_time - last_update) / 1000.0f;

	//Update the when the last update started
	last_update = current_time;

	while (game_active) {
		for (int i = 0; i < Systems.size(); i++) {
			Systems[i]->Update(dt);
		}
	}
}
void CoreEngine::AddSystem(System_Class* sys) {
	Systems.push_back(sys);
}

void CoreEngine::DestroySystem() {
	for (unsigned i = 0; i < Systems.size(); ++i)
	{
		delete Systems[Systems.size() - i - 1];
	}
}

void CoreEngine::Initialize() {
	for (unsigned i = 0; i < Systems.size(); ++i)
		Systems[i]->Initialize();
}