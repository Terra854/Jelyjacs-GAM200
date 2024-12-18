/* !
@file	Scenes.cpp
@author	Tay Sen Chuan
@date	1/11/2023

This file contains the definitions for loading and saving scenes
*//*__________________________________________________________________________*/

#include <Debug.h>
#include "Assets Manager/asset_manager.h"
#include "Assets Manager/json_serialization.h"
#include "../include/components/Event.h"
#include "Scenes.h"
#include <iostream>
#include "LevelEditor.h"
#include <SceneManager.h>
#include <components/Animation.h>
#include <string>
#include <../Scripts/TimeTaken.h>
#include <components/Dialogue.h>

/******************************************************************************
LoadSceneFromJson
-	This function reads a json file and loads all the objects

@param filename - the name of the json file
*******************************************************************************/
void LoadSceneFromJson(std::string filename, bool isParentScene)
{
	// Check if the given file exists
	JsonSerialization jsonobj;
	jsonobj.openFileRead("Asset/Levels/" + filename);

	std::string levelname;
	jsonobj.readString(levelname, "SceneName");
	std::cout << "Loading Scene: " << levelname << std::endl;

	//AssetManager::clearSoundMap();
	/*if (jsonobj.isMember("SoundMap"))
	{
		std::cout << "Linking sound map..." << std::endl;
		std::string soundmap;
		jsonobj.readString(soundmap, "SoundMap");
		linkSoundMap(soundmap);
	}*/
	
	/*
	std::string soundmap = "Asset/Sounds/sounds.json";
	linkSoundMap(soundmap);

	audio->setupSound();
	*/

		/*
		Vec2 start_coord;
		jsonobj.readFloat(start_coord.x, "Size", "startX");
		jsonobj.readFloat(start_coord.y, "Size", "startY");
		engine->Set_Start_Coords(start_coord);

		Vec2 level_size;
		jsonobj.readFloat(level_size.x, "Size", "width");
		jsonobj.readFloat(level_size.y, "Size", "height");
		engine->Set_Level_Size(level_size);
		*/

	int layerNum = 0;

	for (auto& layer : jsonobj.read("Layers")) {
		std::string layername = layer["Name"].asCString();

		//jsonobj.readString(layername, "Name");
		LayerSettings ls = { layer["Settings"]["isVisible"].asBool(), layer["Settings"]["static_layer"].asBool(), !isParentScene };

		if (!isParentScene)
			ls.inheritedJsonName = levelname;

		objectFactory->CreateLayer(layername, ls);

		//for (auto& component : jsonobj.read("Objects"))
		for (auto& component : layer["Objects"])
		{
			JsonSerialization jsonloop;
			jsonloop.jsonObject = &component;

			std::string objprefabs;
			jsonloop.readString(objprefabs, "Prefabs");
			Object* obj;
			// Create the prefab if it doesn't exist
			if (!objprefabs.empty() && AssetManager::prefabsval(objprefabs) == nullptr)
			{
				// Create new prefab
				std::string tempobjprefabs = AssetManager::objectprefabsval() + "/" + objprefabs;
				Object* newPrefab = objectFactory->createObject(tempobjprefabs);

				AssetManager::updateprefab(newPrefab->GetName(), newPrefab);
			}

			if (AssetManager::prefabsval(objprefabs)) {
				// Create object via cloning the prefab
				obj = objectFactory->cloneObject(AssetManager::prefabsval(objprefabs));
				obj->SetPrefab(AssetManager::prefabsval(objprefabs)); // Update the clone object to have usingPrefab value
				// Assign an ID. It will be added to the objectMap
				objectFactory->assignIdToObject(obj);
			}
			else {
				// Create object via creating a new object
				obj = objectFactory->createEmptyObject();
			}

			if (jsonloop.isMember("Name"))
			{
				std::string tempstr;
				jsonloop.readString(tempstr, "Name");
				obj->SetName(tempstr);
			}

			// Read extra data to update object
			if (jsonloop.isMember("Components"))
				for (auto& objcomponent : jsonloop.read("Components"))
				{
					JsonSerialization objcomponentjson;
					objcomponentjson.jsonObject = &objcomponent;

					std::string type;
					objcomponentjson.readString(type, "Type");

					if (type == "Transform")
					{
						Transform* tran_pt = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));
						
						if (!tran_pt)
						{
							obj->AddComponent(new Transform());
							tran_pt = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));
						}
						
						if (tran_pt) {
							objcomponentjson.readFloat(tran_pt->Position.x, "Position", "x");
							objcomponentjson.readFloat(tran_pt->Position.y, "Position", "y");
							objcomponentjson.readFloat(tran_pt->Scale.x, "Scale", "x");
							objcomponentjson.readFloat(tran_pt->Scale.y, "Scale", "y");
							objcomponentjson.readFloat(tran_pt->Rotation, "Rotation");
						}
					}

					if (type == "Body")
					{
						Body* body_pt = static_cast<Body*>(obj->GetComponent(ComponentType::Body));
						
						if (!body_pt)
						{
							obj->AddComponent(new Rectangular());
							body_pt = static_cast<Body*>(obj->GetComponent(ComponentType::Body));
						}
						
						if (body_pt->GetShape() == Shape::Rectangle)
						{
							Rectangular* temp2 = static_cast<Rectangular*>(body_pt);
							objcomponentjson.readFloat(temp2->width, "Properties", "width");
							objcomponentjson.readFloat(temp2->height, "Properties", "height");
						}
					}

					if (type == "Event")
					{
						Event* event_pt = static_cast<Event*>(obj->GetComponent(ComponentType::Event));
						
						if (!event_pt)
						{
							obj->AddComponent(new Event());
							event_pt = static_cast<Event*>(obj->GetComponent(ComponentType::Event));
						}
						
						objcomponentjson.readInt(event_pt->linked_event, "linkedevent");
					}

					if (type == "Behaviour")
					{
						Behaviour* behv = static_cast<Behaviour*>(obj->GetComponent(ComponentType::Behaviour));

						if (!behv)
						{
							obj->AddComponent(new Behaviour());
							behv = static_cast<Behaviour*>(obj->GetComponent(ComponentType::Behaviour));
						}

						if (objcomponentjson.isMember("scripts"))
						{
							std::string name{};
							objcomponentjson.readString(name, "scripts");
							behv->SetBehaviourName(name);
						}

						if (objcomponentjson.isMember("index"))
						{
							int index{};
							objcomponentjson.readInt(index, "index");
							behv->SetBehaviourIndex(index);
						}

						if (objcomponentjson.isMember("counter"))
						{
							float counter{};
							objcomponentjson.readFloat(counter, "counter");
							behv->SetBehaviourCounter(counter);
						}

						if (objcomponentjson.isMember("speed"))
						{
							float speed{};
							objcomponentjson.readFloat(speed, "speed");
							behv->SetBehaviourSpeed(speed);
						}

						if (objcomponentjson.isMember("distance"))
						{
							float distance{};
							objcomponentjson.readFloat(distance, "distance");
							behv->SetBehaviourDistance(distance);
						}
					}

					if (type == "Texture")
					{
						Texture* tex = static_cast<Texture*>(obj->GetComponent(ComponentType::Texture));

						if (!tex)
						{
							obj->AddComponent(new Texture(""));
							tex = static_cast<Texture*>(obj->GetComponent(ComponentType::Texture));
						}

						objcomponentjson.readString(tex->textureName, "Properties", "texturepath");
						objcomponentjson.readFloat(tex->opacity, "Properties", "opacity");
					}

					if (type == "Animation")
					{
						Animation* ani = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));

						if (!ani)
						{
							obj->AddComponent(new Animation());
							ani = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));
						}

						int tmp{};
						std::string stringtmp{};
						AnimationType anitype;
						std::pair<int, AnimationType> animationframesecond;

						objcomponentjson.readString(stringtmp, "Properties", "texturepath");
						ani->animation_tex_obj = AssetManager::animationval(stringtmp);

						objcomponentjson.readFloat(ani->opacity, "Properties", "opacity");

						objcomponentjson.readInt(ani->jump_fixed_frame, "Properties", "jumpfixframe");
						objcomponentjson.readFloat(ani->frame_rate, "Properties", "framerate");
						objcomponentjson.readBool(ani->face_right, "Properties", "faceright");

						objcomponentjson.readFloat(ani->animation_scale.first, "Properties", "frame", 0);
						objcomponentjson.readFloat(ani->animation_scale.second, "Properties", "frame", 1);

						ani->animation_frame.clear();

						int i{ 1 };
						while (objcomponentjson.isMember("Properties")) {
							std::string key = std::to_string(i);
							if (!objcomponentjson.isMember(key, "Properties")) {
								key = std::to_string(i + 1);
								if (objcomponentjson.isMember(key, "Properties")) 	// If there is skipped key
								{
									i++;
									continue;
								}
								break; // Exit the loop if key does not exist under "Properties"
							}

							objcomponentjson.readInt(tmp, "Properties", key, 0);
							objcomponentjson.readString(stringtmp, "Properties", key, 1);

							for (char& c : stringtmp)
								c = (char) std::tolower(c);

							anitype = stringToAnimationType(stringtmp);

							animationframesecond.first = tmp;
							animationframesecond.second = anitype;

							ani->animation_frame.emplace(i, animationframesecond);
							i++;
						}


						ani->set_up_map(true); // Update map

						// For debug 
						//std::cout << "===============ANI DUMP==============\n";
						//std::cout << "GLuint: " << ani->animation_tex_obj << std::endl;
						//std::cout << "Opacity: " << ani->opacity << std::endl;
						//std::cout << "jumpfixframe: " << ani->jump_fixed_frame << std::endl;
						//std::cout << "framerate: " << ani->frame_rate << std::endl;
						//std::cout << "Faceright: "; if (ani->face_right) { std::cout << "true"; }
						//else { std::cout << "false"; } std::cout << std::endl;

					/*	std::cout << "AnimationRow: " << ani->animation_scale.second << ", AnimationCol: " << ani->animation_scale.first << std::endl;
						for (auto& frame : ani->animation_frame)
						{
							std::cout << "Row: " << frame.first;
							std::cout << ", Col: " << frame.second.first;
							std::cout << ", Type: " << frame.second.second << std::endl;
						}*/
					}

					if (type == "Physics")
					{
						Physics* ph = static_cast<Physics*>(obj->GetComponent(ComponentType::Physics));

						if (!ph)
						{
							obj->AddComponent(new Physics());
							ph = static_cast<Physics*>(obj->GetComponent(ComponentType::Physics));
						}

						objcomponentjson.readFloat(ph->Velocity.x, "X_Velocity");
						objcomponentjson.readFloat(ph->Velocity.y, "Y_Velocity");
						objcomponentjson.readFloat(ph->Mass, "Mass");

						objcomponentjson.readBool(ph->AbleToPushObjects, "AbleToPushObjects");
						objcomponentjson.readBool(ph->AffectedByGravity, "AffectedByGravity");
					}

					if (type == "Dialogue")
					{
						Dialogue* d = static_cast<Dialogue*>(obj->GetComponent(ComponentType::Dialogue));

						if (!d)
						{
							obj->AddComponent(new Dialogue());
							d = static_cast<Dialogue*>(obj->GetComponent(ComponentType::Dialogue));
						}

						std::string tmp;
						objcomponentjson.readString(tmp, "dialogue");
						d->ChangeDialogue(tmp);

					}

					// Add here to read oher types of data if necessary WIP
				}

			obj->Initialize();

			objectFactory->AddToLayer(layername, obj);
		}

		layerNum++;
	}

	if (isParentScene) {
		// Save the name of the level to engine to track
		engine->loaded_level = levelname;
		engine->loaded_filename = filename;
	}

	for (auto& additionalScenes : jsonobj.read("AdditionalScenesToLoad"))
	{
		std::string sceneFile = additionalScenes.asCString();

		if (isParentScene)
			SceneManager::AdditionalScenesLoadedConcurrently.push_back(sceneFile);
		
		::LoadSceneFromJson(sceneFile, false);
	}

	jsonobj.closeFile();

	if (isParentScene) {
		Logic->Initialize();
		SceneManager::CalculateLevelSize();
	}
	WINMENU::StartTime();
}

/******************************************************************************
SaveScene
-	This function reads the list of all the objects and save them into a json file

@param filename - the name of the json file
*******************************************************************************/
void SaveScene(std::string filename)
{
	// Save Scene Name
	Json::Value jsonobj;
	jsonobj["SceneName"] = engine->loaded_level;

	//jsonobj["SoundMap"] = "Asset/Sounds/sounds.json"; // Hard coded line, will need to do proper saving
	/*
	Vec2 start_coord = engine->Get_Start_Coords();

	Vec2 level_size = engine->Get_Level_Size();

	jsonobj["Size"]["startX"] = start_coord.x;
	jsonobj["Size"]["startY"] = start_coord.y;
	jsonobj["Size"]["width"] = level_size.x;
	jsonobj["Size"]["height"] = level_size.y;
	*/

	auto& layers = jsonobj["Layers"];

	for (auto& l : SceneManager::layers) {

		// Do not save the inherited layers
		if (l.second.first.isInherited)
			continue;

		Json::Value layer;

		layer["Name"] = l.first;
		layer["Settings"]["isVisible"] = l.second.first.isVisible;
		layer["Settings"]["static_layer"] = l.second.first.static_layer;

		for (Object* obj : l.second.second) {

			if (obj == nullptr)
				continue;

			// Save object prefabs data
			std::string name = obj->GetName();
			Object* prefab = obj->GetPrefab();
			//std::string prefabname = "MISSINGNAME";
			std::string prefabname;

			if (prefab == nullptr)
				std::cout << "OBJECT: " << name << " is missing usingPrefab!" << std::endl;
			else
				prefabname = prefab->GetName() + ".json";

			Json::Value innerobj;

			innerobj["Name"] = name;
			innerobj["Prefabs"] = prefabname;

			Json::Value components;

			// Save object transform data
			if (obj->GetComponent(ComponentType::Transform) != nullptr)
			{
				Json::Value typedata;
				Transform* trans = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));
				typedata["Type"] = "Transform";

				Json::Value position;
				position["x"] = trans->Position.x;
				position["y"] = trans->Position.y;
				typedata["Position"] = position;

				Json::Value scale;
				scale["x"] = trans->Scale.x;
				scale["y"] = trans->Scale.y;
				typedata["Scale"] = scale;

				typedata["Rotation"] = trans->Rotation;

				components.append(typedata);
			}

			if (obj->GetComponent(ComponentType::Body) != nullptr)
			{
				Json::Value typedata;
				Body* temp = static_cast<Body*>(obj->GetComponent(ComponentType::Body));
				typedata["Type"] = "Body";
				if (temp->GetShape() == Shape::Rectangle)
				{
					typedata["Shape"] = "Rectangle";
					Rectangular* temp2 = static_cast<Rectangular*>(temp);
					Json::Value properties;
					properties["width"] = temp2->width;
					properties["height"] = temp2->height;
					typedata["Properties"] = properties;
				}

				components.append(typedata);
			}

			// Save objects event data
			if (obj->GetComponent(ComponentType::Event) != nullptr)
			{
				Json::Value typedata;
				Event* event = static_cast<Event*>(obj->GetComponent(ComponentType::Event));
				typedata["linkedevent"] = event->linked_event;
				typedata["Type"] = "Event";

				components.append(typedata);
			}

			if (obj->GetComponent(ComponentType::Behaviour))
			{
				Json::Value typedata;
				Behaviour* behv = static_cast<Behaviour*>(obj->GetComponent(ComponentType::Behaviour));
				typedata["scripts"] = behv->GetBehaviourName();
				typedata["index"] = behv->GetBehaviourIndex();
				typedata["counter"] = behv->GetBehaviourCounter();
				typedata["speed"] = behv->GetBehaviourSpeed();
				typedata["distance"] = behv->GetBehaviourDistance();
				typedata["Type"] = "Behaviour";

				components.append(typedata);
			}

			// Save textures
			if (obj->GetComponent(ComponentType::Texture) != nullptr)
			{
				Json::Value typedata;
				Texture* texture = static_cast<Texture*>(obj->GetComponent(ComponentType::Texture));
				typedata["Type"] = "Texture";

				Json::Value texture_;
				texture_["texturepath"] = texture->textureName;
				texture_["opacity"] = texture->opacity;
				typedata["Properties"] = texture_;

				components.append(typedata);
			}

			if (obj->GetComponent(ComponentType::Animation) != nullptr)
			{
				Json::Value typedata;
				Animation* ani = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));
				typedata["Type"] = "Animation";

				Json::Value ani_;
				ani_["jumpfixframe"] = ani->jump_fixed_frame;
				ani_["framerate"] = ani->frame_rate;
				ani_["faceright"] = ani->face_right;
				Json::Value frame;
				frame.append(ani->animation_scale.first);
				frame.append(ani->animation_scale.second);
				ani_["frame"] = frame;

				for (auto& it : ani->animation_frame)
				{
					//if (ani->animation_scale.second < it.first)
					//	break;

					std::string tmp = AnimationTypeToString(it.second.second);

					Json::Value section;
					section.append(it.second.first);
					section.append(tmp); // Watch for caps when doing the load

					ani_[std::to_string(it.first)] = section;
				}

				ani_["texturepath"] = AssetManager::animationstring(ani->animation_tex_obj);
				
				typedata["Properties"] = ani_;

				components.append(typedata);
			}

			// Save objects physics data
			if (obj->GetComponent(ComponentType::Physics) != nullptr)
			{
				Json::Value typedata;
				Physics* ph = static_cast<Physics*>(obj->GetComponent(ComponentType::Physics));
				typedata["X_Velocity"] = ph->Velocity.x;
				typedata["Y_Velocity"] = ph->Velocity.y;

				typedata["Mass"] = ph->Mass;
				typedata["AffectedByGravity"] = ph->AffectedByGravity;
				typedata["AbleToPushObjects"] = ph->AbleToPushObjects;

				typedata["Type"] = "Physics";

				components.append(typedata);
			}

			if (obj->GetComponent(ComponentType::Dialogue) != nullptr)
			{
				Json::Value typedata;
				Dialogue* d = static_cast<Dialogue*>(obj->GetComponent(ComponentType::Dialogue));
				typedata["dialogue"] = d->GetEntireDialogue();

				typedata["Type"] = "Dialogue";

				components.append(typedata);
			}

			innerobj["Components"] = components;
			layer["Objects"].append(innerobj);
		}

		layers.append(layer);
	}

	for (std::string& a : SceneManager::AdditionalScenesLoadedConcurrently)
	{
		jsonobj["AdditionalScenesToLoad"].append(a);
	}

	// Write file
	std::ofstream outputFile("Asset/Levels/" + filename);
	if (outputFile.is_open()) {
		Json::StreamWriterBuilder writer;
		writer["indentation"] = "  ";

		outputFile << Json::writeString(writer, jsonobj);
		outputFile.close();
		std::cout << filename << " has been successfully saved." << std::endl;
		return;
	}
	else
		std::cerr << "Failed to open file for writing." << std::endl;
	
}









