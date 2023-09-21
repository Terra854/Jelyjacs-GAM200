#include <Debug.h>
#include "Core_Engine.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include "input.h"
#include <map>

CoreEngine* CORE = NULL;

// DEBUG: To log how long does each system needs to finish updating
void DebugUpdate(ISystems* sys, const float& dt, std::map<std::string, double>& elapsed_time, double& total_time) {
	unsigned start_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	sys->Update(dt);
	unsigned end_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	elapsed_time[sys->SystemName()] = (double)(end_system_time - start_system_time) / 1000000.0;
	total_time += (double)(end_system_time - start_system_time) / 1000000.0;
}

CoreEngine::CoreEngine() {
	last_update = 0;
	game_active = true;
	CORE = this;
}

CoreEngine::~CoreEngine() {
	//for (const std::pair<std::string, ISystems*>& sys : Systems)
		//delete sys.second;
}

void CoreEngine::Initialize() {
	Systems["Window"]->Initialize(); // Must initialize Window first

	for (const std::pair<std::string, ISystems*>& sys : Systems) // Then initialize all other systems
		if (sys.first != "Window") // Window already initialized, do not do it again
			sys.second->Initialize();
}



void CoreEngine::GameLoop() {

	bool log_system_time = false;
	std::cout << "########################################################" << std::endl;
	std::cout << "To switch on/off the performance viewer, ensure that the" << std::endl;
	std::cout << "game window is the active window first and then press P" << std::endl;
	std::cout << "########################################################" << std::endl;

	while (game_active) {
		// Toggle P key to enable performance viewer
		if (input::IsPressed(KEY::p)) {
			log_system_time = !log_system_time;
			std::cout << "Performance viewer is now " << (log_system_time ? "ON" : "OFF. Press P to switch it on again\n(make sure the game window is the active window first)") << std::endl;
		}

		//Get the current time from chrono in milliseconds
		auto now = std::chrono::system_clock::now();
		auto duration = now.time_since_epoch();
		unsigned current_time = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

		//Convert it to the time passed since the last frame (in seconds)
		float dt = (last_update) ? (float)(current_time - last_update) / 1000.0f : 0.f;

		//Update the when the last update started
		last_update = current_time;

		// DEBUG: Calculate the time it takes for each system to complete it'sys update
		std::map<std::string, double> elapsed_time;
		double total_time = 0.0;

		for (const std::pair<std::string, ISystems*>& sys : Systems) {
			if (sys.first != "Window" && sys.first != "Graphics") // These 2 systems need to be updated last after all other systems are done for the most up-to-date info
				if (log_system_time)
					DebugUpdate(sys.second, dt, elapsed_time, total_time); // DEBUG: To log how long does each system needs to finish updating
				else
					sys.second->Update(dt); // The non-debugging version
		}

		if (log_system_time) {
			DebugUpdate(Systems["Graphics"], dt, elapsed_time, total_time);
			DebugUpdate(Systems["Window"], dt, elapsed_time, total_time);

			// Output to console for now, will plan to display ingame when the engine can render fonts
			for (std::pair<std::string, double> p : elapsed_time)
				std::cout << p.first << " system completed it'sys update in " << std::fixed << std::setprecision(6) << p.second << " seconds (" << p.second / total_time * 100.0 << "%)" << std::endl;

			std::cout << "Total time taken for this frame: " << std::fixed << std::setprecision(6) << total_time << " seconds." << std::endl;
			std::cout << "########################################################################################" << std::endl;
			log_system_time = false;
		}
		else {
			Systems["Graphics"]->Update(dt);
			Systems["Window"]->Update(dt);
		}
	}
}
void CoreEngine::AddSystem(std::string SystemName, ISystems* sys) {
	Systems[SystemName] = sys;
}

void CoreEngine::DeleteSystem() {
	for (const std::pair<std::string, ISystems*>& sys : Systems)
		delete sys.second;
}
void CoreEngine::Broadcast(Message* msg) {
	if (msg->messageId == MessageID::Quit) {
		game_active = false;
	}
	
	for (unsigned i = 0; i < Systems.size(); i++) {
		Systems[i]->SendMessage(msg);
	}
	
}

