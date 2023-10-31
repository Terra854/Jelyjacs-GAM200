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
	// Reset value of prefab when loading new scene (prevents wrong cloning due to deleted objs)
	AssetManager::cleanprefab();

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
		// Create object if doesn't exist
		if (AssetManager::prefabsval(objprefabs) == nullptr)
		{
			// Create object
			std::string tempobjprefabs = AssetManager::objectprefabsval() + "/" + objprefabs;
			obj = objectFactory->createObject(tempobjprefabs, false);

			AssetManager::updateprefab(obj->GetName(), obj);

			//AssetManager::updateprefab(objprefabs, obj);
		}
		else
		// clone object
		{
			obj = objectFactory->cloneObject(AssetManager::prefabsval(objprefabs));
		}

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

	jsonobj.closeFile();
}













