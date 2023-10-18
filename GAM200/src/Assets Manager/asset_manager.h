#pragma once
/* !
@file	asset_manager.h
@author	Tay Sen Chuan (t.senchuan@digipen.edu)
@date	28/9/2023

This file contains the declaration of class AssetManager.

*//*__________________________________________________________________________*/
#include <Debug.h>

#include <string>
#include <vector>
#include <filesystem>
#include "../../include/Interface_System.h"
#include "../../include/Graphic_gl/glapp.h"

class AssetManager : public ISystems
{
	public:


	virtual void Initialize();
	virtual void Free();
	virtual void Update();
	virtual std::string SystemName();
	void loadassets();
	void createprefablist();

	// Access value
	GLuint textureval(std::string);
	long prefabsval(std::string);
	void updateprefab(std::string);

	std::string objectprefabsval();

	// These data shouldn't be modified unless file location is changed
	private:
	std::filesystem::path pathtexture = "../Asset/Picture";
	std::filesystem::path objectprefabs = "../Asset/Objects";
	//std::filesystem::path audio;
	//std::filesystem::path fonts;

	// Asset Manager private data
	

};

// Move inside of class
extern std::map<std::string, GLuint> textures;
extern std::map<std::string, long> prefabs;




