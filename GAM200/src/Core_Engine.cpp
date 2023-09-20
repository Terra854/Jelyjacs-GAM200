#include <Debug.h>
#include "Core_Engine.h"
#include <chrono>
#include <iostream>
#include <iomanip>

CoreEngine* CORE;
CoreEngine::CoreEngine() {
	last_update = 0;
	game_active = true;
	CORE = this;
}

CoreEngine::~CoreEngine() {
	for (const std::pair<std::string, ISystems*>& s : Systems)
		delete s.second;
}

// DEBUG: To log how long does each system needs to finish updating
void DebugUpdate(ISystems* s, const float& dt, double& total_time) {
	unsigned start_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	s->Update(dt);
	unsigned end_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	std::cout << s->GetSystemName() << " system completed it's update in " << std::fixed << std::setprecision(6) << (double)(end_system_time - start_system_time) / 1000000.0 << " seconds" << std::endl;
	total_time += (double)(end_system_time - start_system_time) / 1000000.0;
}

void CoreEngine::GameLoop() {
	while (game_active) {
		//Get the current time from chrono in milliseconds
		auto now = std::chrono::system_clock::now();
		auto duration = now.time_since_epoch();
		unsigned current_time = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

		//Convert it to the time passed since the last frame (in seconds)
		float dt = (last_update) ? (float)(current_time - last_update) / 1000.0f : 0.f;

		//Update the when the last update started
		last_update = current_time;

		// DEBUG: Calculate the time it takes for each system to complete it's update
		double total_time = 0.0;

		for (const std::pair<std::string, ISystems*>& s : Systems) {
			if (s.first != "Window" && s.first != "Graphics") // These 2 systems need to be updated last after all other systems are done
				DebugUpdate(s.second, dt, total_time); // DEBUG: To log how long does each system needs to finish updating
				//s.second->Update(dt); // The non-debugging version
		}

		DebugUpdate(Systems["Graphics"], dt, total_time);
		DebugUpdate(Systems["Window"], dt, total_time);

		// Output to console for now, will plan to display ingame when the engine can render fonts
		std::cout << "Total time taken for this frame: " << std::fixed << std::setprecision(6) << total_time << " seconds." << std::endl;
		std::cout << "########################################################################################" << std::endl;

		total_time = 0.0;
	}
}
void CoreEngine::AddSystem(std::string SystemName, ISystems* sys) {
	Systems[SystemName] = sys;
}

void CoreEngine::BroadcastMessage(Message* msg) {
	if (msg->messageId == MessageID::Quit) {
		game_active = false;
	}
	/*
	for (unsigned i = 0; i < Systems.size(); i++) {
		Systems[i]->SendMessage(msg);
	}
	*/
}

void CoreEngine::Initialize() {
	Systems["Window"]->Initialize(); // Must initialize Window first

	for (const std::pair<std::string, ISystems*>& s : Systems) // Then initialize all other systems
		if (s.first != "Window") // Window already initialized, do not do it again
			s.second->Initialize();
}