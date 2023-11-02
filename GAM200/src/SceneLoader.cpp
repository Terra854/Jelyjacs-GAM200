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

	jsonobj.closeFile();
}













