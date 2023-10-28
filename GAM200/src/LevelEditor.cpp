#include <LevelEditor.h>
#include <Debug.h>
#include <ImGui/imgui.h>
#include <Collision.h>
#include <Core_Engine.h>
#include <Factory.h>
#include <components/Texture.h>
#include <components/Animation.h>

LevelEditor* level_editor = nullptr; // declared in LevelEditor.cpp
bool showUniformGrid = false;
bool showPerformanceInfo = false;

bool dock_space = true; // Always must be on for level editor

void LevelEditor::DebugUniformGrid() {
	// DEBUG: Print out the uniform grid
	ImGui::SetNextWindowSize(ImVec2(0, 0));
	ImGui::SetNextWindowPos(ImVec2(900, 100), ImGuiCond_Once);
	ImGui::Begin("DEBUG: Uniform Grid", &showUniformGrid);

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

void LevelEditor::DebugPerformanceViewer() {
	ImGui::SetNextWindowSize(ImVec2(0, 0));
	ImGui::SetNextWindowPos(ImVec2(0, 30), ImGuiCond_Once);
	ImGui::Begin("DEBUG: Performance Viewer", &showPerformanceInfo);

	for (std::pair<std::string, double> p : System_elapsed_time) {
		ImGui::Text("%s system completed it's update in %.6f seconds", p.first.c_str(), p.second);
		ImGui::ProgressBar(p.second / total_time, ImVec2(0.0f, 0.0f), "");
		ImGui::SameLine();
		ImGui::Text("%.2f%%", p.second / total_time * 100.0);
	}
	ImGui::Text("############################################################");
	ImGui::Text("Total time taken for this frame: %.6f seconds.", total_time);
	float fps = engine->Get_FPS();
	ImGui::Text("Frame Rate is: %.6f FPS", fps);

	ImGui::End();
}

int selected = -1;

void LevelEditor::ObjectProperties(){

	ImGui::SetNextWindowSize(ImVec2(450, 0));
	char buffer[100];

	ImGui::Begin("Object Properties");

	if (selected == -1) {
		ImGui::BeginChild("Texture", ImVec2(ImGui::GetContentRegionAvail().x * 0.25f, ImGui::GetContentRegionAvail().x * 0.25f));
		ImGui::Image(NULL, ImGui::GetContentRegionAvail());
		ImGui::EndChild();
		ImGui::End();

		return;
	}

	Object* object = objectFactory->getObjectWithID(selected);

	Transform* tr = (Transform*)object->GetComponent(ComponentType::Transform);
	Texture* te = (Texture*)object->GetComponent(ComponentType::Texture);
	Body* b = (Body*)object->GetComponent(ComponentType::Body);
	Physics* ph = (Physics*)object->GetComponent(ComponentType::Physics);
	PlayerControllable* pc = (PlayerControllable*)object->GetComponent(ComponentType::PlayerControllable);
	Animation* a = (Animation*)object->GetComponent(ComponentType::Animation);

	ImGui::BeginChild("Texture", ImVec2(ImGui::GetContentRegionAvail().x * 0.25f, ImGui::GetContentRegionAvail().x * 0.25f));

	if (a != nullptr){
		
		/* TODO: Idk where the textures for the animations are stored */

		unsigned int anim = a->animation_Map[a->current_type][a->frame_num].texobj;

		if (tr->Scale.x > tr->Scale.y) {
			float padding = ImGui::GetContentRegionAvail().y * (tr->Scale.y / tr->Scale.x) * 0.5f;
			ImGui::Dummy(ImVec2(0, padding));
			ImGui::Image((void*)(intptr_t)anim, ImVec2(ImGui::GetContentRegionAvail().x, tr->Scale.y / tr->Scale.x * ImGui::GetContentRegionAvail().y));
		}
		else if (tr->Scale.x == tr->Scale.y)
			ImGui::Image((void*)(intptr_t)anim, ImGui::GetContentRegionAvail());
		else {
			float padding = ImGui::GetContentRegionAvail().x * (tr->Scale.x / tr->Scale.y) * 0.5f;
			ImGui::Dummy(ImVec2(padding, 0));
			ImGui::SameLine();
			ImGui::Image((void*)(intptr_t)anim, ImVec2(tr->Scale.x / tr->Scale.y * ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
		}

		ImGui::SetCursorPos(ImVec2(0, 0));
		ImGui::Text("There is");
		ImGui::Text("animation");
		ImGui::Text("idk how to");
		ImGui::Text("draw it here");
	}
	else if (te != nullptr) {
		if (tr->Scale.x > tr->Scale.y) {
			float padding = ImGui::GetContentRegionAvail().y * (tr->Scale.y / tr->Scale.x) * 0.5f;
			ImGui::Dummy(ImVec2(0, padding));
			ImGui::Image((void*)(intptr_t)te->texturepath, ImVec2(ImGui::GetContentRegionAvail().x, tr->Scale.y / tr->Scale.x * ImGui::GetContentRegionAvail().y));
		}
		else if (tr->Scale.x == tr->Scale.y)
			ImGui::Image((void*)(intptr_t)te->texturepath, ImGui::GetContentRegionAvail());
		else {
			float padding = ImGui::GetContentRegionAvail().x * (tr->Scale.x / tr->Scale.y) * 0.5f;
			ImGui::Dummy(ImVec2(padding, 0));
			ImGui::SameLine();
			ImGui::Image((void*)(intptr_t)te->texturepath, ImVec2(tr->Scale.x / tr->Scale.y * ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
		}
	}
	else {
		ImGui::Text("This object has");
		ImGui::Text("no texture or");
		ImGui::Text("animations");
	}
	ImGui::EndChild();

	ImGui::SameLine();

	//ImGui::BeginChild("ID", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, ImGui::GetContentRegionAvail().x * 0.5f));
	ImGui::BeginChild("ID", ImVec2(ImGui::GetContentRegionAvail().x * 0.75f, ImGui::GetContentRegionAvail().x * 0.25f));

	ImGui::Text("Object ID: %d", object->GetId());
	ImGui::Text("Object Name: %s", object->GetName().c_str());
	ImGui::Text("Number of components: %d", object->GetNumComponents());
	
	ImGui::EndChild();

	if (te != nullptr) {
		if (ImGui::CollapsingHeader("Texture")) {
			ImGui::Text("Nothing right now");
		}
	}

	if (a != nullptr) {
		if (ImGui::CollapsingHeader("Animation")) {
			ImGui::Text("Nothing right now");
		}
	}

	if (tr != nullptr) {
		if (ImGui::CollapsingHeader("Transform")) {
			ImGui::Text("Position: %.5f, %.5f", tr->Position.x, tr->Position.y);
			ImGui::Text("Rotation: %.5f", tr->Rotation);
			ImGui::Text("Scale: %.5f, %.5f", tr->Scale.x, tr->Scale.y);
		}
	}

	if (b != nullptr) {
		if (ImGui::CollapsingHeader("Body")) {
			if (b->GetShape() == Shape::Rectangle) {
				Rectangular* r = (Rectangular*)b;
				ImGui::Text("AABB Width: %.5f", r->width);
				ImGui::Text("AABB Height: %.5f", r->height);

				ImGui::SeparatorText("AABB Collision");

				/*** ROW 1 ***/

				/* P3 */

				ImGui::BeginChild("P3", ImVec2(ImGui::GetContentRegionAvail().x * 0.333333f, 26.f));

				// Get the child window's size
				ImVec2 childSize = ImGui::GetWindowSize();

				// Calculate the text's size
				sprintf_s(buffer, "P3: x: %.5f", r->aabb.P3().x);
				std::string text = buffer;
				ImVec2 textSizeX = ImGui::CalcTextSize(text.c_str());

				// Set the cursor position to bottom right minus the text size
				ImVec2 textPosX = ImVec2(childSize.x - textSizeX.x, 0);
				ImGui::SetCursorPos(textPosX);

				// Render the text
				ImGui::Text("%s", text.c_str());

				// Calculate the text's size
				sprintf_s(buffer, "y: %.5f", r->aabb.P3().y);
				text = buffer;
				ImVec2 textSizeY = ImGui::CalcTextSize(text.c_str());

				// Set the cursor position
				ImVec2 textPosY = ImVec2(childSize.x - textSizeY.x, textSizeX.y);
				ImGui::SetCursorPos(textPosY);

				// Render the text
				ImGui::Text("%s", text.c_str());

				ImGui::EndChild();

				ImGui::SameLine();

				/* TOP COLLISION */

				ImGui::BeginChild("CollisionTop", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 26.f));

				// Calculate the text's size
				sprintf_s(buffer, "Top");
				text = buffer;
				textSizeX = ImGui::CalcTextSize(text.c_str());

				// Set the cursor position
				textPosX = ImVec2((childSize.x - textSizeX.x) * 0.5f, textSizeX.y);
				ImGui::SetCursorPos(textPosX);

				// Render the text
				if (r->top_collision != nullptr)
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

				ImGui::Text("%s", text.c_str());
				
				if (r->top_collision != nullptr)
					ImGui::PopStyleColor();
				
				ImGui::EndChild();

				ImGui::SameLine();

				/* P2 */

				ImGui::BeginChild("P2", ImVec2(0.f, 26.f));

				// Calculate the text's size
				sprintf_s(buffer, "P2: x: %.5f", r->aabb.P2().x);
				text = buffer;
				textSizeX = ImGui::CalcTextSize(text.c_str());

				// Set the cursor position
				textPosX = ImVec2(0, 0);
				ImGui::SetCursorPos(textPosX);

				// Render the text
				ImGui::Text("%s", text.c_str());

				// Calculate the text's size
				sprintf_s(buffer, "    y: %.5f", r->aabb.P2().y);
				text = buffer;
				textSizeY = ImGui::CalcTextSize(text.c_str());

				// Set the cursor position to bottom right minus the text size
				textPosY = ImVec2(0, textSizeX.y);
				ImGui::SetCursorPos(textPosY);

				// Render the text
				ImGui::Text("%s", text.c_str());

				ImGui::EndChild();

				/*** ROW 2 ***/

				ImVec2 square = ImVec2(ImGui::GetContentRegionAvail().x * 0.333333f, ImGui::GetContentRegionAvail().x * 0.333333f);

				/* LEFT COLLISION */

				ImGui::BeginChild("CollisionLeft", square);

				// Calculate the text's size
				sprintf_s(buffer, "Left");
				text = buffer;
				textSizeX = ImGui::CalcTextSize(text.c_str());

				// Set the cursor position
				textPosX = ImVec2(square.x - textSizeX.x, (square.y - textSizeX.y) * 0.5f);
				ImGui::SetCursorPos(textPosX);

				// Render the text
				if (r->left_collision != nullptr)
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

				ImGui::Text("%s", text.c_str());

				if (r->left_collision != nullptr)
					ImGui::PopStyleColor();

				ImGui::EndChild();

				ImGui::SameLine();

				/* SQUARE */

				ImGui::BeginChild("SquareVis", square);
				ImDrawList* draw_list = ImGui::GetWindowDrawList();

				ImVec2 p0 = ImGui::GetCursorScreenPos(); // Current cursor position as the top-left
				ImVec2 p1 = ImVec2(p0.x + square.x, p0.y + square.y); // Scale square to child size

				// Draw the square
				draw_list->AddRectFilled(p0, p1, IM_COL32(255, 255, 255, 255));
				ImGui::EndChild();

				ImGui::SameLine();

				/* RIGHT COLLISION */

				ImGui::BeginChild("CollisionRight", square);

				// Calculate the text's size
				sprintf_s(buffer, "Right");
				text = buffer;
				textSizeX = ImGui::CalcTextSize(text.c_str());

				// Set the cursor position
				textPosX = ImVec2(0, (square.y - textSizeX.y) * 0.5f);
				ImGui::SetCursorPos(textPosX);

				// Render the text
				if (r->right_collision != nullptr)
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
				
				ImGui::Text("%s", text.c_str());

				if (r->right_collision != nullptr)
					ImGui::PopStyleColor();

				ImGui::EndChild();

				/*** ROW 3 ***/

				/* P0 */

				ImGui::BeginChild("P0", ImVec2(ImGui::GetContentRegionAvail().x * 0.333333f, 26.f));

				// Calculate the text's size
				sprintf_s(buffer, "P0: x: %.5f", r->aabb.P0().x);
				text = buffer;
				textSizeX = ImGui::CalcTextSize(text.c_str());

				// Set the cursor position to bottom right minus the text size
				textPosX = ImVec2(childSize.x - textSizeX.x, 0);
				ImGui::SetCursorPos(textPosX);

				// Render the text
				ImGui::Text("%s", text.c_str());

				// Calculate the text's size
				sprintf_s(buffer, "y: %.5f", r->aabb.P0().y);
				text = buffer;
				textSizeY = ImGui::CalcTextSize(text.c_str());

				// Set the cursor position
				textPosY = ImVec2(childSize.x - textSizeY.x, textSizeX.y);
				ImGui::SetCursorPos(textPosY);

				// Render the text
				ImGui::Text("%s", text.c_str());

				ImGui::EndChild();

				ImGui::SameLine();

				/* BOTTOM COLLISION */

				ImGui::BeginChild("CollisionBottom", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 26.f));

				// Calculate the text's size
				sprintf_s(buffer, "Bottom");
				text = buffer;
				textSizeX = ImGui::CalcTextSize(text.c_str());

				// Set the cursor position
				textPosX = ImVec2((childSize.x - textSizeX.x) * 0.5f, 0);
				ImGui::SetCursorPos(textPosX);

				// Render the text
				if (r->bottom_collision != nullptr)
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

				ImGui::Text("%s", text.c_str());

				if (r->bottom_collision != nullptr)
					ImGui::PopStyleColor();

				ImGui::EndChild();

				ImGui::SameLine();

				/* P1 */

				ImGui::BeginChild("P1", ImVec2(0.f, 26.f));

				// Calculate the text's size
				sprintf_s(buffer, "P1: x: %.5f", r->aabb.P1().x);
				text = buffer;
				textSizeX = ImGui::CalcTextSize(text.c_str());

				// Set the cursor position
				textPosX = ImVec2(0, 0);
				ImGui::SetCursorPos(textPosX);

				// Render the text
				ImGui::Text("%s", text.c_str());

				// Calculate the text's size
				sprintf_s(buffer, "    y: %.5f", r->aabb.P1().y);
				text = buffer;
				textSizeY = ImGui::CalcTextSize(text.c_str());

				// Set the cursor position to bottom right minus the text size
				textPosY = ImVec2(0, textSizeX.y);
				ImGui::SetCursorPos(textPosY);

				// Render the text
				ImGui::Text("%s", text.c_str());

				ImGui::EndChild();
			}
		}
	}

	if (ph != nullptr) {
		if (ImGui::CollapsingHeader("Physics")) {
			ImGui::Text("Velocity: %.5f, %.5f", ph->Velocity.x, ph->Velocity.y);
		}
	}

	if (pc != nullptr) {
		if (ImGui::CollapsingHeader("PlayerControllable")) {
			ImGui::Text("Nothing right now");
		}
	}


	if (ImGui::Button("Delete"))
	{
		objectFactory->destroyObject(object);
		selected = -1;
	}

	ImGui::End();
}

void LevelEditor::ListOfObjects() {

	ImGui::Begin("Object List");
	if (ImGui::BeginTable("split", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
	{
		for (size_t i = 0; i < objectFactory->NumberOfObjects(); i++)
		{

			if (objectFactory->getObjectWithID(i) == nullptr)
			{
				continue;
			}
			ImGui::TableNextColumn();
			Object* object = objectFactory->getObjectWithID(static_cast<int>(i));
			char buf[32];
			if (object->GetName().empty())
				sprintf_s(buf, "%d) Object", static_cast<int>(i));
			else
				sprintf_s(buf, "%d) %s", static_cast<int>(i), object->GetName().c_str());

			// Creating button for each object
			if (ImGui::Selectable(buf, selected == static_cast<int>(i))) {
				selected = static_cast<int>(i);
			}
		}
		ImGui::EndTable();
	}
	ImGui::End();
}

void DoNothing() {

}

void LevelEditor::Initialize() {
	total_time = 0.0;
}

void LevelEditor::Update() {

	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	
	ImGui::Begin("DockSpace Demo", &dock_space, window_flags);

	ImGui::PopStyleVar(3);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("level_editor_dockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	else
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("ERROR: Docking is not enabled! See Demo > Configuration.");
		ImGui::Text("Set io.ConfigFlags |= ImGuiConfigFlags_DockingEnable in your code, or ");
		ImGui::SameLine(0.0f, 0.0f);
		if (ImGui::SmallButton("click here"))
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	}

	ImGui::ShowDemoWindow();

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit")) { engine->game_active = false; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Performance Viewer")) { showPerformanceInfo = true; }
			if (ImGui::MenuItem("Uniform Grid")) { showUniformGrid = true; }
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	showUniformGrid ? DebugUniformGrid() : DoNothing();
	showPerformanceInfo ? DebugPerformanceViewer() : DoNothing();

	ListOfObjects();

	ObjectProperties();

}
