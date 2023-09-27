#include <Precompile.h>
#include<GLWindow.h>
#include <Debug.h>
#include "Core_Engine.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <input.h>
#include <map>
#include <thread>

CoreEngine* CORE = NULL;

// DEBUG: To log how long does each system needs to finish updating
void DebugUpdate(ISystems* sys, const float& dt, std::map<std::string, double>& elapsed_time, double& total_time) {
	long long start_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	sys->Update(dt);
	//Printing System name for debugging purposes
	std::cout << sys->SystemName() << " is updating" << std::endl;
	long long end_system_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	elapsed_time[sys->SystemName()] = (double)(end_system_time - start_system_time) / 1000000.0;
	total_time += (double)(end_system_time - start_system_time) / 1000000.0;
}

CoreEngine::CoreEngine() {
	last_update = 0;
	game_active = true;
	CORE = this;
}

CoreEngine::~CoreEngine() {
	DeleteSystem();
}

void CoreEngine::Initialize() {
	std::cout << "Initialising " << Systems["Window"]->SystemName() << std::endl;
	Systems["Window"]->Initialize(); // Must initialize Window first
	for (const std::pair<std::string, ISystems*>& sys : Systems) { // Then initialize all other systems
		if (sys.first != "Window") { // Window already initialized, do not do it again
			// printing system name for debugging purposes
			std::cout << "Initialising " << sys.second->SystemName() << std::endl;
			sys.second->Initialize();
		}
	}
}



void CoreEngine::GameLoop() {
	bool log_system_time = false;
	int fps_set = 60;
	auto invFpsLimit = std::chrono::round<std::chrono::system_clock::duration>(std::chrono::duration<double>{ 1. / fps_set }); // 1/60
	auto m_BeginFrame = std::chrono::system_clock::now();
	auto m_EndFrame = m_BeginFrame + invFpsLimit;
	unsigned frame_count_per_second = 0;
	auto prev_time_in_seconds = std::chrono::time_point_cast<std::chrono::seconds>(m_BeginFrame);
	float dt = 1.f / fps_set;
	std::cout << "########################################################" << std::endl;
	std::cout << "Press F to print out frametime performance information" << std::endl;
	std::cout << "for the current frame" << std::endl;
	//std::cout << "game window is the active window first and then press P" << std::endl;
	std::cout << "########################################################" << std::endl;

	while (game_active) {
		
		// Toggle F key to enable performance viewer
		if (input::IsPressed(KEY::f)) {
			log_system_time = !log_system_time;
			//std::cout << "Performance viewer is now " << (log_system_time ? "ON" : "OFF. Press P to switch it on again\n(make sure the game window is the active window first)") << std::endl;
		}
		/*
		//Get the current time from chrono in milliseconds
		auto now = std::chrono::system_clock::now();
		auto duration = now.time_since_epoch();
		long long current_time = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

		
		//Convert it to the time passed since the last frame (in seconds)
		const float FPS = 60;
		const float frame_time = 1.0f / FPS;
		
		float dt = static_cast<float>(current_time - last_update) / 1000.f;
		
		if (frame_time > dt) {
			long long time_left = frame_time - dt;
			std::this_thread::sleep_for(std::chrono::milliseconds(time_left));
		}

		//Update the when the last update started
		last_update = current_time;
		*/
		// DEBUG: Calculate the time it takes for each system to complete it'sys update
		std::map<std::string, double> elapsed_time;
		double total_time = 0.0;

		for (const std::pair<std::string, ISystems*>& sys : Systems) {
			if (sys.first != "Window" && sys.first != "Graphics") // These 2 systems need to be updated last after all other systems are done for the most up-to-date info
				if (log_system_time) {
					DebugUpdate(sys.second, dt, elapsed_time, total_time); // DEBUG: To log how long does each system needs to finish updating
				}
				else {
					sys.second->Update(dt); // The non-debugging version
				}
		}

		if (log_system_time) {
			DebugUpdate(Systems["Window"], dt, elapsed_time, total_time);
			DebugUpdate(Systems["Graphics"], dt, elapsed_time, total_time);
			
			// Output to console for now, will plan to display ingame when the engine can render fonts
			for (std::pair<std::string, double> p : elapsed_time)
				std::cout << p.first << " system completed it's update in " << std::fixed << std::setprecision(6) << p.second << " seconds (" << p.second / total_time * 100.0 << "%)" << std::endl;

			std::cout << "Total time taken for this frame: " << std::fixed << std::setprecision(6) << total_time << " seconds." << std::endl;
			std::cout << "########################################################################################" << std::endl;
			log_system_time = false;
		}
		else {
			Systems["Window"]->Update(dt);
			Systems["Graphics"]->Update(dt);

		}
		auto time_in_seconds = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());
		++frame_count_per_second;

		// This part keeps the frame rate.
		std::this_thread::sleep_until(m_EndFrame);
		if (time_in_seconds > prev_time_in_seconds)
		{
			window->fps = frame_count_per_second;
			dt = 1.f / frame_count_per_second;
			//std::cout << frame_count_per_second << " frames per second\n";				// For Frame Rate Per Second Display
			frame_count_per_second = 0;
			prev_time_in_seconds = time_in_seconds;
		}
		
		m_BeginFrame = m_EndFrame;
		m_EndFrame = m_BeginFrame + invFpsLimit;
	}
}
void CoreEngine::AddSystem(ISystems* sys) {
	Systems[sys->SystemName()] = sys;
}

void CoreEngine::DeleteSystem() {
	for (const std::pair<std::string, ISystems*>& sys : Systems)
		delete sys.second;
}
void CoreEngine::Broadcast(Message* msg) {
	// Set Game_mode to 0 to stop loop
	if (msg->messageId == MessageID::Quit) {
		game_active = false;
	}
	// Loop Messaging System
	for (const std::pair<std::string, ISystems*>& sys : Systems) {
		sys.second->MessageRelay(msg);
	}
}

