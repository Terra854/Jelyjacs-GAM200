#include <DebugGui.h>
#include <ImGui/imgui.h>
#include <Collision.h>
#include <Core_Engine.h>

DebugGui* debug_gui = nullptr; // declared in DebugGui.cpp
bool showUniformGrid = false;
bool showPerformanceInfo = false;

void DebugGui::DebugUniformGrid() {
	// DEBUG: Print out the uniform grid
	ImGui::SetNextWindowSize(ImVec2(0, 0));
	ImGui::SetNextWindowPos(ImVec2(900, 100), ImGuiCond_Once);
	ImGui::Begin("DEBUG: Uniform Grid", &showUniformGrid);

	char addString[512];
	char buffer[2048];

	// Print separator
	buffer[0] = '\0';
	for (int i = 0; i < Collision::uniform_grid.size() + 1; ++i) {
		ImGui::SameLine(0, 0);
		ImGui::Text("--------");
	}

	// Print rows
	for (int y = static_cast<int>(Collision::uniform_grid[0].size() - 1); y >= 0; --y) {
		ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "%4d   ", y);
		for (int x = 0; x < Collision::uniform_grid.size(); ++x) {
			ImGui::SameLine(0, 0);
			ImGui::Text("|%4zu   ", Collision::uniform_grid[x][y].size());  // %zu format specifier is for size_t
		}

		ImGui::SameLine(0, 0);
		ImGui::Text("|");

		// Print separator
		for (int i = 0; i < Collision::uniform_grid.size() + 1; ++i) {
			ImGui::Text("--------");
			ImGui::SameLine(0, 0);
		}
		ImGui::Text("");
	}

	// Print the column header below
	ImGui::Text("       ");
	ImGui::SameLine(0, 0);
	for (int i = 0; i < Collision::uniform_grid.size(); ++i) {
		ImGui::Text("|");
		ImGui::SameLine(0, 0);
		ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "%4d   ", i);
		ImGui::SameLine(0, 0);
	}
	ImGui::Text("|");

	ImGui::End();
}

void DebugGui::DebugPerformanceViewer() {
	ImGui::SetNextWindowSize(ImVec2(0, 0));
	ImGui::SetNextWindowPos(ImVec2(0, 30), ImGuiCond_Once);
	ImGui::Begin("DEBUG: Performance Viewer", &showPerformanceInfo);

	for (std::pair<std::string, double> p : System_elapsed_time)
		ImGui::Text("%s system completed it's update in %.6f seconds (%.2f%%)", p.first.c_str(), p.second, (p.second / total_time * 100.0));

	ImGui::Text("############################################################");
	ImGui::Text("Total time taken for this frame: %.6f seconds.", total_time);
	float fps = engine->Get_FPS();
	ImGui::Text("Frame Rate is: %.6f FPS", fps);

	ImGui::End();
}

void DoNothing() {

}

void DebugGui::Initialize() {
	total_time = 0.0;
}

void DebugGui::Update(){
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
			if (ImGui::MenuItem("Exit")) { engine->game_active = false; }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Window"))
        {
            if (ImGui::MenuItem("Performance Viewer")) { showPerformanceInfo = true;}
			if (ImGui::MenuItem("Uniform Grid")) { showUniformGrid = true; }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

	showUniformGrid ? DebugUniformGrid() : DoNothing();
	showPerformanceInfo ? DebugPerformanceViewer() : DoNothing();
}
