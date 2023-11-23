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
#include "../../include/Audio.h"
#include "../../include/Interface_System.h"
#include "../../include/Graphic_gl/glapp.h"
#include <variant>

class AssetManager : public ISystems
{
public:
	AssetManager() {};
	~AssetManager() { Free(); }
	virtual void Initialize();
	virtual void Free();
	virtual void Update();
	virtual std::string SystemName();

	static void loadalltextures();
	static void unloadalltextures();
	
	void loadanimations();
	void createprefablist();
	void loadsounds();
	static void clearsounds();

	// Access value
	static bool texturecheckexist(std::string str);
	static GLuint textureval(std::string str);

	static bool animationcheckexist(std::string str);
	static GLuint animationval(std::string str);

	static Object* prefabsval(std::string str);
	static Object* prefabById(long id);
	static void updateprefab(std::string str, Object* o);

	static std::string objectprefabsval();
	static void cleanprefab();

	static FMOD::Sound* soundsval(std::string str);
	static FMOD::Sound* getsoundbyaudiotype(AudioType a, bool random = false, int seq_num = 0);

	static void updateSoundMap(AudioType a, std::string);
	static void updateSoundMap(AudioType a, std::vector<std::string>);
	static void clearSoundMap();

	// Add textures while the game is running (defaults to missing texture if no textures are provided)
	static void addtextures(std::string str, GLuint tex = missing_texture);

	// Level Editor will need to access the private data
	friend class LevelEditor;

	// These data shouldn't be modified unless file location is changed
private:
	// Location initialize in asset_manager.cpp
	static std::filesystem::path pathtexture;
	static std::filesystem::path pathanimations;
	static std::filesystem::path objectprefabs;
	static std::filesystem::path pathaudio;
	static std::filesystem::path pathfonts;

	// Asset Manager private data
	static std::map<std::string, GLuint> textures;
	static std::map<std::string, GLuint> animations;
	static std::map<std::string, Object*> prefabs;
	static std::map<std::string, FMOD::Sound*> sounds;

	// std::string if there is only 1 audio mapped to a sound type.
	// vector of strings otherwise if there is more then 1
	static std::map<AudioType, std::variant<std::string, std::vector<std::string>>> soundMapping;

	static GLuint missing_texture;
};