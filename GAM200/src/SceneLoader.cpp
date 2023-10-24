/* !
@file	SceneLoader.cpp
@author	Tay Sen Chuan
@date	

This file contains the definitions 
*//*__________________________________________________________________________*/
#include <json/json.h>

#include <Debug.h>
#include "Assets Manager/asset_manager.h"
#include "SceneLoader.h"
#include <fstream>
#include <iostream>

void LoadScene(std::string filename)
{
	// Check if the given file exists
	std::ifstream jsonFile(filename);
	if (!jsonFile.is_open()) {
		std::cerr << "Could not load scene file: " << filename << std::endl;
		return;
	}

	Json::Value* jsonObject = new Json::Value;;
	Json::Reader reader;

	// Check if the given file is a valid json file
	if (!reader.parse(jsonFile, *jsonObject)) {
		std::cout << "Failed to parse JSON" << std::endl;
		jsonFile.close();
		return;
	}
	jsonFile.close();

	// Now parse the file to populate the object with components

	std::string levelname = (*jsonObject)["SceneName"].asString();
	std::cout << "Loading Scene: " << levelname << std::endl;

	for (const auto& component : (*jsonObject)["Objects"]) 
	{
		std::string objprefabs = component["Prefabs"].asString();
		Object* obj;
		// Create object if doesn't exist
		if (AssetManager::prefabsval(objprefabs) == -1)
		{
			// Create object
			std::string tempobjprefabs = AssetManager::objectprefabsval() + "/" + objprefabs;
			obj = objectFactory->createObject(tempobjprefabs);

			AssetManager::updateprefab(objprefabs, obj->GetId());
		}
		else
		// clone object
		{
			obj = objectFactory->cloneObject(objectFactory->getObjectWithID(AssetManager::prefabsval(objprefabs)));
		}

		// Read extra data to update object
		std::string type = component["Type"].asString();

		if (type == "Transform")
		{
			Transform* tran_pt = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));
			tran_pt->Position.x = component["Position"]["x"].asFloat();
			tran_pt->Position.y = component["Position"]["y"].asFloat();
		}

		// Add here to read oher types of data if necessary WIP


		obj->Intialize();

	}

	delete jsonObject;
}













