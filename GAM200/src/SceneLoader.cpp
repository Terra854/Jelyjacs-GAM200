/* !
@file	SceneLoader.cpp
@author	Tay Sen Chuan
@date	

This file contains the definitions for loading scenes
*//*__________________________________________________________________________*/

#include <Debug.h>
#include "Assets Manager/asset_manager.h"
#include "Assets Manager/json_serialization.h"
#include "../include/components/Event.h"
#include "SceneLoader.h"
#include <iostream>

void LoadScene(std::string filename)
{
	
	// Check if the given file exists
	JsonSerialization jsonobj;
	jsonobj.openFileRead(filename);

	std::string levelname;
	jsonobj.readString(levelname, "SceneName");
	std::cout << "Loading Scene: " << levelname << std::endl;

	for (auto& component : jsonobj.read("Objects"))
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

		if (jsonloop.isMember("linkedevent"))
		{
			Event* event_pt = static_cast<Event*>(obj->GetComponent(ComponentType::Event));
			jsonloop.readInt(event_pt->linked_event, "linkedevent");
		}

		// Add here to read oher types of data if necessary WIP


		obj->Intialize();

	}

	// Save the name of the level to engine to track
	engine->loaded_level = levelname;
	engine->loaded_filename = filename;

	jsonobj.closeFile();
}

void SaveScene(std::string filename)
{
	Json::Value jsonobj;
	jsonobj["SceneName"] = "testsaving";

	for (size_t i = 0; i < objectFactory->NumberOfObjects(); i++)
	{
		Object* obj = objectFactory->getObjectWithID(static_cast<long>(i));

		if (obj == nullptr)
			continue;

		std::string name = obj->GetName() + ".json";
		Json::Value innerobj;

		innerobj["Prefabs"] = name;

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

		if (obj->GetComponent(ComponentType::Event) != nullptr)
		{
			Event* event = static_cast<Event*>(obj->GetComponent(ComponentType::Event));
			innerobj["linkedevent"] = event->linked_event;
		}

		jsonobj["Objects"].append(innerobj);
	}

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











