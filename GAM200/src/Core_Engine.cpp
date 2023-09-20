#include <Debug.h>
#include "Core_Engine.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include "input.h"
#include <map>

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
void DebugUpdate(ISystems* s, const float& dt, std::map<std::string, double>& elapsed_time, double& total_time) {
	unsigned start_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	s->Update(dt);
	unsigned end_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	elapsed_time[s->SystemName()] = (double)(end_system_time - start_system_time) / 1000000.0;
	total_time += (double)(end_system_time - start_system_time) / 1000000.0;
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

		// DEBUG: Calculate the time it takes for each system to complete it's update
		std::map<std::string, double> elapsed_time;
		double total_time = 0.0;

		for (const std::pair<std::string, ISystems*>& s : Systems) {
			if (s.first != "Window" && s.first != "Graphics") // These 2 systems need to be updated last after all other systems are done
				if (log_system_time)
					DebugUpdate(s.second, dt, elapsed_time, total_time); // DEBUG: To log how long does each system needs to finish updating
				else
					s.second->Update(dt); // The non-debugging version
		}

		if (log_system_time) {
			DebugUpdate(Systems["Graphics"], dt, elapsed_time, total_time);
			DebugUpdate(Systems["Window"], dt, elapsed_time, total_time);

			// Output to console for now, will plan to display ingame when the engine can render fonts

			//std::cout << s->SystemName() << " system completed it's update in " << std::fixed << std::setprecision(6) << (double)(end_system_time - start_system_time) / 1000000.0 << " seconds (" << p.second / total_time * 100.0 << "%)" << std::endl;

			for (std::pair<std::string, double> p : elapsed_time)
				std::cout << p.first << " system completed it's update in " << std::fixed << std::setprecision(6) << p.second << " seconds (" << p.second / total_time * 100.0 << "%)" << std::endl;

			std::cout << "Total time taken for this frame: " << std::fixed << std::setprecision(6) << total_time << " seconds." << std::endl;
			std::cout << "########################################################################################" << std::endl;
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

void CoreEngine::Broadcast(Message* msg) {
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