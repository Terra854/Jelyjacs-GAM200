/* !
@file	SceneLoader.cpp
@author	Tay Sen Chuan
@date	

This file contains the definitions 
*//*__________________________________________________________________________*/
#include <json/json.h>

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
		if (prefabs[objprefabs] == -1)
		{
			// Create object
			AssetManager temp;
			std::string tempobjprefabs =  temp.objectprefabsval() + "/" + objprefabs;
			obj = objectFactory->createObject(tempobjprefabs);

			prefabs[objprefabs] = obj->GetId();
			std::cout << prefabs[objprefabs];
		}
		else
		// clone object
		{
			obj = objectFactory->cloneObject(objectFactory->getObjectWithID(prefabs[objprefabs]));
		}

		// Read extra data to update object
		std::string type = component["Type"].asString();

		if (type == "Transform")
		{
			Transform* tran_pt = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));
			tran_pt->Position.x = component["Position"]["x"].asFloat();
			tran_pt->Position.y = component["Position"]["y"].asFloat();
		}

		obj->Intialize();

	}
}













