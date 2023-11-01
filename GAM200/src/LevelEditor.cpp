#include <LevelEditor.h>
#include <Debug.h>
#include <ImGui/imgui.h>
#include <Collision.h>
#include <Core_Engine.h>
#include <Factory.h>
#include <components/Texture.h>
#include <components/Animation.h>
#include <filesystem>
#include "SceneLoader.h"
#include "Assets Manager/asset_manager.h"

LevelEditor* level_editor = nullptr; // declared in LevelEditor.cpp
bool showUniformGrid = false;
bool showPerformanceInfo = false;

bool dock_space = true; // Always must be on for level editor

LevelEditor::LevelEditor() {
	editor_grid = new LevelEditorGrid();
}

LevelEditor::~LevelEditor() {
	delete editor_grid;
}
/*
	This window is to print out the uniform grid
*/
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
		ImGui::ProgressBar((float)(p.second / total_time), ImVec2(0.0f, 0.0f), "");
		ImGui::SameLine();
		ImGui::Text("%.2f%%", p.second / total_time * 100.0);
	}
	ImGui::Text("############################################################");
	ImGui::Text("Total time taken for this frame: %.6f seconds.", total_time);
	float fps = engine->Get_FPS();
	ImGui::Text("Frame Rate is: %.6f FPS", fps);

	ImGui::End();
}

/*
	Object Properties
*/
int cloneSuccessful = -1;
bool selected = false;

void LevelEditor::ObjectProperties() {

	ImGui::SetNextWindowSize(ImVec2(450, 0));
	char buffer[100];

	ImGui::Begin("Object Properties");

	if (!selected) {
		ImGui::BeginChild("Texture", ImVec2(ImGui::GetContentRegionAvail().x * 0.25f, ImGui::GetContentRegionAvail().x * 0.25f));
		ImGui::Image(NULL, ImGui::GetContentRegionAvail());
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("ID", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x * 0.25f));

		ImGui::Text("Select an object to view or edit it's");
		ImGui::Text("properties");

		ImGui::EndChild();
		ImGui::End();

		return;
	}

	static bool Transform_EditMode = false;

	static Vec2 edited_position;
	static float edited_rotation;
	static Vec2 edited_scale;

	static bool Body_EditMode = false;

	static bool edited_active;
	static bool edited_collision_response;

	static bool AABB_EditMode = false;

	static float edited_aabb_width;
	static float edited_aabb_height;

	static bool Physics_EditMode = false;

	static Vec2 edited_velocity;
	static bool edited_gravity;

	Object* object;

	if (selectedNum < 0) {
		auto it = AssetManager::prefabs.begin();
		std::advance(it, -(selectedNum+1));
		object = it->second;
	}
	else
		object = objectFactory->getObjectWithID(selectedNum);

	Transform* tr = (Transform*)object->GetComponent(ComponentType::Transform);
	Texture* te = (Texture*)object->GetComponent(ComponentType::Texture);
	Body* b = (Body*)object->GetComponent(ComponentType::Body);
	Physics* ph = (Physics*)object->GetComponent(ComponentType::Physics);
	PlayerControllable* pc = (PlayerControllable*)object->GetComponent(ComponentType::PlayerControllable);
	Animation* a = (Animation*)object->GetComponent(ComponentType::Animation);

	ImGui::BeginChild("Texture", ImVec2(ImGui::GetContentRegionAvail().x * 0.25f, ImGui::GetContentRegionAvail().x * 0.25f));

	if (a != nullptr) {
		GLint width, height;

		// Bind the texture
		glBindTexture(GL_TEXTURE_2D, a->animation_tex_obj);

		// Get the texture width
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);

		// Get the texture height
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

		// Unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);

		ImVec2 uv0 = { (float)a->frame_num * 128.f / (float)width, (float)a->current_type * 128.f / (float)height };
		ImVec2 uv1 = { uv0.x + (128.f / (float)width), uv0.y + (128.f / (float)height) };

		ImGui::Image((void*)(intptr_t)a->animation_tex_obj, ImGui::GetContentRegionAvail(), uv0, uv1);
	}
	else if (te != nullptr) {
		if (tr->Scale.x > tr->Scale.y) {
			float padding = ImGui::GetContentRegionAvail().y * (tr->Scale.y / tr->Scale.x) * 0.5f;
			ImGui::Dummy(ImVec2(0, padding));
			ImGui::Image((void*)(intptr_t)AssetManager::textureval(te->textureName), ImVec2(ImGui::GetContentRegionAvail().x, tr->Scale.y / tr->Scale.x * ImGui::GetContentRegionAvail().y));
		}
		else if (tr->Scale.x == tr->Scale.y)
			ImGui::Image((void*)(intptr_t)AssetManager::textureval(te->textureName), ImGui::GetContentRegionAvail());
		else {
			float padding = ImGui::GetContentRegionAvail().x * (tr->Scale.x / tr->Scale.y) * 0.5f;
			ImGui::Dummy(ImVec2(padding, 0));
			ImGui::SameLine();
			ImGui::Image((void*)(intptr_t)AssetManager::textureval(te->textureName), ImVec2(tr->Scale.x / tr->Scale.y * ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
		}
	}
	else {
		ImGui::Text("This object has");
		ImGui::Text("no texture or");
		ImGui::Text("animations");
	}
	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild("ID", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x * 0.25f));

	if (selectedNum < 0) {
		ImGui::Text("Prefab Name: %s", object->GetName().c_str());
		ImGui::Text("Number of components: %d", object->GetNumComponents());

		if (ImGui::Button("Insert Prefab"))
		{
			Object* o = objectFactory->cloneObject(object);
			objectFactory->assignIdToObject(o);
			selectedNum = o->GetId();
			cloneSuccessful = selectedNum;
		}
	}
	else {
		ImGui::Text("Object ID: %d", object->GetId());
		ImGui::Text("Object Name: %s", object->GetName().c_str());
		ImGui::Text("Number of components: %d", object->GetNumComponents());

		if (ImGui::Button("Clone"))
		{
			Object* o = objectFactory->cloneObject(object);
			objectFactory->assignIdToObject(o);
			selectedNum = o->GetId();
			cloneSuccessful = selectedNum;
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Add Component")) {
		if (b == nullptr || ph == nullptr)
			ImGui::OpenPopup("AddComponent");
		else
			ImGui::OpenPopup("NoComponentsToAdd");
	}

	if (ImGui::BeginPopup("AddComponent"))
	{
		if (b == nullptr)
			if (ImGui::Selectable("Body")) {
				object->AddComponent(new Rectangular());
			}

		if (ph == nullptr)
			if (ImGui::Selectable("Physics")) {
				object->AddComponent(new Physics());
			}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopup("NoComponentsToAdd"))
	{
		ImGui::TextDisabled("There isn't any missing components to add");
		ImGui::EndPopup();
	}

	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.f, 0.f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.f, 0.f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.f, 0.f, 1.f));
	if (ImGui::Button("Delete Object"))
	{
		objectFactory->destroyObject(object);
		selectedNum = -1;
		selected = false;
	}
	ImGui::PopStyleColor(3);

	ImGui::EndChild();
	ImGui::BeginChild("ObjectPropertiesScroll", ImGui::GetContentRegionAvail());
	// Texture
	if (te != nullptr) {
		if (ImGui::CollapsingHeader("Texture")) {

			for (const auto& pair : AssetManager::textures) {
				if (pair.first == te->textureName) {
					ImGui::Text("Texture: %s", pair.first.c_str());
				}
			}

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.f, 0.f, 1.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.f, 0.f, 1.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.f, 0.f, 1.f));
			if (ImGui::Button("Delete"))
			{
				objectFactory->DeleteComponent(object->GetId(), ComponentType::Texture);
			}
			ImGui::PopStyleColor(3);
		}
	}

	// Animation
	if (a != nullptr) {
		if (ImGui::CollapsingHeader("Animation")) {
			GLint width, height;

			// Bind the texture
			glBindTexture(GL_TEXTURE_2D, a->animation_tex_obj);

			// Get the texture width
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);

			// Get the texture height
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

			// Unbind the texture
			glBindTexture(GL_TEXTURE_2D, 0);

			ImGui::SeparatorText("Sprite Sheet");

			if (width > height) {
				ImGui::Image((void*)(intptr_t)a->animation_tex_obj, ImVec2(ImGui::GetContentRegionAvail().x, (float)height / (float)width * ImGui::GetContentRegionAvail().x));
			}
			else if (width == height)
				ImGui::Image((void*)(intptr_t)a->animation_tex_obj, ImGui::GetContentRegionAvail());
			else {
				ImGui::Image((void*)(intptr_t)a->animation_tex_obj, ImVec2((float)width / (float)height * ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x));
			}

			ImGui::SeparatorText("Values");

			ImGui::Text("Current Type: %d", a->current_type);
			ImGui::Text("Frame Number: %d", a->frame_num);

		}
	}

	// Transform
	if (tr != nullptr) {
		if (ImGui::CollapsingHeader("Transform")) {
			if (Transform_EditMode)
			{
				// Display input fields
				ImGui::InputFloat2("Position", &(edited_position.x));
				ImGui::InputFloat("Rotation", &edited_rotation);
				ImGui::InputFloat2("Scale", &(edited_scale.x));

				// Button to exit edit mode
				if (ImGui::Button("Done##Transform"))
				{
					Transform_EditMode = false;
					tr->Position = edited_position;
					tr->Rotation = edited_rotation;
					tr->Scale = edited_scale;
				}

				ImGui::SameLine();

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.f, 0.f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.f, 0.f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.f, 0.f, 1.f));
				if (ImGui::Button("Cancel##Transform"))
				{
					Transform_EditMode = false;
				}
				ImGui::PopStyleColor(3);
			}
			else
			{
				// Display the values as text
				ImGui::Text("Position: %.5f, %.5f", tr->Position.x, tr->Position.y);
				ImGui::Text("Rotation: %.5f", tr->Rotation);
				ImGui::Text("Scale: %.5f, %.5f", tr->Scale.x, tr->Scale.y);

				// Button to enter edit mode
				if (ImGui::Button("Edit##Transform"))
				{
					Transform_EditMode = true;
					edited_position = tr->Position;
					edited_rotation = tr->Rotation;
					edited_scale = tr->Scale;
				}
			}
		}
	}

	// Body
	if (b != nullptr) {
		if (ImGui::CollapsingHeader("Body")) {
			ImGui::SeparatorText("General Body Settings");

			if (Body_EditMode)
			{
				// Display input fields
				ImGui::Checkbox("Active", &edited_active);
				ImGui::Checkbox("Respond to collision", &edited_collision_response);

				// Button to exit edit mode
				if (ImGui::Button("Done##Body"))
				{
					Body_EditMode = false;
					b->active = edited_active;
					b->collision_response = edited_collision_response;
				}

				ImGui::SameLine();

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.f, 0.f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.f, 0.f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.f, 0.f, 1.f));
				if (ImGui::Button("Cancel##Body"))
				{
					Body_EditMode = false;
				}
				ImGui::PopStyleColor(3);
			}
			else
			{
				// Display the values as text
				ImGui::Text("Active: ");
				ImGui::SameLine();
				b->active ? ImGui::Text("true") : ImGui::Text("false");

				ImGui::Text("Respond to collision: ");
				ImGui::SameLine();
				b->collision_response ? ImGui::Text("true") : ImGui::Text("false");

				// Button to enter edit mode
				if (ImGui::Button("Edit##Body"))
				{
					Body_EditMode = true;
					edited_active = b->active;
					edited_collision_response = b->collision_response;
				}

				ImGui::SameLine();

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.f, 0.f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.f, 0.f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.f, 0.f, 1.f));
				if (ImGui::Button("Delete##Body"))
				{
					objectFactory->DeleteComponent(object->GetId(), ComponentType::Body);
					b = nullptr;
				}
				ImGui::PopStyleColor(3);
			}

			if (b != nullptr && b->GetShape() == Shape::Rectangle) {
				Rectangular* r = (Rectangular*)b;

				ImGui::SeparatorText("AABB Collision Settings");

				if (AABB_EditMode)
				{
					// Display input fields
					ImGui::InputFloat("AABB Width", &edited_aabb_width);
					ImGui::InputFloat("AABB Height", &edited_aabb_height);

					// Button to exit edit mode
					if (ImGui::Button("Done##AABB"))
					{
						AABB_EditMode = false;
						r->width = edited_aabb_width;
						r->height = edited_aabb_height;
					}

					ImGui::SameLine();

					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.f, 0.f, 1.f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.f, 0.f, 1.f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.f, 0.f, 1.f));
					if (ImGui::Button("Cancel##AABB"))
					{
						AABB_EditMode = false;
					}
					ImGui::PopStyleColor(3);
				}
				else
				{
					// Display the values as text
					ImGui::Text("AABB Width: %.5f", r->width);

					ImGui::SameLine();

					ImGui::Button("-##aabbwidth");

					if (ImGui::IsItemActive()) { // Will run so long as the above button is held
						r->width -= engine->GetDt() * 2.f;
						r->width < 0.f ? 0.f : r->width; // Do not go below 0
					}

					ImGui::SameLine();

					ImGui::Button("+##aabbwidth");

					if (ImGui::IsItemActive()) { // Will run so long as the above button is held
						r->width += engine->GetDt() * 2.f;
					}

					ImGui::Text("AABB Height: %.5f", r->height);

					ImGui::SameLine();

					ImGui::Button("-##aabbheight");

					if (ImGui::IsItemActive()) { // Will run so long as the above button is held
						r->height -= engine->GetDt() * 2.f;
						r->height < 0.f ? 0.f : r->width; // Do not go below 0
					}

					ImGui::SameLine();

					ImGui::Button("+##aabbheight");

					if (ImGui::IsItemActive()) { // Will run so long as the above button is held
						r->height += engine->GetDt() * 2.f;
					}

					// Button to enter edit mode
					if (ImGui::Button("Edit##AABB"))
					{
						AABB_EditMode = true;
						edited_aabb_width = r->width;
						edited_aabb_height = r->height;
					}
				}

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

	// Physics
	if (ph != nullptr) {
		if (ImGui::CollapsingHeader("Physics")) {
			if (Physics_EditMode)
			{
				// Display input fields
				ImGui::InputFloat2("Velocity", &(edited_velocity.x));
				ImGui::Checkbox("Affected by gravity: ", &edited_gravity);

				// Button to exit edit mode
				if (ImGui::Button("Done##Physics"))
				{
					Physics_EditMode = false;
					ph->Velocity = edited_velocity;
					ph->AffectedByGravity = edited_gravity;
				}

				ImGui::SameLine();

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.f, 0.f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.f, 0.f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.f, 0.f, 1.f));
				if (ImGui::Button("Cancel##Physics"))
				{
					Physics_EditMode = false;
				}
				ImGui::PopStyleColor(3);
			}
			else
			{
				// Display the values as text
				ImGui::Text("Velocity: %.5f, %.5f", ph->Velocity.x, ph->Velocity.y);
				ImGui::Text("Affected by gravity: ");
				ImGui::SameLine();
				ph->AffectedByGravity ? ImGui::Text("true") : ImGui::Text("false");

				// Button to enter edit mode
				if (ImGui::Button("Edit##Physics"))
				{
					Physics_EditMode = true;
					edited_velocity = ph->Velocity;
					edited_gravity = ph->AffectedByGravity;
				}

				ImGui::SameLine();

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.f, 0.f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.f, 0.f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.f, 0.f, 1.f));
				if (ImGui::Button("Delete##Physics"))
				{
					objectFactory->DeleteComponent(object->GetId(), ComponentType::Physics);
					ph = nullptr;
				}
				ImGui::PopStyleColor(3);
			}
		}
	}

	// PlayerControllable
	if (pc != nullptr) {
		if (ImGui::CollapsingHeader("PlayerControllable")) {
			ImGui::Text("Nothing right now");
		}
	}

	ImGui::EndChild();
	ImGui::End();
}

/*
	Object List
*/
void LevelEditor::ListOfObjects() {

	ImGui::Begin("Object List");
	ImGui::Text("Number of game objects in level: %d", objectFactory->NumberOfObjects());
	ImGui::BeginChild("ObjectListScroll", ImGui::GetContentRegionAvail());
	if (ImGui::BeginTable("ObjectList", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
	{

		for (size_t i = 0; i < objectFactory->NumberOfObjects(); i++)
		{

			if (objectFactory->getObjectWithID((long)i) == nullptr)
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
			if (ImGui::Selectable(buf, selectedNum == static_cast<int>(i))) {
				selected = true;
				selectedNum = static_cast<int>(i);
			}
		}
		ImGui::EndTable();
	}
	ImGui::EndChild();
	ImGui::End();
}

/*
	Displays the selected texture from the asset list
*/

std::pair<std::string, GLuint> selectedTexture;
bool display_selected_texture = false;

void LevelEditor::DisplaySelectedTexture() {

	if (display_selected_texture) {

		GLint width, height;

		// Bind the texture
		glBindTexture(GL_TEXTURE_2D, selectedTexture.second);

		// Get the texture width
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);

		// Get the texture height
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

		// Unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);

		ImGui::SetNextWindowPos(ImVec2((float)window->width * 0.15f, (float)window->height * 0.15f), ImGuiCond_Appearing);

		ImVec2 image_size((float) width, (float)height);

		if (width > window->width * 0.3) {
			image_size = ImVec2((float)window->width * 0.3f, (float)height * ((float)window->width * 0.3f / (float) width));
		} else if (height > window->height * 0.3) {
			image_size = ImVec2(((float)width * ((float)window->height * 0.3f / (float)height)), (float)window->height * 0.3f);
		}

		
		ImGui::SetNextWindowSize(ImVec2(image_size.x + 20, image_size.y + 40), ImGuiCond_Always);

		ImGui::Begin(selectedTexture.first.c_str(), &display_selected_texture);

		//ImVec2 windowSize = ImGui::GetWindowSize();
		//ImVec2 displaySize = ImVec2(windowSize.x, windowSize.y);

		ImGui::Image((void*)(intptr_t)selectedTexture.second, image_size);

		ImGui::End();
	}
}

/*
	Asset List
*/
void LevelEditor::AssetList() {
	ImGui::Begin("Asset List");

	if (ImGui::BeginTabBar("##AssetList")) {
		if (ImGui::BeginTabItem("Textures")) {

			if (ImGui::Button("Refresh Textures"))
			{
				AssetManager::unloadalltextures();
				AssetManager::loadalltextures();

			}
			ImGui::BeginChild("AssetListScroll", ImGui::GetContentRegionAvail());
			ImVec2 button_size = ImVec2(ImGui::GetWindowSize().x, 64);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 1.f));
			for (const std::pair<std::string, GLuint>& t : AssetManager::textures) {
				char buffer[256];
				sprintf_s(buffer, "##%s", t.first.c_str());

				// Start the invisible button
				
				if (ImGui::Button(buffer, button_size))
				{
					selectedTexture = t;
					display_selected_texture = true;

				}

				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y - 68));

				// Image
				ImGui::Image((void*)(intptr_t)t.second, ImVec2(64, 64));

				// Move to the right of the image without moving to a new line
				ImGui::SameLine();

				// Text
				ImGui::Text(t.first.c_str());
			}

			ImGui::PopStyleColor();
			ImGui::EndChild();
			ImGui::EndTabItem();
		}
	}
	if (ImGui::BeginTabItem("Prefabs")) {
		ImVec2 button_size = ImVec2(ImGui::GetWindowSize().x, 64);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 1.f));
		//int i = 0;
		for (const std::pair<std::string, Object *>& p : AssetManager::prefabs) {
			//i--;

			char buffer[256];
			sprintf_s(buffer, "##%s", p.first.c_str());

			// Text and images will be in the above layer
			if (ImGui::Button(buffer, button_size))
			{
				selected = true;
				//selectedNum = i;
				selectedNum = -(std::distance(AssetManager::prefabs.begin(), AssetManager::prefabs.find(p.first))) - 1;
			}

			ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y - 68));

			// Get texture or animation
			Texture* t = (Texture*)p.second->GetComponent(ComponentType::Texture);
			Animation* a = (Animation*)p.second->GetComponent(ComponentType::Animation);

			// Image
			if (t != nullptr)
				ImGui::Image((void*)(intptr_t)AssetManager::textureval(t->textureName), ImVec2(64, 64));
			// or Animation
			else if (a != nullptr) {
				GLint width, height;

				// Bind the texture
				glBindTexture(GL_TEXTURE_2D, a->animation_tex_obj);

				// Get the texture width
				glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);

				// Get the texture height
				glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

				// Unbind the texture
				glBindTexture(GL_TEXTURE_2D, 0);

				ImVec2 uv0 = { (float)a->frame_num * 128.f / (float)width, (float)a->current_type * 128.f / (float)height };
				ImVec2 uv1 = { uv0.x + (128.f / (float)width), uv0.y + (128.f / (float)height) };

				ImGui::Image((void*)(intptr_t)a->animation_tex_obj, ImVec2(64, 64), uv0, uv1);
			}

			// Move to the right of the image without moving to a new line
			ImGui::SameLine();

			// Text
			ImGui::Text(p.second->GetName().c_str());
		}

		ImGui::PopStyleColor();
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();
	ImGui::End();
}

/*
	This is for a window to pop up telliing that the object has cloned successfully
*/
void ObjectClonedSuccessfully(int i) {
	ImGui::SetNextWindowPos(ImVec2((float)window->width / 2.f, (float)window->height / 2.f));
	ImGui::SetNextWindowSize(ImVec2(0, 0));
	char text[50];

	ImGui::Begin("Clone Successful");
	sprintf_s(text, "New object ID is: %d", i);

	ImVec2 textSize = ImGui::CalcTextSize(text);
	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 textPos = {
		(windowSize.x - textSize.x) * 0.5f,
		(windowSize.y - textSize.y) * 0.5f
	};
	ImGui::SetCursorPos(textPos);
	ImGui::Text(text);

	if (ImGui::Button("OK"))
	{
		cloneSuccessful = -1;
	}
	ImGui::End();
}

void PlayPauseGame() {
	ImGui::Begin("Play/Pause");

	if (engine->isPaused()) {
		if (ImGui::Button("Play"))
			engine->setPause();
	}
	else {
		if (ImGui::Button("Pause"))
			engine->setPause();
	}

	ImGui::End();
}

void CameraControl() {

	ImGui::Begin("Camera Control");

	if (camera2D->isFreeCamEnabled()) {
		if (ImGui::Button("Disable Free Cam")) {
			camera2D->scale = { 1.f, 1.f };
			camera2D->position = { 0.f, 0.f };
			camera2D->toggleFreeCam();
		}
	}
	else {
		if (ImGui::Button("Enable Free Cam"))
			camera2D->toggleFreeCam();
	}

	if (!camera2D->isFreeCamEnabled()) {
		// Make the button look disabled by reducing its alpha
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

		// Make the button unclickable
		ImGui::BeginDisabled(true);
	}

	ImGui::SameLine();

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 50.f, ImGui::GetCursorPosY()));

	ImGui::Button("Up##CameraControl");
	if (ImGui::IsItemActive()) { camera2D->position.y -= engine->GetDt() * 2.f; }

	ImGui::SameLine();

	ImGui::Button("Down##CameraControl");
	if (ImGui::IsItemActive()) { camera2D->position.y += engine->GetDt() * 2.f; }

	ImGui::SameLine();

	ImGui::Button("Left##CameraControl");
	if (ImGui::IsItemActive()) { camera2D->position.x += engine->GetDt() * 2.f; }

	ImGui::SameLine();

	ImGui::Button("Right##CameraControl");
	if (ImGui::IsItemActive()) { camera2D->position.x -= engine->GetDt() * 2.f; }

	ImGui::SameLine();

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 50.f, ImGui::GetCursorPosY()));

	if(ImGui::Button("Reset to Player##CameraControl")) { camera2D->SetToPlayer(); }

	ImGui::SameLine();

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 50.f, ImGui::GetCursorPosY()));

	ImGui::Button("Zoom In##CameraControl");
	if (ImGui::IsItemActive()) { camera2D->scale *= 1.f + (engine->GetDt() * 1.5f); }

	ImGui::SameLine();

	ImGui::Button("Zoom Out##CameraControl");
	if (ImGui::IsItemActive()) { camera2D->scale /= 1.f + (engine->GetDt() * 1.5f); }

	ImGui::SameLine();

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 50.f, ImGui::GetCursorPosY()));

	if(ImGui::Button("Reset Zoom##CameraControl")) { camera2D->scale = {1.f, 1.f}; }

	if (!camera2D->isFreeCamEnabled()) {
		// End the disabled section
		ImGui::EndDisabled();

		// Restore original style
		ImGui::PopStyleVar();
	}
		
	ImGui::End();
}

void DoNothing() {

}

void LevelEditor::Initialize() {
	total_time = 0.0;
}

/************************************LEVEL EDITOR MAIN UPDATE LOOP************************************/

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

	ImGui::Begin("DockSpace", &dock_space, window_flags);

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
		io = ImGui::GetIO();
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
			if (ImGui::BeginMenu("Load Level")) {
				std::vector<std::string> level_files;
				const std::string path = "Asset/Levels/";

				try {
					for (const auto& entry : std::filesystem::directory_iterator(path)) {
						if (entry.is_regular_file() && entry.path().extension() == ".json") {
							level_files.push_back(entry.path().filename().string());
						}
					}

					// Print the filenames
					for (const auto& filename : level_files) {
						if (ImGui::MenuItem(filename.c_str())) {
							selectedNum = -1;
							objectFactory->destroyAllObjects();
							LoadScene(path + filename.c_str());
						}
					}
				}
				catch (std::filesystem::filesystem_error& e) {
					std::cerr << e.what() << std::endl;
				}
				ImGui::EndMenu();
			}

			ImGui::Separator();

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

	AssetList();

	DisplaySelectedTexture();

	PlayPauseGame();

	CameraControl();

	if (cloneSuccessful > -1) {
		ObjectClonedSuccessfully(cloneSuccessful);
	}
}

/************************************LEVEL EDITOR GRID************************************/

LevelEditorGrid* editor_grid;
LevelEditorGrid::LevelEditorGrid()
{
	set_num({ 12, 12 });
	editor_grid = this;
}

glm::vec3 box_color_editor{ 0.0f, 0.5f, 0.5f };

void LevelEditorGrid::drawleveleditor()
{
	if (num.x > num.y)
		box_size = scale_window.x / num.x;
	else box_size = scale_window.y / num.y;
	Vec2 scaling = { box_size / window->width, box_size / window->height };
	pos_botleft = {
		(-box_size * (num.x - 1)) / window->width,
		(-box_size * (num.y - 1)) / window->height
	};
	for (int i = 0; i < num.x; i++) {
		for (int j = 0; j < num.y; j++) {
			Vec2 pos = pos_botleft + Vec2(i * box_size * 2 / window->width, j * box_size * 2 / window->height);
			Mat3 mat_test = Mat3Translate(pos.x, pos.y) * Mat3Scale(scaling.x, scaling.y);
			mat_test = camera2D->world_to_ndc * mat_test;
			app->shdrpgms["shape"].Use();
			// bind VAO of this object's model
			glBindVertexArray(app->models["square"].vaoid);
			// copy object's model-to-NDC matrix to vertex shader's
			// uniform variable uModelToNDC
			app->shdrpgms["shape"].SetUniform("uModel_to_NDC", mat_test.ToGlmMat3());
			app->shdrpgms["shape"].SetUniform("uColor", box_color_editor);
			// call glDrawElements with appropriate arguments
			glDrawElements(app->models["square"].primitive_type, app->models["square"].draw_cnt, GL_UNSIGNED_SHORT, 0);

			// unbind VAO and unload shader program
			glBindVertexArray(0);
			app->shdrpgms["shape"].UnUse();
			Vec2 botleft = { (i - num.x / 2) * box_size, (j - num.y / 2) * box_size };
			Vec2 topright = { botleft.x + box_size,botleft.y + box_size };
			app->drawline(Vec2(topright.x, botleft.y), botleft, glm::vec3{ 0.0f, 1.0f, 1.0f });
			app->drawline(topright, Vec2(topright.x, botleft.y), glm::vec3{ 0.0f, 1.0f, 1.0f });
			app->drawline(topright, Vec2(botleft.x, topright.y), glm::vec3{ 0.0f, 1.0f, 1.0f });
			app->drawline(Vec2(botleft.x, topright.y), botleft, glm::vec3{ 0.0f, 1.0f, 1.0f });
		}
	}
}