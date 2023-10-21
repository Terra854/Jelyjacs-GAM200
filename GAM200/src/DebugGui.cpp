#include <DebugGui.h>
#include <ImGui/imgui.h>
#include <Collision.h>
#include <Core_Engine.h>

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

void DebugGui::Update(){
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Window"))
        {
            if (ImGui::MenuItem("Performance Viewer")) { engine->displayPerformanceInfo = true; }
			if (ImGui::MenuItem("Uniform Grid")) { showUniformGrid = true; }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

	showUniformGrid ? DebugUniformGrid() : DoNothing();
}
