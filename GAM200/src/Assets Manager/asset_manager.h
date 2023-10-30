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
	void loadtextures();
	void loadanimations();
	void createprefablist();

	// Access value
	static bool texturecheckexist(std::string str);
	static GLuint textureval(std::string str);

	static bool animationcheckexist(std::string str);
	static GLuint animationval(std::string str);

	static long prefabsval(std::string str);
	static void updateprefab(std::string str, long val);

	static std::string objectprefabsval();
	static void cleanprefab();

	static GLuint gettexture(std::string);

	// Level Editor will need to access the private data
	friend class LevelEditor;

	// These data shouldn't be modified unless file location is changed
	private:
	// Location initialize in asset_manager.cpp
	static std::filesystem::path pathtexture;
	static std::filesystem::path pathanimations;
	static std::filesystem::path objectprefabs;
	//std::filesystem::path audio;
	//std::filesystem::path fonts;

	// Asset Manager private data
	static std::map<std::string, GLuint> textures;
	static std::map<std::string, GLuint> animations;
	static std::map<std::string, long> prefabs;
};




