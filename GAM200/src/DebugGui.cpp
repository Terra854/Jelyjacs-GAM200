#include <DebugGui.h>
#include <ImGui/imgui.h>
#include <Collision.h>
#include <Core_Engine.h>

DebugGui* debug_gui = nullptr; // declared in DebugGui.cpp
bool showUniformGrid = false;

void DebugUniformGrid() {
	// DEBUG: Print out the uniform grid
	ImGui::SetNextWindowSize(ImVec2(0, 0));
	ImGui::SetNextWindowPos(ImVec2(900, 100), ImGuiCond_Once);
	ImGui::Begin("DEBUG: Uniform Grid", &showUniformGrid);

	char addString[512];
	char buffer[2048];

	// Print separator
	buffer[0] = '\0';
	for (int i = 0; i < Collision::uniform_grid.size() + 1; ++i) {
		strcat_s(buffer, "--------");
	}
	///ImGui::Text("----------------------------------------------------------------------------------------------------\n");
	ImGui::Text(buffer);

	// Print rows
	for (int y = Collision::uniform_grid[0].size() - 1; y >= 0; --y) {
		sprintf_s(buffer, "%4d   ", y);
		for (int x = 0; x < Collision::uniform_grid.size(); ++x) {
			sprintf_s(addString, "|%4zu   ", Collision::uniform_grid[x][y].size());
			strcat_s(buffer, addString);
			//ImGui::Text("| %4zu ", uniform_grid[x][y].size());  // %zu format specifier is for size_t
		}
		strcat_s(buffer, "|");

		ImGui::Text(buffer);

		// Print separator
		buffer[0] = '\0';
		for (int i = 0; i < Collision::uniform_grid.size() + 1; ++i) {
			strcat_s(buffer, "--------");
		}
		ImGui::Text(buffer);
	}

	// Print the column header below
	sprintf_s(buffer, "       ");
	for (int i = 0; i < Collision::uniform_grid.size(); ++i) {
		sprintf_s(addString, "|%4d   ", i);
		strcat_s(buffer, addString);
	}
	strcat_s(buffer, "|");

	ImGui::Text(buffer);

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
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Window"))
        {
            if (ImGui::MenuItem("Performance Viewer")) {
				engine->displayPerformanceInfo = true;
				
				
				
			}
			if (ImGui::MenuItem("Uniform Grid")) { showUniformGrid = true; }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

	if (engine->displayPerformanceInfo) {
		ImGui::SetNextWindowSize(ImVec2(0, 0));
		ImGui::SetNextWindowPos(ImVec2(0, 30), ImGuiCond_Once);
		ImGui::Begin("DEBUG: Performance Viewer");

		for (std::pair<std::string, double> p : System_elapsed_time)
			ImGui::Text("%s system completed it's update in %.6f seconds (%.2f%%)", p.first.c_str(), p.second, (p.second / total_time * 100.0));

		ImGui::Text("############################################################");
		ImGui::Text("Total time taken for this frame: %.6f seconds.", total_time);
		float fps = engine->Get_FPS();
		ImGui::Text("Frame Rate is: %.6f FPS", fps);

		ImGui::End();
	}


	showUniformGrid ? DebugUniformGrid() : DoNothing();
}
