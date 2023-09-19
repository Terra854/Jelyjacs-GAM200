#include "Core_Engine.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <map>

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
		
		// Calculate the time it takes for each system to complete it's update
		std::map<std::string, double> elapsed_time;
		double total_time = 0.0;
		
		for (int i = 0; i < Systems.size(); i++) {
			unsigned start_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			Systems[i]->Update(dt);
			unsigned end_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			//std::cout << Systems[i]->GetSystemName() << " system completed it's update in " << std::fixed << std::setprecision(6) << (double)(end_system_time - start_system_time) / 1000000.0 << " seconds" << std::endl;
			elapsed_time[Systems[i]->GetSystemName()] = (double)(end_system_time - start_system_time) / 1000000.0;
			total_time += (double)(end_system_time - start_system_time) / 1000000.0;
		}

		// Output to console for now, will plan to display ingame when the engine can render fonts
		for (const std::pair<std::string, double> p : elapsed_time) {
			std::cout << p.first << " system completed it's update in " << std::fixed << std::setprecision(6) << p.second << " seconds (" << p.second / total_time * 100.0 << "%)" << std::endl;
		}
		std::cout << "Total time taken for this frame: " << std::fixed << std::setprecision(6) << total_time << " seconds." << std::endl;

		total_time = 0.0;
		std::cout << "########################################################################################" << std::endl;
	}
}
void CoreEngine::AddSystem(ISystems* sys) {
	Systems.push_back(sys);
}

void CoreEngine::DestroySystem() {
	for (unsigned i = 0; i < Systems.size(); ++i)
	{
		delete Systems[Systems.size() - i - 1];
	}
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
	for (unsigned i = 0; i < Systems.size(); ++i)
		Systems[i]->Initialize();
}