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

#include <Font.h>

class AssetManager : public ISystems
{
public:
	AssetManager() {};
	~AssetManager() { Free(); }
	virtual void Initialize();
	virtual void Free();
	virtual void Update();
	virtual std::string SystemName();

	// Setup of the assets
	static void loadtexture(const std::string& textureName);
	static void loadalltextures();
	static void unloadtexture(const std::string& textureName);
	static void unloadalltextures();
	
	static void loadanimation(const std::string& animationName);
	static void unloadanimation(const std::string& animationName);
	void loadallanimations();

	void createprefablist();
	void loadsounds();
	static void clearsounds();

	void loadshaders();
	static void addshader(std::string, GLSLShader);
	void freeshader();
	static GLSLShader shaderval(std::string);

	void loadmodels();
	static void addmodel(std::string, GLApp::GLModel);
	void freemodel();
	static GLApp::GLModel modelval(std::string);
	static bool modelexist(std::string);

	void loadcutscenes();
	static std::vector<std::pair<GLuint, std::string>> cutsceneval(std::string);

	void loadfonts();
	void unloadfonts();



	// Access value of animation and textures
	static bool texturecheckexist(std::string str);
	static GLuint textureval(std::string str);

	static bool animationcheckexist(std::string str);
	static GLuint animationval(std::string str);
	static std::string animationstring(GLuint uint);

	// Get prefab objects and related data
	static Object* prefabsval(std::string str);
	static Object* prefabById(long id);
	static void updateprefab(std::string str, Object* o);

	static std::string objectprefabsval();
	static void cleanprefab();

	// Get value of sounds
	static FMOD::Sound* soundsval(std::string str);
	static FMOD::Sound* getsoundbyaudiotype(std::string audioType, bool random = true, int seq_num = 0);

	// Adding sounds into soundmap and clearing them
	static void updateSoundMap(std::string audioType, std::string);
	static void updateSoundMap(std::string audioType, std::vector<std::string>);
	static void clearSoundMap();

	// Add textures while the game is running (defaults to missing texture if no textures are provided)
	static void addtextures(std::string str, GLuint tex = missing_texture);

	//Get the font outline/face
	static outline* getoutline(std::string);

	// Level Editor will need to access the private data
	friend class LevelEditor;
	friend class Factory;

	// These data shouldn't be modified unless file location is changed
private:
	// Location initialize in asset_manager.cpp
	static std::filesystem::path pathtexture;
	static std::filesystem::path pathanimations;
	static std::filesystem::path objectprefabs;
	static std::filesystem::path pathaudio;
	static std::filesystem::path pathfonts;
	static std::filesystem::path pathshaders;
	static std::filesystem::path pathmodels;
	static std::filesystem::path pathcutscene;

	// Asset Manager private data
	static std::map<std::string, GLuint> textures;
	static std::map<std::string, GLuint> animations;
	static std::map<std::string, Object*> prefabs;
	static std::map<std::string, FMOD::Sound*> sounds;
	static std::map<std::string, GLSLShader> shaders;
	static std::map<std::string, GLApp::GLModel> models;
	static std::map<std::string, std::vector<std::pair<GLuint, std::string>>> cutscenes;
	static std::map<std::string, outline> font_outlines;

	// std::string if there is only 1 audio mapped to a sound type.
	// vector of strings otherwise if there is more then 1
	static std::map<std::string, std::variant<std::string, std::vector<std::string>>> soundMapping;

	static GLuint missing_texture;
};