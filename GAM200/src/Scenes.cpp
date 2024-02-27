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

/******************************************************************************
LoadSceneFromJson
-	This function reads a json file and loads all the objects

@param filename - the name of the json file
*******************************************************************************/
void LoadSceneFromJson(std::string filename, bool isParentScene)
{
	// Check if the given file exists
	JsonSerialization jsonobj;
	jsonobj.openFileRead(filename);

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

	if (isParentScene) {
		SceneManager::AdditionalScenesLoadedConcurrently.clear();

		Vec2 start_coord;
		jsonobj.readFloat(start_coord.x, "Size", "startX");
		jsonobj.readFloat(start_coord.y, "Size", "startY");
		engine->Set_Start_Coords(start_coord);

		Vec2 level_size;
		jsonobj.readFloat(level_size.x, "Size", "width");
		jsonobj.readFloat(level_size.y, "Size", "height");
		engine->Set_Level_Size(level_size);
	}

	int layerNum = 0;

	for (auto& layer : jsonobj.read("Layers")) {
		std::string layername = layer["Name"].asCString();

		if (!isParentScene)
			layername.append("(inherited from " + filename + ")");

		//jsonobj.readString(layername, "Name");
		LayerSettings ls = { layer["Settings"]["isVisible"].asBool(), layer["Settings"]["static_layer"].asBool(), !isParentScene };

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
			if (AssetManager::prefabsval(objprefabs) == nullptr)
			{
				// Create new prefab
				std::string tempobjprefabs = AssetManager::objectprefabsval() + "/" + objprefabs;
				Object* newPrefab = objectFactory->createObject(tempobjprefabs);

				AssetManager::updateprefab(newPrefab->GetName(), newPrefab);
			}

			// Create object via cloning the prefab
			obj = objectFactory->cloneObject(AssetManager::prefabsval(objprefabs));
			obj->SetPrefab(AssetManager::prefabsval(objprefabs)); // Update the clone object to have usingPrefab value

			// Assign an ID. It will be added to the objectMap
			objectFactory->assignIdToObject(obj);

			// Read extra data to update object
			std::string type;
			jsonloop.readString(type, "Type");

			if (type == "Transform")
			{
				Transform* tran_pt = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));
				jsonloop.readFloat(tran_pt->Position.x, "Position", "x");
				jsonloop.readFloat(tran_pt->Position.y, "Position", "y");
				jsonloop.readFloat(tran_pt->Scale.x, "Scale", "x");
				jsonloop.readFloat(tran_pt->Scale.y, "Scale", "y");
				jsonloop.readFloat(tran_pt->Rotation, "Rotation");
			}

			if (jsonloop.isMember("Name"))
			{
				std::string tempstr;
				jsonloop.readString(tempstr, "Name");
				obj->SetName(tempstr);
			}
			//else
			//	obj->SetName("");
			// ^Default name for objects is the name set in prefabs object list

			if (jsonloop.isMember("Properties"))
			{
				Body* temp = static_cast<Body*>(obj->GetComponent(ComponentType::Body));
				if (temp->GetShape() == Shape::Rectangle)
				{
					Rectangular* temp2 = static_cast<Rectangular*>(temp);
					jsonloop.readFloat(temp2->width, "Properties", "width");
					jsonloop.readFloat(temp2->height, "Properties", "height");
				}
			}

			if (jsonloop.isMember("linkedevent"))
			{
				Event* event_pt = static_cast<Event*>(obj->GetComponent(ComponentType::Event));
				jsonloop.readInt(event_pt->linked_event, "linkedevent");
			}

			if (jsonloop.isMember("scripts"))
			{
				// THIS DOESN'T REALLY WORK FOR CHANGING THE SCRIPTS OF INDIVIDUAL OBJECTS
				std::string behvstr;
				jsonloop.readString(behvstr, "scripts");

				//Behaviour* b = (Behaviour*)(obj->GetComponent(ComponentType::Behaviour));
				//Logic->RemoveBehaviourComponent(b);
				objectFactory->DeleteComponent(obj, ComponentType::Behaviour);

				obj->AddComponent(new Behaviour(0, behvstr));
				//Logic->AddBehaviourComponent((Behaviour*)obj->GetComponent(ComponentType::Behaviour));

				Behaviour* behv = static_cast<Behaviour*>(obj->GetComponent(ComponentType::Behaviour));

				behv->SetBehaviourName(behvstr);
				behv->SetBehaviourIndex(0);
			}

			// Add here to read oher types of data if necessary WIP


			obj->Initialize();

			objectFactory->AddToLayer(layername, obj);
		}

		layerNum++;
	}

	// Save the name of the level to engine to track
	engine->loaded_level = levelname;
	engine->loaded_filename = filename;

	for (auto& additionalScenes : jsonobj.read("AdditionalScenesToLoad"))
	{
		std::string sceneFile = additionalScenes.asCString();
		SceneManager::AdditionalScenesLoadedConcurrently.push_back(sceneFile);
		::LoadSceneFromJson("Asset/Levels/" + sceneFile, false);
	}

	jsonobj.closeFile();

	Logic->Initialize();
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
	jsonobj["SceneName"] = filename;

	//jsonobj["SoundMap"] = "Asset/Sounds/sounds.json"; // Hard coded line, will need to do proper saving

	Vec2 start_coord = engine->Get_Start_Coords();

	Vec2 level_size = engine->Get_Level_Size();

	jsonobj["Size"]["startX"] = start_coord.x;
	jsonobj["Size"]["startY"] = start_coord.y;
	jsonobj["Size"]["width"] = level_size.x;
	jsonobj["Size"]["height"] = level_size.y;

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
			std::string prefabname = "MISSINGNAME";

			if (prefab == nullptr)
				std::cout << "OBJECT: " << name << " is missing usingPrefab!" << std::endl;
			else
				prefabname = prefab->GetName() + ".json";

			Json::Value innerobj;


			innerobj["Name"] = name;
			innerobj["Prefabs"] = prefabname;

			// Save object transform data
			if (obj->GetComponent(ComponentType::Transform) != nullptr)
			{
				Transform* trans = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));
				innerobj["Type"] = "Transform";

				Json::Value position;
				position["x"] = trans->Position.x;
				position["y"] = trans->Position.y;
				innerobj["Position"] = position;

				Json::Value scale;
				scale["x"] = trans->Scale.x;
				scale["y"] = trans->Scale.y;
				innerobj["Scale"] = scale;

				innerobj["Rotation"] = trans->Rotation;
			}

			if (obj->GetComponent(ComponentType::Body) != nullptr)
			{
				Body* temp = static_cast<Body*>(obj->GetComponent(ComponentType::Body));
				if (temp->GetShape() == Shape::Rectangle)
				{
					Rectangular* temp2 = static_cast<Rectangular*>(temp);
					Json::Value properties;
					properties["width"] = temp2->width;
					properties["height"] = temp2->height;
					innerobj["Properties"] = properties;
				}
			}

			// Save objects event data
			if (obj->GetComponent(ComponentType::Event) != nullptr)
			{
				Event* event = static_cast<Event*>(obj->GetComponent(ComponentType::Event));
				innerobj["linkedevent"] = event->linked_event;
			}

			if (obj->GetComponent(ComponentType::Behaviour))
			{
				Behaviour* behv = static_cast<Behaviour*>(obj->GetComponent(ComponentType::Behaviour));
				innerobj["scripts"] = behv->GetBehaviourName();
			}

			layer["Objects"].append(innerobj);
		}

		layers.append(layer);
	}
	/*
	for (size_t i = 0; i < objectFactory->NumberOfObjects(); i++)
	{
		Object* obj = objectFactory->getObjectWithID(static_cast<long>(i));

		if (obj == nullptr)
			continue;

		// Save object prefabs data
		std::string name = obj->GetName();
		Object* prefab = obj->GetPrefab();
		std::string prefabname = "MISSINGNAME";
		
		if (prefab == nullptr)
			std::cout << "OBJECT: " << name << " is missing usingPrefab!" <<std::endl;
		else
			prefabname = prefab->GetName() + ".json";

		Json::Value innerobj;

		
		innerobj["Name"] = name;
		innerobj["Prefabs"] = prefabname;

		// Save object transform data
		if (obj->GetComponent(ComponentType::Transform) != nullptr)
		{
			Transform* trans = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));
			innerobj["Type"] = "Transform";

			Json::Value position;
			position["x"] = trans->Position.x;
			position["y"] = trans->Position.y;
			innerobj["Position"] = position;

			Json::Value scale;
			scale["x"] = trans->Scale.x;
			scale["y"] = trans->Scale.y;
			innerobj["Scale"] = scale;

			innerobj["Rotation"] = trans->Rotation;
		}

		if (obj->GetComponent(ComponentType::Body) != nullptr)
		{
			Body* temp = static_cast<Body*>(obj->GetComponent(ComponentType::Body));
			if (temp->GetShape() == Shape::Rectangle)
			{
				Rectangular* temp2 = static_cast<Rectangular*>(temp);
				Json::Value properties;
				properties["width"] = temp2->width;
				properties["height"] = temp2->height;
				innerobj["Properties"] = properties;
			}
		}

		// Save objects event data
		if (obj->GetComponent(ComponentType::Event) != nullptr)
		{
			Event* event = static_cast<Event*>(obj->GetComponent(ComponentType::Event));
			innerobj["linkedevent"] = event->linked_event;
		}

		if (obj->GetComponent(ComponentType::Behaviour))
		{
			Behaviour* behv = static_cast<Behaviour*>(obj->GetComponent(ComponentType::Behaviour));
			innerobj["scripts"] = behv->GetBehaviourName();
		}

		jsonobj["Objects"].append(innerobj);
	}
	*/

	// Write file
	std::ofstream outputFile(filename);
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
	
	// GETTING ERROR, TEMPORARY NOT USING JSONSERILIZATION.H
	// 
	//JsonSerialization jsonobj;
	//jsonobj.writeData("SceneName", filename);	

	//for (size_t i = 0; i < objectFactory->NumberOfObjects(); i++) 
	//{
	//	Object* obj = objectFactory->getObjectWithID((long)i);

	//	if (obj == nullptr)
	//		continue;

	//	std::string name = obj->GetName() + ".json";
	//	jsonobj.writeArrData("Prefabs", name);

	//	if (obj->GetComponent(ComponentType::Transform) != nullptr)
	//	{
	//		Transform* trans = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));
	//		jsonobj.writeArrData("Type", "Transform");
	//		Json::Value position;
	//		position["x"] = trans->Position.x;
	//		position["y"] = trans->Position.y;
	//		jsonobj.writeArrData("Position", position);
	//		Json::Value scale;
	//		scale["x"] = trans->Scale.x;
	//		scale["y"] = trans->Scale.y;
	//		jsonobj.writeArrData("Scale", scale);
	//		jsonobj.writeArrData("Rotation", trans->Rotation);
	//	}

	//	if (obj->GetComponent(ComponentType::Event) != nullptr)
	//	{
	//		Event* event = static_cast<Event*>(obj->GetComponent(ComponentType::Event));
	//		jsonobj.writeArrData("linkedevent", event->linked_event);
	//	}

	//	jsonobj.appendToArr();
	//}

	//jsonobj.openFileWrite(filename);
	//jsonobj.closeFile();
}









