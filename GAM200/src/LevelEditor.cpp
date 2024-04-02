/* !
@file	LevelEditor.cpp
@author Tan Yee Ann (t.yeeann@digipen.edu)
@date	2/11/2023

This file contains the definitions of the functions that are part of the level editor
*//*__________________________________________________________________________*/
#include <LevelEditor.h>
#include <Debug.h>
#include <ImGui/imgui.h>
#include <Collision.h>
#include <Core_Engine.h>
#include <components/Texture.h>
#include <components/Animation.h>
#include <filesystem>
#include "Scenes.h"
#include "Assets Manager/asset_manager.h"
#include <PhysicsSystem.h>
#include <Vec4.h>
#include <components/Event.h>
#include <SceneManager.h>
#include "../../src/Assets Manager/asset_manager.h"
#include <components/Text.h>
#include <json/json.h>
#include <windows.h>
#include <commdlg.h>
#include <ThreadPool.h>
LevelEditor* level_editor = nullptr; // declared in LevelEditor.cpp
bool showUniformGrid = false;
bool showPerformanceInfo = false;

bool dock_space = true; // Always must be on for level editor

ImGuiStyle* style;
ImFont* font;

char buffer[256];

bool save_as_dialog = false;
bool new_prefab_dialog = false;

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

/******************************************************************************
	Default Constructor for LevelEditor
*******************************************************************************/
LevelEditor::LevelEditor() {
	editor_grid = new LevelEditorGrid();
}

/******************************************************************************
	Default Destructor for LevelEditor
*******************************************************************************/
LevelEditor::~LevelEditor() {
	delete editor_grid;

	SceneManager::ClearInitialObjectMap(true);

	Json::Value soundsJson;
	for (const auto& [key, value] : AssetManager::soundMapping) {
		std::visit(overload{
			[&](const std::string& s) { soundsJson[key] = s; },
			[&](const std::vector<std::string>& v) {
				for (const auto& item : v) {
					soundsJson[key].append(item);
				}
			}
			}, value);
	}
	Json::StreamWriterBuilder builder;
	builder["commentStyle"] = "None";
	builder["indentation"] = "  ";

	std::ofstream outputFile("Asset/Sounds/sounds.json");
	if (outputFile.is_open()) {
		Json::StreamWriterBuilder writer;
		writer["indentation"] = "  ";

		outputFile << Json::writeString(builder, soundsJson);
		outputFile.close();
		std::cout << "Successfully saved sounds.json" << std::endl;
	}
	else
		std::cerr << "Failed to open file for writing." << std::endl;
}

/******************************************************************************
	DebugUniformGrid
	- This window prints out a formatted table of how many objects are inside
	  a particular grid
*******************************************************************************/
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

/******************************************************************************
	DebugPerformanceViewer
	- This window prints out the time taken for each system to complete it's update
	  and the total time taken to complete each game loop.
	- It will also print out the FPS of the game
*******************************************************************************/
void LevelEditor::DebugPerformanceViewer() {
	ImGui::SetNextWindowSize(ImVec2(0, 0));
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

/******************************************************************************
	Object Properties
	- This window allows the user to view the details of a selected object and
	  it's components
	- It also allows the user to modify the object's properties
*******************************************************************************/
int cloneSuccessful = -1;

static bool Transform_EditMode = false;

static Vec2 edited_position;
static float edited_rotation;
static Vec2 edited_scale;

static bool Animation_EditMode = false;

static bool Text_EditMode = false;
static char edited_text[1024] = "";
static float edited_font_size;

static bool Body_EditMode = false;

static bool edited_active;
static bool edited_collision_response;
static bool edited_pushable;
static Material edited_material;

static bool AABB_EditMode = false;

static float edited_aabb_width;
static float edited_aabb_height;

static bool Physics_EditMode = false;

static Vec2 edited_velocity;
static float edited_mass;
static bool edited_gravity;
static bool edited_able_to_push_objects;

static bool update_all_instances = false;

void LevelEditor::ObjectProperties() {

	ImGui::SetNextWindowSize(ImVec2(450, 0));

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

	Object* object;

	if (selectedNum < 0) {
		auto it = AssetManager::prefabs.begin();
		std::advance(it, -(selectedNum + 1));
		object = it->second;
	}
	else
		object = objectFactory->getObjectWithID(selectedNum);

	Transform* tr = (Transform*)object->GetComponent(ComponentType::Transform);
	Texture* te = (Texture*)object->GetComponent(ComponentType::Texture);
	Body* bo = (Body*)object->GetComponent(ComponentType::Body);
	Physics* ph = (Physics*)object->GetComponent(ComponentType::Physics);
	PlayerControllable* pc = (PlayerControllable*)object->GetComponent(ComponentType::PlayerControllable);
	Animation* a = (Animation*)object->GetComponent(ComponentType::Animation);
	Event* e = (Event*)object->GetComponent(ComponentType::Event);
	Behaviour* be = (Behaviour*)object->GetComponent(ComponentType::Behaviour);
	Text* t = (Text*)object->GetComponent(ComponentType::Text);

	ImGui::BeginChild("Texture", ImVec2(128.f, 128.f));

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
	else if (te != nullptr)
	{
		Vec2 size_preview = tr ? tr->Scale : Vec2(64.f, 64.f);

		if (size_preview.x > size_preview.y)
		{
			float padding = ImGui::GetContentRegionAvail().y * (size_preview.y / size_preview.x) * 0.5f;
			ImGui::Dummy(ImVec2(0, padding));
			ImGui::Image((void*)(intptr_t)AssetManager::textureval(te->textureName), ImVec2(ImGui::GetContentRegionAvail().x, size_preview.y / size_preview.x * ImGui::GetContentRegionAvail().y));
		}
		else if (size_preview.x == size_preview.y)
			ImGui::Image((void*)(intptr_t)AssetManager::textureval(te->textureName), ImGui::GetContentRegionAvail());
		else
		{
			float padding = ImGui::GetContentRegionAvail().x * (size_preview.x / size_preview.y) * 0.5f;
			ImGui::Dummy(ImVec2(padding, 0));
			ImGui::SameLine();
			ImGui::Image((void*)(intptr_t)AssetManager::textureval(te->textureName), ImVec2(size_preview.x / size_preview.y * ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
		}


	}
	else {
		ImGui::Text("This object has");
		ImGui::Text("no texture or");
		ImGui::Text("animations");
	}
	ImGui::EndChild();

	//Accept drag and drop for game texture
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Game texture"))
		{
			char dropped_object[1024];

			memcpy_s(dropped_object, payload->DataSize, payload->Data, payload->DataSize);

			if (te == nullptr && a == nullptr) {
				object->AddComponent(new Texture(dropped_object));
				tr->Scale = { 64.f, 64.f };
			}
			else
				te->textureName = dropped_object;

			UpdateAllObjectInstances(object);
			ImGui::EndDragDropTarget();
		}

		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Game animations"))
		{
			char dropped_object[1024];

			memcpy_s(dropped_object, payload->DataSize, payload->Data, payload->DataSize);

			if (te == nullptr && a == nullptr) {
				Animation* dropped_a = new Animation();
				dropped_a->animation_tex_obj = AssetManager::animationval(dropped_object);
				object->AddComponent(dropped_a);
				tr->Scale = { 64.f, 64.f };
			}
			else
				a->animation_tex_obj = AssetManager::animationval(dropped_object);

			UpdateAllObjectInstances(object);
			ImGui::EndDragDropTarget();
		}
	}

	ImGui::SameLine();

	ImGui::BeginChild("ID", ImVec2(ImGui::GetContentRegionAvail().x, 128.f));

	if (selectedNum < 0) {
		ImGui::Text("Prefab Name: %s", object->GetName().c_str());
		ImGui::Text("Number of components: %d", object->GetNumComponents());

		if (ImGui::Button("Insert Prefab"))
		{
			/*
			Object* o = objectFactory->cloneObject(object);
			objectFactory->assignIdToObject(o);
			selectedNum = o->GetId();
			cloneSuccessful = selectedNum;
			*/
			ImGui::OpenPopup("CloneObject");
		}
	}
	else {
		static char newName[256];
		strncpy_s(newName, object->GetName().c_str(), sizeof(newName));
		ImGui::Text("Object ID: %d", object->GetId());
		ImGui::Text("In Layer: %s", objectFactory->FindLayerThatHasThisObject(object)->first.c_str());
		LE_InputText("Name", newName, sizeof(newName));

		object->SetName(newName);

		ImGui::Text("Object Name: %s", object->GetName().c_str());
		ImGui::Text("Number of components: %d", object->GetNumComponents());

		/*
		if (ImGui::Button("Clone (To be deleted)"))
		{
			Object* o = objectFactory->cloneObject(object, 64);
			objectFactory->assignIdToObject(o);
			selectedNum = o->GetId();
			cloneSuccessful = selectedNum;

			objectFactory->FindLayerThatHasThisObject(object)->second.second.push_back(o);
		}

		// For convinence
		if (ImGui::Button("Cloneup (To be deleted)"))
		{
			Object* o = objectFactory->cloneObject(object, 0, 64);
			objectFactory->assignIdToObject(o);
			selectedNum = o->GetId();
			cloneSuccessful = selectedNum;

			objectFactory->FindLayerThatHasThisObject(object)->second.second.push_back(o);
		}

		*/

		// For convinence
		if (ImGui::Button("Clone"))
		{
			ImGui::OpenPopup("CloneObject");
		}
	}

	if (ImGui::BeginPopup("CloneObject"))
	{
		ImGui::Text("Select layer to insert the new object to:");
		for (auto& l : SceneManager::layers) {
			if (ImGui::Selectable(l.first.c_str())) {
				Object* o = objectFactory->cloneObject(object, 0, 64);

				// For inserting prefab
				if (!o->GetPrefab())
					o->SetPrefab(object);

				objectFactory->assignIdToObject(o);

				SceneManager::CalculateLevelSize();

				selectedNum = o->GetId();
				//o->SetPrefab(object->GetPrefab()); // testing this line
				cloneSuccessful = selectedNum;
				l.second.second.push_back(o);
			}
		}

		ImGui::EndPopup();
	}

	ImGui::SameLine();

	if (ImGui::Button("Add Component")) {
		if (bo == nullptr || ph == nullptr || be == nullptr)
			ImGui::OpenPopup("AddComponent");
		else
			ImGui::OpenPopup("NoComponentsToAdd");
	}

	if (ImGui::BeginPopup("AddComponent"))
	{
		if (tr == nullptr)
			if (ImGui::Selectable("Transform")) {
				object->AddComponent(new Transform());
			}
		if (te == nullptr && a == nullptr) {
			if (ImGui::Selectable("Texture")) {
				object->AddComponent(new Texture(""));
				UpdateAllObjectInstances(object);
			}
			/*
			if (ImGui::Selectable("Animation")) {
				object->AddComponent(new Animation());
				UpdateAllObjectInstances(object);
			}
			*/
		}
		if (bo == nullptr)
			if (ImGui::Selectable("Body")) {
				object->AddComponent(new Rectangular());
				UpdateAllObjectInstances(object);
			}
		if (ph == nullptr)
			if (ImGui::Selectable("Physics")) {
				object->AddComponent(new Physics());
				UpdateAllObjectInstances(object);
			}
		/*
		if (e == nullptr)
			if (ImGui::Selectable("Event")) {
				object->AddComponent(new Event());
				UpdateAllObjectInstances(object);
			}
			*/
		if (be == nullptr)
			if (ImGui::Selectable("Behaviour")) {
				object->AddComponent(new Behaviour());
				Logic->AddBehaviourComponent(static_cast<Behaviour*>(object->GetComponent(ComponentType::Behaviour)));
				UpdateAllObjectInstances(object);
				std::cout << "Object Name : " << object->GetName() << " | Object ID : " << object->GetId() << " Behaviour Added" << std::endl;
			}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopup("NoComponentsToAdd"))
	{
		ImGui::TextDisabled("There isn't any missing components to add");
		ImGui::EndPopup();
	}

	ImGui::SameLine();

	if (selectedNum >= 0) {
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
	}
	ImGui::EndChild();

	if (object->GetPrefab()) {
		sprintf_s(buffer, "Apply all changes to all instances of %s", object->GetPrefab()->GetName().c_str());

		ImGui::Checkbox(buffer, &update_all_instances);
	}
	else {
		ImGui::BeginDisabled();
		ImGui::Checkbox("There is no prefab linked to this object", &update_all_instances);
		ImGui::EndDisabled();
	}

	ImGui::BeginChild("ObjectPropertiesScroll", ImGui::GetContentRegionAvail());
	// Texture
	if (te != nullptr)
	{
		if (ImGui::CollapsingHeader("Texture"))
		{

			for (const auto& pair : AssetManager::textures)
			{
				if (pair.first == te->textureName)
				{
					ImGui::Text("Texture: %s", pair.first.c_str());
				}
			}

			LE_InputFloat("Opacity", &te->opacity);

			if (ImGui::Button("Change Texture"))
				ImGui::OpenPopup("ChangeTexture");

			if (ImGui::BeginPopup("ChangeTexture"))
			{
				ImGui::Text("Changes will apply to all instances of %s", object->GetName().c_str());
				int i = 0;
				for (const auto& tex : AssetManager::textures) {
					if (ImGui::ImageButton(tex.first.c_str(), (void*)(intptr_t)tex.second, ImVec2(64, 64))) {
						te->textureName = tex.first;
						UpdateAllObjectInstances(object);
					}

					if (((i + 1) % 4))
						ImGui::SameLine();

					i++;
				}

				ImGui::EndPopup();
			}

			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.f, 0.f, 1.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.f, 0.f, 1.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.f, 0.f, 1.f));

			if (ImGui::Button("Delete"))
			{
				objectFactory->DeleteComponent(object, ComponentType::Texture);
				te = nullptr;
				UpdateAllObjectInstances(object);
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

			if (Animation_EditMode){
				ImGui::SeparatorText("Values");

				ImGui::Text("Current Type: %d", a->current_type);
				ImGui::Text("Frame Number: %d", a->frame_num);
				LE_InputFloat("Opacity", &a->opacity);
				LE_InputFloat("Frame Rate", &a->frame_rate);
				LE_InputFloat2("Animation Scale", &a->animation_scale.first);

				ImGui::SeparatorText("Animation Settings");

				if (ImGui::BeginTable("AnimationSettings", 4, NULL)) {
					ImGui::TableSetupColumn("Row", ImGuiTableColumnFlags_WidthFixed, 50.f);
					ImGui::TableSetupColumn("Frame", ImGuiTableColumnFlags_WidthFixed, 150.f);
					ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 150.f);
					ImGui::TableSetupColumn("Options", ImGuiTableColumnFlags_WidthStretch);

					ImGui::TableHeadersRow();

					//for (auto& pair : a->animation_frame) {

					int a_size = a->animation_frame.size();

					for (int i = 0; i < a_size; i++) {

						std::pair<int, AnimationType> pair;

						try {
							pair = a->animation_frame.at(i + 1);
						}
						catch (std::out_of_range) {
							a_size++;
							continue;
						}

						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("%d", i + 1);
						ImGui::TableNextColumn();

						sprintf_s(buffer, "##FrameNum%d", i + 1);

						LE_InputInt(buffer, &a->animation_frame.at(i + 1).first);

						ImGui::TableNextColumn();

						int currentType = static_cast<int>(pair.second);

						std::vector<std::string> itemStrings(No_Animation_Type + 1);
						const char* items[No_Animation_Type + 1];
						for (int i = 0; i < No_Animation_Type + 1; ++i) {
							itemStrings[i] = AnimationTypeToString(static_cast<AnimationType>(i));
							items[i] = itemStrings[i].c_str();
						}

						sprintf_s(buffer, "##AnimationTypeBox%d", i + 1);

						if (ImGui::Combo(buffer, &currentType, items, No_Animation_Type + 1)) {
							a->animation_frame.at(i + 1).second = static_cast<AnimationType>(currentType);
						}

						ImGui::TableNextColumn();

						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.f, 0.f, 1.f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.f, 0.f, 1.f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.f, 0.f, 1.f));

						sprintf_s(buffer, "Delete##AnimationSettingRow%d", i + 1);
						if (ImGui::Button(buffer))
						{
							a->animation_frame.erase(i + 1);
						}
						ImGui::PopStyleColor(3);
					}

					ImGui::EndTable();

					if (ImGui::Button("Add Row")) {
						a->animation_frame.insert(std::make_pair((a->animation_frame.empty() ? 1 : a->animation_frame.rbegin()->first + 1), std::make_pair(0, AnimationType::No_Animation_Type)));
					}
				}
				LE_InputInt("Jump Fixed Frame", &a->jump_fixed_frame);

				if (ImGui::Button("Done##Animation")) {
					Animation_EditMode = false;
					a->set_up_map();

					UpdateAllObjectInstances(object);
				}


				ImGui::SameLine();

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.f, 0.f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.f, 0.f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.f, 0.f, 1.f));
				if (ImGui::Button("Cancel##Animation"))
				{
					Animation_EditMode = false;
				}
				ImGui::PopStyleColor(3);
			}
			else {
				ImGui::SeparatorText("Values");

				ImGui::Text("Current Type: %d", a->current_type);
				ImGui::Text("Frame Number: %d", a->frame_num);
				ImGui::Text("Opacity: %.2f", a->opacity);
				ImGui::Text("Frame Rate: %.2f", a->frame_rate);
				ImGui::Text("Animation Scale: %.2f, %.2f", a->animation_scale.first, a->animation_scale.second);

				ImGui::SeparatorText("Animation Settings");

				if (ImGui::BeginTable("AnimationSettings", 4, NULL)) {
					ImGui::TableSetupColumn("Row", ImGuiTableColumnFlags_WidthFixed, 50.f);
					ImGui::TableSetupColumn("Frame", ImGuiTableColumnFlags_WidthFixed, 150.f);
					ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 150.f);

					ImGui::TableHeadersRow();

					//for (auto& pair : a->animation_frame) {

					int a_size = a->animation_frame.size();

					for (int i = 0; i < a_size; i++) {

						std::pair<int, AnimationType> pair;

						try {
							pair = a->animation_frame.at(i + 1);
						}
						catch (std::out_of_range) {
							a_size++;
							continue;
						}

						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("%d", i + 1);
						ImGui::TableNextColumn();

						sprintf_s(buffer, "##FrameNum%d", i + 1);

						ImGui::Text("%d", a->animation_frame.at(i + 1).first);

						ImGui::TableNextColumn();

						int currentType = static_cast<int>(pair.second);

						std::vector<std::string> itemStrings(No_Animation_Type + 1);
						const char* items[No_Animation_Type + 1];
						for (int i = 0; i < No_Animation_Type + 1; ++i) {
							itemStrings[i] = AnimationTypeToString(static_cast<AnimationType>(i));
							items[i] = itemStrings[i].c_str();
						}

						sprintf_s(buffer, "##AnimationTypeBox%d", i + 1);

						ImGui::Text("%s", itemStrings[currentType].c_str());
					}

					ImGui::EndTable();
				}
				ImGui::Text("Jump Fixed Frame: %d", a->jump_fixed_frame);

				if (ImGui::Button("Edit values")) {
					Animation_EditMode = true;
				}
			}
		}
	}

	// Text
	if (t != nullptr) {
		if (ImGui::CollapsingHeader("Text")) {
			if (Text_EditMode)
			{
				// Display input fields
				LE_InputText("Text#TextInText", edited_text, 1000);
				LE_InputFloat("Font Size", &edited_font_size);

				// Button to exit edit mode
				if (ImGui::Button("Done##Text"))
				{
					Text_EditMode = false;
					t->text = edited_text;
					t->fontSize = edited_font_size;
				}

				ImGui::SameLine();

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.f, 0.f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.f, 0.f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.f, 0.f, 1.f));
				if (ImGui::Button("Cancel##Text"))
				{
					Text_EditMode = false;
				}
				ImGui::PopStyleColor(3);
			}
			else
			{
				// Display the values as text
				ImGui::Text("Text: %s", t->text.c_str());
				ImGui::Text("Font Size: %.5f", t->fontSize);

				// Button to enter edit mode
				if (ImGui::Button("Edit##Text"))
				{
					Text_EditMode = true;
					strcpy_s(edited_text, sizeof(edited_text), t->text.c_str());
					edited_font_size = t->fontSize;
				}
			}
		}
	}

	// Transform
	if (tr != nullptr) {
		if (ImGui::CollapsingHeader("Transform")) {
			if (Transform_EditMode)
			{
				// Display input fields
				LE_InputFloat2("Position", &(edited_position.x));
				LE_InputFloat("Rotation", &edited_rotation);
				LE_InputFloat2("Scale", &(edited_scale.x));

				// Button to exit edit mode
				if (ImGui::Button("Done##Transform"))
				{
					Transform_EditMode = false;
					tr->Position = edited_position;
					tr->Rotation = edited_rotation;
					tr->Scale = edited_scale;

					if (bo != nullptr)
						RecalculateBody(tr, bo);
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

				ImGui::SameLine();

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.f, 0.f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.f, 0.f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.f, 0.f, 1.f));
				if (ImGui::Button("Delete##Transform"))
				{
					objectFactory->DeleteComponent(object, ComponentType::Transform);
					tr = nullptr;
				}
				ImGui::PopStyleColor(3);
			}
		}
	}

	// Body
	if (bo != nullptr) {
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
					bo->active = edited_active;
					bo->collision_response = edited_collision_response;

					UpdateAllObjectInstances(object);
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
				bo->active ? ImGui::Text("true") : ImGui::Text("false");

				ImGui::Text("Respond to collision: ");
				ImGui::SameLine();
				bo->collision_response ? ImGui::Text("true") : ImGui::Text("false");

				// Button to enter edit mode
				if (ImGui::Button("Edit##Body"))
				{
					Body_EditMode = true;
					edited_active = bo->active;
					edited_collision_response = bo->collision_response;
				}

				ImGui::SameLine();

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.f, 0.f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.f, 0.f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.f, 0.f, 1.f));
				if (ImGui::Button("Delete##Body"))
				{
					objectFactory->DeleteComponent(object, ComponentType::Body);
					bo = nullptr;
					UpdateAllObjectInstances(object);
				}
				ImGui::PopStyleColor(3);
			}

			if (bo != nullptr && bo->GetShape() == Shape::Rectangle) {
				Rectangular* r = (Rectangular*)bo;

				ImGui::SeparatorText("AABB Collision Settings");

				if (AABB_EditMode)
				{
					ImGui::Text("All values will apply to all instances of %s", object->GetName().c_str());
					// Display input fields
					LE_InputFloat("AABB Width", &edited_aabb_width);
					LE_InputFloat("AABB Height", &edited_aabb_height);
					ImGui::Checkbox("Pushable", &edited_pushable);

					const char* materials[] = { "Concrete", "Metal" };
					ImGui::Combo("Material", (int*)&edited_material, materials, IM_ARRAYSIZE(materials));

					// Button to exit edit mode
					if (ImGui::Button("Done##AABB"))
					{
						AABB_EditMode = false;
						r->width = edited_aabb_width;
						r->height = edited_aabb_height;
						r->pushable = edited_pushable;
						r->material = edited_material;

						UpdateAllObjectInstances(object);

						SceneManager::CalculateLevelSize();
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

					ImGui::Text("Pushable: ");
					ImGui::SameLine();
					r->pushable ? ImGui::Text("true") : ImGui::Text("false");

					ImGui::Text("Material: ");
					ImGui::SameLine();
					switch (r->material) {
					case Material::Concrete:
						ImGui::Text("Concrete");
						break;
					case Material::Metal:
						ImGui::Text("Metal");
						break;
					}

					// Button to enter edit mode
					if (ImGui::Button("Edit##AABB"))
					{
						AABB_EditMode = true;
						edited_aabb_width = r->width;
						edited_aabb_height = r->height;
						edited_pushable = r->pushable;
						edited_material = r->material;
					}
				}

				ImGui::SeparatorText("AABB Collision");

				/*** ROW 1 ***/

				/* P3 */

				ImGui::BeginChild("P3", ImVec2(ImGui::GetContentRegionAvail().x * 0.333333f, 40.f));

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

				ImGui::BeginChild("CollisionTop", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 40.f));

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

				ImGui::BeginChild("P2", ImVec2(0.f, 40.f));

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

				ImGui::BeginChild("P0", ImVec2(ImGui::GetContentRegionAvail().x * 0.333333f, 40.f));

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

				ImGui::BeginChild("CollisionBottom", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 40.f));

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

				ImGui::BeginChild("P1", ImVec2(0.f, 40.f));

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
				LE_InputFloat2("Velocity", &(edited_velocity.x));
				ImGui::Text("All values below will apply to all instances of %s", object->GetName().c_str());
				LE_InputFloat("Mass", &(edited_mass));
				ImGui::Checkbox("Affected by gravity: ", &edited_gravity);
				ImGui::Checkbox("Able to push objects: ", &edited_able_to_push_objects);

				// Button to exit edit mode
				if (ImGui::Button("Done##Physics"))
				{
					Physics_EditMode = false;
					ph->Velocity = edited_velocity;
					ph->Mass = edited_mass;
					ph->AffectedByGravity = edited_gravity;
					ph->AbleToPushObjects = edited_able_to_push_objects;

					UpdateAllObjectInstances(object);
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
				ImGui::Text("Mass: %.5f", ph->Mass);

				ImGui::Text("Affected by gravity: ");
				ImGui::SameLine();
				ph->AffectedByGravity ? ImGui::Text("true") : ImGui::Text("false");

				ImGui::Text("Able to push objects: ");
				ImGui::SameLine();
				ph->AbleToPushObjects ? ImGui::Text("true") : ImGui::Text("false");

				ImGui::Text("IsBeingPushed: ");
				ImGui::SameLine();
				ph->IsBeingPushed ? ImGui::Text("true") : ImGui::Text("false");

				// Button to enter edit mode
				if (ImGui::Button("Edit##Physics"))
				{
					Physics_EditMode = true;
					edited_velocity = ph->Velocity;
					edited_mass = ph->Mass;
					edited_gravity = ph->AffectedByGravity;
					edited_able_to_push_objects = ph->AbleToPushObjects;
				}

				ImGui::SameLine();

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.f, 0.f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.f, 0.f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.f, 0.f, 1.f));
				if (ImGui::Button("Delete##Physics"))
				{
					objectFactory->DeleteComponent(object, ComponentType::Physics);
					ph = nullptr;
					UpdateAllObjectInstances(object);
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

	// Event
	if (e != nullptr) {
		if (ImGui::CollapsingHeader("Event")) {
			ImGui::Text("Linked Event : %d", e->linked_event);
		}
	}

	// Behaviour
	if (be != nullptr) {
		if (ImGui::CollapsingHeader("Behaviour")) {
			ImGui::Text("Script Name : %s", be->GetBehaviourName().c_str());
			ImGui::Text("Script Index : %d", be->GetBehaviourIndex());
			if (be->GetBehaviourCounter() > 0)
				ImGui::Text("Script Counter : %d", be->GetBehaviourCounter());
			if (be->GetBehaviourDistance() > 0)
				ImGui::Text("Script Distance : %d", be->GetBehaviourDistance());
			if (be->GetBehaviourSpeed() > 0)
				ImGui::Text("Script Speed : %d", be->GetBehaviourSpeed());
			if (ImGui::CollapsingHeader("Change Scripts")) {
				ImGui::OpenPopup("Select Scripts");
				for (auto& it : Logic->behaviours)
					if (ImGui::Selectable(it.first.c_str())) {
						be->SetBehaviourName(it.first.c_str());
						be->SetBehaviourIndex(0);
						UpdateAllObjectInstances(object);
					}

			}
			if (ImGui::Button("Delete Behaviour"))
			{
				Logic->RemoveBehaviourComponent(be);
				objectFactory->DeleteComponent(object->GetId(), ComponentType::Behaviour);
				std::cout << "Updated Map Size : " << Logic->behaviourComponents.size() << std::endl;
				UpdateAllObjectInstances(object);
			}
		}
	}


	ImGui::EndChild();
	ImGui::End();
}

/******************************************************************************
	ListOfObjects
	- This window lists down the the objects that are in the scene
	- Selecting an object will display it's properties in the Object Properties window
*******************************************************************************/
void LevelEditor::ListOfObjects() {

	ImGui::Begin("Object List");
	ImGui::Text("Number of game objects in level: %d", objectFactory->NumberOfObjects());
	if (ImGui::Button("Create new layer")) {
		std::string layerName = std::string("Layer " + static_cast<char>(sceneManager->layers.size()));
		//buffer
		sprintf_s(buffer, "Layer %d", static_cast<int>(sceneManager->layers.size()));
		objectFactory->CreateLayer(std::string(buffer), true, true);
	}
	if (ImGui::Button("Create empty object")) {
		ImGui::OpenPopup("CreateEmptyObject");

	}

	if (ImGui::BeginPopup("CreateEmptyObject"))
	{
		ImGui::Text("Select layer to insert the new object to:");
		for (auto& l : SceneManager::layers) {
			if (ImGui::Selectable(l.first.c_str())) {
				Object* o = objectFactory->createEmptyObject();
				selectedNum = o->GetId();
				cloneSuccessful = selectedNum;
				l.second.second.push_back(o);
			}
		}

		ImGui::EndPopup();
	}
	ImGui::BeginChild("ObjectListScroll", ImGui::GetContentRegionAvail());
	if (ImGui::BeginTable("ObjectList", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
	{

		for (auto& l : SceneManager::layers) {
			ImGui::TableNextColumn();
			char buf[512];
			sprintf_s(buf, "##%s_%s", engine->loaded_level.c_str(), l.first.c_str());
			ImGui::Checkbox(buf, &l.second.first.isVisible);
			ImGui::SameLine();

			if (l.second.first.isInherited)
				sprintf_s(buf, "%s (inherited from %s)##%s_%s", l.first.c_str(), l.second.first.inheritedJsonName.c_str(), engine->loaded_level.c_str(), l.first.c_str());
			else
				sprintf_s(buf, "%s##%s_%s", l.first.c_str(), engine->loaded_level.c_str(), l.first.c_str());

			if (ImGui::TreeNode(buf)) {
				// For all objects in the layer
				for (auto& object : l.second.second) {
					if (object->GetName().empty())
						sprintf_s(buf, "%d) Object", static_cast<int>(object->GetId()));
					else
						sprintf_s(buf, "%d) %s", static_cast<int>(object->GetId()), object->GetName().c_str());

					// Creating button for each object
					if (ImGui::Selectable(buf, selectedNum == static_cast<int>(object->GetId()))) {
						selected = true;
						selectedNum = static_cast<int>(object->GetId());

						// Cancel all edits inside the property editor
						Transform_EditMode = false;
						Body_EditMode = false;
						AABB_EditMode = false;
						Physics_EditMode = false;
					}
				}
				ImGui::TreePop();
			}
		}
		/*
		for (size_t i = 0; i < objectFactory->NumberOfObjects(); i++)
		{

			if (objectFactory->getObjectWithID((long)i) == nullptr)
			{
				continue;
			}
			ImGui::TableNextColumn();
			Object* object = objectFactory->getObjectWithID(static_cast<int>(i));
			char buf[256];
			if (object->GetName().empty())
				sprintf_s(buf, "%d) Object", static_cast<int>(i));
			else
				sprintf_s(buf, "%d) %s", static_cast<int>(i), object->GetName().c_str());

			// Creating button for each object
			if (ImGui::Selectable(buf, selectedNum == static_cast<int>(i))) {
				selected = true;
				selectedNum = static_cast<int>(i);

				// Cancel all edits inside the property editor
				Transform_EditMode = false;
				Body_EditMode = false;
				AABB_EditMode = false;
				Physics_EditMode = false;
			}
		}
		*/
		ImGui::EndTable();
	}
	ImGui::EndChild();
	ImGui::End();
}

/******************************************************************************
	DisplaySelectedTexture
	- This window displays the texture that is selected in the Asset List window
*******************************************************************************/
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

		ImVec2 image_size((float)width, (float)height);

		if (width > window->width * 0.3) {
			image_size = ImVec2((float)window->width * 0.3f, (float)height * ((float)window->width * 0.3f / (float)width));
		}
		else if (height > window->height * 0.3) {
			image_size = ImVec2(((float)width * ((float)window->height * 0.3f / (float)height)), (float)window->height * 0.3f);
		}


		ImGui::SetNextWindowSize(ImVec2(image_size.x + 20, image_size.y + 70), ImGuiCond_Always);

		ImGui::Begin(selectedTexture.first.c_str(), &display_selected_texture);

		//ImVec2 windowSize = ImGui::GetWindowSize();
		//ImVec2 displaySize = ImVec2(windowSize.x, windowSize.y);

		ImGui::Image((void*)(intptr_t)selectedTexture.second, image_size);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.f, 0.f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.f, 0.f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.f, 0.f, 1.f));

		if (ImGui::Button("Delete Texture"))
		{
			display_selected_texture = false;
			AssetManager::unloadtexture(selectedTexture.first);

			bool result = std::filesystem::remove("Asset/Picture/" + selectedTexture.first);

			if (result) {
				std::cout << "Texture deleted successfully." << std::endl;
			}
			else {
				std::cout << "Texture delete failed." << std::endl;
			}
		}
		ImGui::PopStyleColor(3);

		ImGui::End();
	}
}

static bool selectingAudio = false;
static bool selectingTexture = false;
static std::string SelectedAudioType;

/******************************************************************************
	AssetList
	- This window displays the lists of assets that are loaded by the engine
*******************************************************************************/
void LevelEditor::AssetList()
{
	ImGui::Begin("Asset List");

	if (ImGui::BeginTabBar("##AssetList"))
	{
		if (ImGui::BeginTabItem("Textures"))
		{
			if (ImGui::Button("Add Texture")) {
				selectingTexture = true;
			}

			ImGui::SameLine();

			if (ImGui::Button("Refresh Textures"))
			{
				AssetManager::unloadalltextures();
				AssetManager::loadalltextures();

			}
			ImGui::BeginChild("AssetListScrollTexture", ImGui::GetContentRegionAvail());
			ImVec2 button_size = ImVec2(ImGui::GetWindowSize().x - style->ScrollbarSize, 64);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 1.f));
			for (const std::pair<std::string, GLuint>& t : AssetManager::textures)
			{
				sprintf_s(buffer, "##%s", t.first.c_str());

				// Start the invisible button

				if (ImGui::Button(buffer, button_size))
				{
					selectedTexture = t;
					display_selected_texture = true;

				}

				if (ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload("Game texture", t.first.c_str(), 1024);
					ImGui::EndDragDropSource();
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
	if (ImGui::BeginTabItem("Animations")) {
		ImGui::Text("wip");

		ImGui::BeginChild("AssetListScrollAnimation", ImGui::GetContentRegionAvail());
		ImVec2 button_size = ImVec2(ImGui::GetWindowSize().x - style->ScrollbarSize, 64);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 1.f));

		for (const std::pair<std::string, GLuint>& a : AssetManager::animations)
		{
			sprintf_s(buffer, "##%s", a.first.c_str());

			// Start the invisible button

			if (ImGui::Button(buffer, button_size))
			{
				
			}

			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("Game animations", a.first.c_str(), 1024);
				ImGui::EndDragDropSource();
			}

			ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y - 68));

			// Image
			ImGui::Image((void*)(intptr_t)a.second, ImVec2(64, 64));

			// Move to the right of the image without moving to a new line
			ImGui::SameLine();

			// Text
			ImGui::Text(a.first.c_str());
		}

		ImGui::PopStyleColor();
		ImGui::EndChild();
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Prefabs"))
	{
		if (ImGui::Button("Create Prefab")) {
			new_prefab_dialog = true;
		}

		ImGui::SameLine();

		if (ImGui::Button("Save Prefabs")) {
			for (auto& prefab : AssetManager::prefabs)
				objectFactory->saveObject(prefab.first, prefab.second);
		}

		ImVec2 button_size = ImVec2(ImGui::GetWindowSize().x, 64);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 1.f));

		for (const std::pair<std::string, Object*>& p : AssetManager::prefabs)
		{
			//i--;

			sprintf_s(buffer, "##%s", p.first.c_str());

			// Text and images will be in the above layer
			if (ImGui::Button(buffer, button_size))
			{
				selected = true;
				//selectedNum = i;
				selectedNum = (int)-(std::distance(AssetManager::prefabs.begin(), AssetManager::prefabs.find(p.first))) - 1;

				// Cancel all edits inside the property editor
				Transform_EditMode = false;
				Body_EditMode = false;
				AABB_EditMode = false;
				Physics_EditMode = false;
			}
			size_t size = sizeof(p);
			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("Game object", &p, size);
				ImGui::EndDragDropSource();
			}

			ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y - 68));

			// Get texture or animation
			Texture* t = (Texture*)p.second->GetComponent(ComponentType::Texture);
			Animation* a = (Animation*)p.second->GetComponent(ComponentType::Animation);

			// Image
			if (t != nullptr)
				ImGui::Image((void*)(intptr_t)AssetManager::textureval(t->textureName), ImVec2(64, 64));
			// or Animation
			else if (a != nullptr)
			{
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
			// There is neither
			else {
				sprintf_s(buffer, "%s#NoTexOrAnim", p.second->GetName().c_str());

				ImGui::BeginChild(buffer, ImVec2(64.f, 64.f));

				ImGui::Text("This object has");
				ImGui::Text("no texture or");
				ImGui::Text("animations");

				ImGui::EndChild();
			}

			// Move to the right of the image without moving to a new line
			ImGui::SameLine();

			// Text
			ImGui::Text(p.second->GetName().c_str());

		}

		ImGui::PopStyleColor();
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Scripts"))
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 1.f));
		for (const auto& script : Logic->behaviours)
		{
			ImGui::Text(script.first.c_str());
		}
		ImGui::PopStyleColor();
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Audio"))
	{

		if (ImGui::Button("Add Audio")) {
			ImGui::OpenPopup("AddToWhichSound");
		}

		if (ImGui::BeginPopup("AddToWhichSound")) {
			ImGui::Text("Add to:");
			ImGui::Separator();
			ImGui::EndPopup();
		}


		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 1.f));
		for (const auto& audioPair : AssetManager::soundMapping)
		{
			std::string AudioTypeString = audioPair.first;

			if (ImGui::BeginPopup("AddToWhichSound")) {
				if (ImGui::Selectable(AudioTypeString.c_str())) {
					SelectedAudioType = AudioTypeString;
					selectingAudio = true;
				}
				ImGui::EndPopup();
			}

			if (ImGui::TreeNode(AudioTypeString.c_str())) {
				if (std::holds_alternative<std::string>(audioPair.second)) {
					if (!std::get<std::string>(audioPair.second).empty()) {
						if (ImGui::Selectable(std::get<std::string>(audioPair.second).c_str())) {
							ImGui::OpenPopup("SoundOption");
						}

						if (ImGui::BeginPopup("SoundOption")) {
							if (ImGui::MenuItem("Delete Audio")) {
								DeleteSound(AudioTypeString, 0);
							}
							ImGui::EndPopup();
						}
					}
				}
				else {
					std::vector<std::string> v = std::get<std::vector<std::string>>(audioPair.second);
					int audioIndex = 0;
					static int audioIndexSelected = -1;
					for (const std::string& s : v) {
						if (ImGui::Selectable(s.c_str())) {
							audioIndexSelected = audioIndex;
							ImGui::OpenPopup("SoundOption");
						}
						audioIndex++;
					}
					if (ImGui::BeginPopup("SoundOption")) {
						if (ImGui::MenuItem("Delete Audio")) {
							DeleteSound(AudioTypeString, audioIndexSelected);
						}
						ImGui::EndPopup();
					}
				}
				ImGui::TreePop();
			}
		}

		ImGui::PopStyleColor();
		ImGui::EndTabItem();

	}
	ImGui::EndTabBar();
	ImGui::End();
}

/******************************************************************************
	PlayPauseGame
	- This window contains buttons that allows a user to play/pause the game.
	- This window also has a reset button that allows the user to reset the level
	  back to it's initial state before the user hits play for the first time.
*******************************************************************************/
void LevelEditor::PlayPauseGame() {
	ImGui::Begin("Play/Pause");

	// Make the buttons unclickable if Finn or Spark are not inside 
	//ImGui::BeginDisabled(objectFactory->FindObject("Finn") == nullptr || objectFactory->FindObject("Spark") == nullptr);

	if (engine->isPaused()) {
		if (ImGui::Button("Play")) {
			SceneManager::PlayScene();
			if (camera2D->isFreeCamEnabled()) {
				camera2D->scale = { 1.f, 1.f };
				camera2D->position = { 0.f, 0.f };
				camera2D->toggleFreeCam();
			}
		}
	}
	else {
		if (ImGui::Button("Pause"))
			SceneManager::PauseScene();
	}

	//ImGui::EndDisabled();

	ImGui::SameLine();


	ImGui::BeginDisabled(!engine->isPaused() || SceneManager::IsInitialObjectMapEmpty());

	if (ImGui::Button("Reset")) {
		SceneManager::RestartScene();
	}

	ImGui::EndDisabled();

	ImGui::End();
}

/******************************************************************************
	CameraControl
	- This window is used to toggle the camera between the game camera and the free camera.
	- The free camera allows the user to move it in all directions
*******************************************************************************/
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

	if (ImGui::Button("Reset to Player##CameraControl")) { camera2D->SetToPlayer(); }

	ImGui::SameLine();

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 50.f, ImGui::GetCursorPosY()));

	ImGui::Button("Zoom In##CameraControl");
	if (ImGui::IsItemActive()) { camera2D->scale *= 1.f + (engine->GetDt() * 1.5f); }

	ImGui::SameLine();

	ImGui::Button("Zoom Out##CameraControl");
	if (ImGui::IsItemActive()) { camera2D->scale /= 1.f + (engine->GetDt() * 1.5f); }

	ImGui::SameLine();

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 50.f, ImGui::GetCursorPosY()));

	if (ImGui::Button("Reset Zoom##CameraControl")) { camera2D->scale = { 1.f, 1.f }; }

	if (!camera2D->isFreeCamEnabled()) {
		// End the disabled section
		ImGui::EndDisabled();

		// Restore original style
		//ImGui::PopStyleVar();
	}

	ImGui::End();
}

/******************************************************************************
	LoadLevelPanel
	- This window lets users select a level to load
	- All levels are in json files and stored in Asset/Levels
*******************************************************************************/
void LevelEditor::LoadLevelPanel() {

	std::vector<std::string> level_files;
	const std::string path = "Asset/Levels/";

	try {
		for (const auto& entry : std::filesystem::directory_iterator(path)) {
			if (entry.is_regular_file() && entry.path().extension() == ".json") {
				level_files.push_back(entry.path().filename().string());
			}
		}
	}
	catch (std::filesystem::filesystem_error& e) {
		std::cerr << e.what() << std::endl;
	}

	ImGui::Begin("Level List");
	ImGui::Text("Number of levels detected: %d", level_files.size());
	ImGui::BeginChild("LevelListScroll", ImGui::GetContentRegionAvail());
	if (ImGui::BeginTable("LevelList", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
	{
		// Print the filenames
		for (const auto& filename : level_files) {
			ImGui::TableNextColumn();
			if (ImGui::Selectable(filename.c_str())) {
				if (!engine->isPaused())
					//engine->setPause();
					SceneManager::PauseScene();
				selected = false;
				sceneManager->LoadScene(filename);
			}
		}

		ImGui::EndTable();
	}
	ImGui::EndChild();
	ImGui::End();
}

/******************************************************************************
	ObjectClonedSuccessfully
	- This pop up window tells that the object has cloned successfully
*******************************************************************************/
void ObjectClonedSuccessfully(int i) {
	ImGui::SetNextWindowPos(ImVec2((float)window->width / 2.f, (float)window->height / 2.f));
	ImGui::SetNextWindowSize(ImVec2(0, 0));
	char text[50];

	if (cloneSuccessful != -1)
		ImGui::OpenPopup("Clone Successful");

	if (ImGui::BeginPopupModal("Clone Successful", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
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
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

/******************************************************************************
	SaveAsDialog
	- This pop up a dialog box to ask where to save the level to
*******************************************************************************/
void LevelEditor::SaveAsDialog() {
	ImGui::SetNextWindowPos(ImVec2((float)window->width / 2.f - 150, (float)window->height / 2.f));
	ImGui::SetNextWindowSize(ImVec2(300, 0));
	static char text[100];

	if (save_as_dialog)
		ImGui::OpenPopup("Save as");

	if (ImGui::BeginPopupModal("Save as", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

		ImGui::Text("Save to file:");
		LE_InputText("##Filename", text, 100);

		ImGui::SameLine();

		ImGui::Text(".json");

		if (ImGui::Button("OK"))
		{
			if (!initialObjectMap.empty()) {
				objectFactory->destroyAllObjects();

				for (const std::pair<int, Object*>& p : initialObjectMap) {
					objectFactory->assignIdToObject(p.second);
				}

				SceneManager::CalculateLevelSize();
				SceneManager::ClearInitialObjectMap(false);
			}

			//char saveLocation[110];

			//sprintf_s(saveLocation, "Asset/Levels/%s.json", text);

			std::string filename(text);
			filename += ".json";

			SaveScene(filename);
			save_as_dialog = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			save_as_dialog = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

/******************************************************************************
	NewPrefabDialog
	- This pop up a dialog box to ask the name of the new prefab
*******************************************************************************/
void LevelEditor::NewPrefabDialog() {
	ImGui::SetNextWindowPos(ImVec2((float)window->width / 2.f - 150, (float)window->height / 2.f));
	ImGui::SetNextWindowSize(ImVec2(300, 0));
	static char text[100];

	if (new_prefab_dialog)
		ImGui::OpenPopup("New Prefab");

	if (ImGui::BeginPopupModal("New Prefab", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

		ImGui::Text("Name of this new prefab:");
		LE_InputText("##NewPrefabName", text, 100);

		if (ImGui::Button("OK"))
		{
			Object* prefab = objectFactory->createEmptyPrefab();
			prefab->SetName(text);
			AssetManager::prefabs.emplace(text + std::string(".json"), prefab);
			new_prefab_dialog = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			new_prefab_dialog = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

/******************************************************************************
	UpdateAllObjectInstances
	- This function will update all instances of the given object

	@param object - The object to reference when updating. All objects with the
					same name (including prefabs) will be updated
*******************************************************************************/
void LevelEditor::UpdateAllObjectInstances(Object* object) {

	// Make sure the object is from a prefab and the update_all_instances flag is set
	if (object->GetPrefab() == nullptr || !update_all_instances)
		return;

	// Do not copy Transform
	Texture* te = (Texture*)object->GetComponent(ComponentType::Texture);
	Body* bo = (Body*)object->GetComponent(ComponentType::Body);
	Physics* ph = (Physics*)object->GetComponent(ComponentType::Physics);
	Animation* a = (Animation*)object->GetComponent(ComponentType::Animation);
	Event* e = (Event*)object->GetComponent(ComponentType::Event);
	Behaviour* be = (Behaviour*)object->GetComponent(ComponentType::Behaviour);

	// Get all instances of the object
	std::vector<Object*> v = objectFactory->FindAllObjectsByName(object->GetName());

	// Get the prefab too
	v.push_back(AssetManager::prefabsval(object->GetName() + ".json"));

	for (Object* o : v) {
		if (o != object) { // Make sure it's not the same object as the reference
			Texture* o_te = (Texture*)o->GetComponent(ComponentType::Texture);
			Body* o_bo = (Body*)o->GetComponent(ComponentType::Body);
			Physics* o_ph = (Physics*)o->GetComponent(ComponentType::Physics);
			Animation* o_a = (Animation*)o->GetComponent(ComponentType::Animation);
			Event* o_e = (Event*)o->GetComponent(ComponentType::Event);
			Behaviour* o_be = (Behaviour*)o->GetComponent(ComponentType::Behaviour);

			// Check to see if a component needs to be deleted

			// Transform object is not supposed to be deleted at this time
			if (te == nullptr && o_te != nullptr)
				objectFactory->DeleteComponent(o, ComponentType::Texture);
			if (bo == nullptr && o_bo != nullptr)
				objectFactory->DeleteComponent(o, ComponentType::Body);
			if (ph == nullptr && o_ph != nullptr)
				objectFactory->DeleteComponent(o, ComponentType::Physics);
			if (a == nullptr && o_a != nullptr)
				objectFactory->DeleteComponent(o, ComponentType::Animation);
			if (e == nullptr && o_e != nullptr)
				objectFactory->DeleteComponent(o, ComponentType::Event);
			if (be == nullptr && o_be != nullptr)
				objectFactory->DeleteComponent(o, ComponentType::Behaviour);

			if (te != nullptr && o_te == nullptr) {
				o->AddComponent(new Texture(te->textureName));
				o_te = (Texture*)o->GetComponent(ComponentType::Texture);
			}
			if (bo != nullptr && o_bo == nullptr) {
				o->AddComponent(new Rectangular());
				o_bo = (Body*)o->GetComponent(ComponentType::Body);
			}
			if (ph != nullptr && o_ph == nullptr) {
				o->AddComponent(new Physics());
				o_ph = (Physics*)o->GetComponent(ComponentType::Physics);
			}
			if (be != nullptr && o_be == nullptr) {
				o->AddComponent(new Behaviour(be->GetBehaviourIndex(), be->GetBehaviourName()));
				o_be = (Behaviour*)o->GetComponent(ComponentType::Behaviour);
			}
			if (a != nullptr && o_a == nullptr) {
				o->AddComponent(new Animation());
				o_a = (Animation*)o->GetComponent(ComponentType::Animation);
			}

			// Check to see if we are not copying the data to components that doesn't exist
			if (te != nullptr && o_te != nullptr) {
				o_te->textureName = te->textureName;
			}
			if (bo != nullptr && o_bo != nullptr) {
				// Only copy the collision data. Don't copy the active or collision response flags 
				if (bo->GetShape() == Shape::Rectangle) {
					Rectangular* r = (Rectangular*)bo;
					Rectangular* o_r = (Rectangular*)o_bo;

					o_r->aabb = r->aabb;
					o_r->width = r->width;
					o_r->height = r->height;
					o_r->pushable = r->pushable;

					o_r->Initialize();
				}
				else if (bo->GetShape() == Shape::Circle) {
					Circular* c = (Circular*)bo;
					Circular* o_c = (Circular*)o_bo;

					o_c->circle = c->circle;
				}
				else if (bo->GetShape() == Shape::Line) {
					Lines* l = (Lines*)bo;
					Lines* o_l = (Lines*)o_bo;

					o_l->line = l->line;
				}
			}
			if (ph != nullptr && o_ph != nullptr) {
				// Do not copy velocity
				o_ph->AffectedByGravity = ph->AffectedByGravity;
				o_ph->Mass = ph->Mass;
				o_ph->AbleToPushObjects = ph->AbleToPushObjects;
			}
			if (a != nullptr && o_a != nullptr) {
				o_a->animation_tex_obj = a->animation_tex_obj;
				o_a->animation_Map = a->animation_Map;
				o_a->frame_rate = a->frame_rate;
				o_a->animation_scale = a->animation_scale;
			}
			if (e != nullptr && o_e != nullptr) {
				o_e->linked_event = e->linked_event;
			}
			if (be != nullptr && o_be != nullptr) {
				o_be->SetBehaviourIndex(be->GetBehaviourIndex());
				o_be->SetBehaviourName(be->GetBehaviourName());
			}
		}
	}
}

/******************************************************************************
	DoNothing
	- Just a stub function. It does nothing
*******************************************************************************/
void DoNothing() {

}

/******************************************************************************
	Initialize
	- Initializes the level editor
*******************************************************************************/
void LevelEditor::Initialize() {
	total_time = 0.0;

	style = &ImGui::GetStyle();

	//style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	//style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	//style->ItemSpacing = ImVec2(12, 8);
	//style->ItemInnerSpacing = ImVec2(8, 6);
	//style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	//style->GrabMinSize = 5.0f;
	//style->GrabRounding = 3.0f;

	Vec4 NormalizeColor(255.f, 255.f, 255.f, 1.f);

	Vec4 DisabledModifier(0.5f, 0.5f, 0.5f, 1.f);

	Vec4 HoveredModifier = Vec4(32.f, 32.f, 32.f, 1.f) / NormalizeColor;
	Vec4 ActiveModifier = Vec4(-64.f, -64.f, -64.f, 1.f) / NormalizeColor;

	//Vec4 ShadowModifier(0.8f, 0.8f, 0.8f, 0.8f);

	Vec4 BorderModifier(1.2f, 1.2f, 1.2f, 1.f);

	Vec4 TextColor = Vec4(255.f, 255.f, 255.f, 1.f) / NormalizeColor;
	Vec4 BackgroundColor = Vec4(32.f, 32.f, 32.f, 1.f) / NormalizeColor;
	Vec4 FrameColor = Vec4(8.f, 8.f, 8.f, 1.f) / NormalizeColor;
	Vec4 BaseColor = Vec4(96.f, 96.f, 96.f, 1.f) / NormalizeColor;
	Vec4 ButtonColor = Vec4(128.f, 128.f, 128.f, 1.f) / NormalizeColor;

	Vec4 ProgressBarColor = Vec4(66.f, 150.f, 250.f, 1.f) / NormalizeColor;


	// Text
	style->Colors[ImGuiCol_Text] = TextColor.ToImVec4();
	style->Colors[ImGuiCol_TextDisabled] = Vec4(TextColor * DisabledModifier).ToImVec4();

	// ImGui window backgrounds
	style->Colors[ImGuiCol_WindowBg] = BackgroundColor.ToImVec4();
	style->Colors[ImGuiCol_ChildBg] = BackgroundColor.ToImVec4();
	style->Colors[ImGuiCol_PopupBg] = BackgroundColor.ToImVec4();

	// Title bars
	style->Colors[ImGuiCol_TitleBg] = Vec4(ButtonColor * DisabledModifier).ToImVec4();
	style->Colors[ImGuiCol_TitleBgActive] = ButtonColor.ToImVec4();
	style->Colors[ImGuiCol_TitleBgCollapsed] = Vec4(ButtonColor * DisabledModifier).ToImVec4();

	// Scroll bar
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.f, 0.f, 0.f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = BaseColor.ToImVec4();
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = Vec4(BaseColor + HoveredModifier).ToImVec4();
	style->Colors[ImGuiCol_ScrollbarGrabActive] = Vec4(BaseColor + ActiveModifier).ToImVec4();

	// Progress bar
	style->Colors[ImGuiCol_PlotHistogram] = ProgressBarColor.ToImVec4();

	/*
	// ImGui window borders
	style->Colors[ImGuiCol_Border] = Vec4(BackgroundColor * BorderModifier).ToImVec4();
	//style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	//style->Colors[ImGuiCol_BorderShadow] = Vec4(BackgroundColor * ShadowModifier).ToImVec4();

	// Checkbox, radio button, plot, slider, text input
	style->Colors[ImGuiCol_FrameBg] = FrameColor.ToImVec4();
	style->Colors[ImGuiCol_FrameBgHovered] = Vec4(FrameColor * HoveredModifier).ToImVec4();
	style->Colors[ImGuiCol_FrameBgActive] = Vec4(FrameColor * ActiveModifier).ToImVec4();

	//style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	//style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	//style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = Vec4(BackgroundColor + Vec4(0.2f, 0.2f, 0.2f, 0.f)).ToImVec4();;

	// Default buttons
	style->Colors[ImGuiCol_Button] = ButtonColor.ToImVec4();
	style->Colors[ImGuiCol_ButtonHovered] = Vec4(ButtonColor * HoveredModifier).ToImVec4();
	style->Colors[ImGuiCol_ButtonActive] = Vec4(ButtonColor * ActiveModifier).ToImVec4();

	// Tabs
	style->Colors[ImGuiCol_Tab] = ButtonColor.ToImVec4();
	style->Colors[ImGuiCol_TabHovered] = Vec4(ButtonColor * HoveredModifier).ToImVec4();
	style->Colors[ImGuiCol_TabActive] = Vec4(ButtonColor * ActiveModifier).ToImVec4();



	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

	*/

	ImGuiIO& io = ImGui::GetIO();
	font = io.Fonts->AddFontFromFileTTF("Asset/Fonts/Roboto-Regular.ttf", 15);

	io.Fonts->Build();

	objectFactory->CreateLayer("Layer 0", true, true);
}

/******************************************************************************
	Update
	- The update loop for the level editor
*******************************************************************************/
void LevelEditor::Update() {

	// Reset flag for every loop
	input::LevelEditorTextActive = false;

	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::PushFont(font);

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

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			char savetext[108];
			sprintf_s(savetext, "Save to %s", engine->loaded_filename.c_str());

			if (ImGui::MenuItem(savetext)) {
				if (!initialObjectMap.empty()) {
					objectFactory->destroyAllObjects();

					for (const std::pair<int, Object*>& p : initialObjectMap) {
						objectFactory->assignIdToObject(p.second);
					}
					SceneManager::CalculateLevelSize();
					SceneManager::ClearInitialObjectMap(false);
				}

				SaveScene(engine->loaded_filename.c_str());
			}
			if (ImGui::MenuItem("Save as...")) {
				save_as_dialog = true;
			}
			ImGui::Separator();

			if (ImGui::MenuItem("Exit")) { engine->game_active = false; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("TileSet")) { engine->show_tileset = true; }
			if (ImGui::MenuItem("Uniform Grid")) { showUniformGrid = true; }
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	showUniformGrid ? DebugUniformGrid() : DoNothing();

	DebugPerformanceViewer();

	ListOfObjects();

	ObjectProperties();

	AssetList();

	DisplaySelectedTexture();

	PlayPauseGame();

	CameraControl();

	LoadLevelPanel();

	if (cloneSuccessful > -1) {
		ObjectClonedSuccessfully(cloneSuccessful);
	}

	save_as_dialog ? SaveAsDialog() : DoNothing();
	new_prefab_dialog ? NewPrefabDialog() : DoNothing();
	selectingTexture ? AddTexture() : DoNothing();
	selectingAudio ? AddAudio() : DoNothing();

	ImGui::PopFont();

	if (input::IsPressed(KEY::del) && selectedNum >= 0) {
		objectFactory->destroyObject(objectFactory->getObjectWithID(selectedNum));
		selectedNum = -1;
		selected = false;
	}
}

/******************************************************************************
	These are basically helper functions that encapsulates the actual ImGui
	text inputs. These functions will disable the game's input system whenever the
	text boxes are in focus so that the game doesn't trigger whenever something
	is getting typed in those boxes. ImGui itself will not be affected as it has it's
	own input system seperate from the engine
*******************************************************************************/

void LevelEditor::LE_InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags) {
	ImGui::InputText(label, buf, buf_size, flags);

	if (!input::LevelEditorTextActive)
		input::LevelEditorTextActive = ImGui::IsItemActive();
}

void LevelEditor::LE_InputInt(const char* label, int* v) {
	ImGui::InputInt(label, v);

	if (!input::LevelEditorTextActive)
		input::LevelEditorTextActive = ImGui::IsItemActive();
}

void LevelEditor::LE_InputFloat(const char* label, float* v) {
	ImGui::InputFloat(label, v);

	if (!input::LevelEditorTextActive)
		input::LevelEditorTextActive = ImGui::IsItemActive();
}

void LevelEditor::LE_InputFloat2(const char* label, float* v) {
	ImGui::InputFloat2(label, v);

	if (!input::LevelEditorTextActive)
		input::LevelEditorTextActive = ImGui::IsItemActive();
}

/******************************************************************************
DeleteSound
- This function deletes the sound from the AssetManager and the file system
*******************************************************************************/
void LevelEditor::DeleteSound(std::string audioType, int audio_num) {
	try {
		auto& sound = AssetManager::soundMapping.at(audioType);

		std::string audioName;

		if (std::holds_alternative<std::string>(sound)) {

			audioName = std::get<std::string>(sound);

			if (std::get<std::string>(sound) == audio->current_background_audio) {
				audio->stopBackground();
				audio->current_background_audio.clear();
			}

			audio->deleteSound(AssetManager::soundsval(std::get<std::string>(sound)));
			AssetManager::sounds.erase(std::get<std::string>(sound));
			std::get<std::string>(sound).clear();
		}
		else {
			std::vector<std::string>& v = std::get<std::vector<std::string>>(sound);

			audioName = v[audio_num];

			if (v[audio_num] == audio->current_background_audio) {
				audio->stopBackground();
				audio->current_background_audio.clear();
			}

			AssetManager::sounds.erase(v[audio_num]);
			v.erase(v.begin() + audio_num);

			if (v.size() <= 1) {
				AssetManager::soundMapping[audioType] = v[0];
			}
		}

		bool result = std::filesystem::remove("Asset/Sounds/looping/" + audioName) || std::filesystem::remove("Asset/Sounds/non looping/" + audioName);

		if (result) {
			std::cout << "Audio deleted successfully." << std::endl;
		}
		else {
			std::cout << "Audio deleted failed." << std::endl;
		}
	}
	catch (std::out_of_range) {
		return;
	}
}

static bool SelectTextureRunning = false;
static bool SelectAudioRunning = false;
static std::future<std::string> futurePath;

/******************************************************************************
AddTexture
- This function adds a texture to the AssetManager and the file system
*******************************************************************************/
void LevelEditor::AddTexture() {
	if (SelectTextureRunning) {
		ImGui::OpenPopup("Selecting texture");
	}

	if (ImGui::BeginPopupModal("Selecting texture", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

		ImGui::Text("Selecting texture");

		if (!SelectTextureRunning)
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}

	if (!SelectTextureRunning) {
		futurePath = thread_pool->enqueue(&OpenFileDialog, 0);
		SelectTextureRunning = true;
	}
	else if (futurePath.valid()) {
		if (futurePath.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
			// The future is ready
			try {
				std::string texturepath = futurePath.get();; // Retrieve the result
				std::cout << "Texture path: " << texturepath << std::endl;

				if (texturepath.empty()) {
					selectingTexture = false;
					SelectTextureRunning = false;
					return;
				}

				std::filesystem::copy_file(texturepath, "Asset/Picture/" + std::filesystem::path(texturepath).filename().string(), std::filesystem::copy_options::overwrite_existing);

				AssetManager::loadtexture(std::filesystem::path(texturepath).filename().string());

				selectingTexture = false;
				SelectTextureRunning = false;
			}
			catch (const std::exception& e) {
				std::cout << "Exception: " << e.what() << std::endl;
				selectingTexture = false;
				SelectTextureRunning = false;
			}
		}
	}

}

/******************************************************************************
AddAudio
- This function adds an audio to the AssetManager and the file system
*******************************************************************************/
void LevelEditor::AddAudio() {

	if (SelectAudioRunning) {
		ImGui::OpenPopup("Selecting audio");
	}

	if (ImGui::BeginPopupModal("Selecting audio", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

		ImGui::Text("Selecting audio");

		if (!SelectAudioRunning)
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}

	if (!SelectAudioRunning) {
		futurePath = thread_pool->enqueue(&OpenFileDialog, 1);
		SelectAudioRunning = true;
	}
	else if (futurePath.valid()) {
		if (futurePath.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
			// The future is ready
			try {
				std::string audiopath = futurePath.get();; // Retrieve the result
				std::cout << "Audio path: " << audiopath << std::endl;

				if (audiopath.empty()) {
					selectingAudio = false;
					SelectAudioRunning = false;
					return;
				}

				unsigned int sound_ms = 0;

				FMOD::Sound* selectedAudio;

				audio->createSound(audiopath, FMOD_DEFAULT, &selectedAudio);
				selectedAudio->getLength(&sound_ms, FMOD_TIMEUNIT_MS);

				if (sound_ms / 1000 < 10)
					std::filesystem::copy_file(audiopath, "Asset/Sounds/non looping/" + std::filesystem::path(audiopath).filename().string(), std::filesystem::copy_options::overwrite_existing);
				else {
					std::filesystem::copy_file(audiopath, "Asset/Sounds/looping/" + std::filesystem::path(audiopath).filename().string(), std::filesystem::copy_options::overwrite_existing);
					selectedAudio->setMode(FMOD_LOOP_NORMAL);
				}
				AssetManager::sounds.emplace(std::filesystem::path(audiopath).filename().string(), selectedAudio);

				auto& a = AssetManager::soundMapping.at(SelectedAudioType);

				if (std::holds_alternative<std::string>(a)) {
					if (std::get<std::string>(a).empty()) {
						std::get<std::string>(a) = std::filesystem::path(audiopath).filename().string();
					}
					else {
						a = std::vector<std::string>({ std::get<std::string>(a) });
					}
				}
				if (std::holds_alternative<std::vector<std::string>>(a)) {
					std::get<std::vector<std::string>>(a).push_back(std::filesystem::path(audiopath).filename().string());
				}

				selectingAudio = false;
				SelectAudioRunning = false;
			}
			catch (const std::exception& e) {
				std::cout << "Exception: " << e.what() << std::endl;
				selectingAudio = false;
				SelectAudioRunning = false;
			}
		}
	}
}

/******************************************************************************
OpenFileDialog
- This function opens a dialog box to select a file
*******************************************************************************/
std::string LevelEditor::OpenFileDialog(int type) {
	OPENFILENAME ofn;       // Common dialog box structure
	char szFile[260] = { 0 }; // Buffer for file name

	// Initialize OPENFILENAME structure
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetConsoleWindow();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";

	switch (type) {
	case 0:
		ofn.lpstrFilter = "Image Files\0*.PNG;*.JPG;*.JPEG;*.BMP\0";
		break;
	case 1:
		ofn.lpstrFilter = "Audio Files\0*.MP3;*.WAV;*.AAC;*.FLAC;*.OGG;*.WMA;*.M4A\0";
		break;
	default:
		break;
	}

	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	// Display the Open dialog box 
	if (GetOpenFileName(&ofn) == TRUE) {
		return std::string(ofn.lpstrFile);
	}

	return std::string(); // No file selected or dialog cancelled
}

/************************************LEVEL EDITOR GRID************************************/

LevelEditorGrid* editor_grid;

/******************************************************************************
	Default Constructor for LevelEditorGrid
*******************************************************************************/
LevelEditorGrid::LevelEditorGrid()
{
	set_num({ 12, 12 });
	editor_grid = this;
}

glm::vec3 box_color_editor{ 0.0f, 0.5f, 0.5f };

/******************************************************************************
	drawleveleditor
	- Draws the level editor grid
*******************************************************************************/
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

			AssetManager::shaderval("shape").Use();
			// bind VAO of this object's model

			glBindVertexArray(AssetManager::modelval("square").vaoid);
			// copy object's model-to-NDC matrix to vertex shader's
			// uniform variable uModelToNDC
			AssetManager::shaderval("shape").SetUniform("uModel_to_NDC", mat_test.ToGlmMat3());
			AssetManager::shaderval("shape").SetUniform("uColor", box_color_editor);
			// call glDrawElements with appropriate arguments
			glDrawElements(AssetManager::modelval("square").primitive_type, AssetManager::modelval("square").draw_cnt, GL_UNSIGNED_SHORT, 0);

			// unbind VAO and unload shader program
			glBindVertexArray(0);
			AssetManager::shaderval("shape").UnUse();
			Vec2 botleft = { (i - num.x / 2) * box_size, (j - num.y / 2) * box_size };
			Vec2 topright = { botleft.x + box_size,botleft.y + box_size };
			app->drawline(Vec2(topright.x, botleft.y), botleft, glm::vec3{ 0.0f, 1.0f, 1.0f });
			app->drawline(topright, Vec2(topright.x, botleft.y), glm::vec3{ 0.0f, 1.0f, 1.0f });
			app->drawline(topright, Vec2(botleft.x, topright.y), glm::vec3{ 0.0f, 1.0f, 1.0f });
			app->drawline(Vec2(botleft.x, topright.y), botleft, glm::vec3{ 0.0f, 1.0f, 1.0f });
		}
	}
}