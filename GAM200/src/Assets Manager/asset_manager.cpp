/* !
@file	asset_manager.cpp
@author	Tay Sen Chuan (t.senchuan@digipen.edu)
@date	28/9/2023

This file contains the definition of class AssetManager. It will look through all the folder
where the assets are stored and load them, before storing them into a std::map container
to be referenced from when needed.

*//*__________________________________________________________________________*/
#include "asset_manager.h"
#include "../../include/Audio.h"
#include <Windows.h>
#include <iostream>
#include <random>

// Creating of static data members
std::filesystem::path AssetManager::pathtexture = "Asset/Picture";
std::filesystem::path AssetManager::pathanimations = "Asset/Animation";
std::filesystem::path AssetManager::objectprefabs = "Asset/Objects";
std::filesystem::path AssetManager::pathaudio = "Asset/Sounds";
std::filesystem::path AssetManager::pathfonts = "Asset/Fonts";

std::map<std::string, GLuint> AssetManager::textures;
std::map<std::string, GLuint> AssetManager::animations;
std::map<std::string, Object*> AssetManager::prefabs;
std::map<std::string, FMOD::Sound*> AssetManager::sounds;
std::map<AudioType, std::variant<std::string, std::vector<std::string>>> AssetManager::soundMapping;

GLuint AssetManager::missing_texture;

// Looked through the asset file and load all assets
void AssetManager::Initialize()
{
	std::cout << "File List: " << std::endl;

	if (std::filesystem::exists(pathtexture))
	{
		for (const auto& list : std::filesystem::directory_iterator(pathtexture))
		{
			std::cout << list.path() << std::endl;
		}
		loadalltextures();
	}
	else
		std::cout << pathtexture << " does not exist!" << std::endl;

	if (std::filesystem::exists(pathanimations))
	{
		for (const auto& list : std::filesystem::directory_iterator(pathanimations))
		{
			std::cout << list.path() << std::endl;
		}
		loadanimations();
	}
	else
		std::cout << pathanimations << " does not exist!" << std::endl;

	// Load the placeholder for missing textures
	missing_texture = GLApp::setup_texobj("Asset/missing_texture.png");

	// Create a list of object prefabs, that will be used for scene loading
	if (std::filesystem::exists(objectprefabs))
	{
		createprefablist();
	}
	else
		std::cout << objectprefabs << " does not exist!" << std::endl;


	if (std::filesystem::exists(pathaudio))
	{
		loadsounds();
	}
	else
		std::cout << pathaudio << " does not exist!" << std::endl;

}

void AssetManager::Free()
{
	// No freeing needed for textures and animations
	cleanprefab();
	// Freeing of sound is called in audio.cpp when audio system is destroyed
}

std::string AssetManager::SystemName()
{
	return "AssetManager";
}

void AssetManager::Update()
{
	
}

// Load all textures
void AssetManager::loadalltextures()
{
	GLuint textureuint;
	for (const auto& list : std::filesystem::directory_iterator(pathtexture))
	{
		textureuint = GLApp::setup_texobj(list.path().string().c_str());
		std::filesystem::path filename = list.path().filename();
		textures.emplace(filename.string(), textureuint);
		std::cout << "Added to list: " << filename.string() << std::endl;
	}
}

// Unload all textures
void AssetManager::unloadalltextures()
{
	for (const auto& t : textures) {
		glDeleteTextures(1, &t.second);
	}
	textures.clear();
}

void AssetManager::loadanimations()
{
	GLuint textureuint;
	for (const auto& list : std::filesystem::directory_iterator(pathanimations))
	{
		textureuint = GLApp::setup_texobj(list.path().string().c_str());
		std::filesystem::path filename = list.path().filename();
		animations.emplace(filename.string(), textureuint);
		std::cout << "Added to list: " << filename.string() << std::endl;
	}
}

void AssetManager::createprefablist()
{
	for (const auto& list : std::filesystem::directory_iterator(objectprefabs))
	{
		if (list.path().extension() == ".json")
		{
			std::filesystem::path filename = list.path().filename();
			prefabs.emplace(filename.string(), objectFactory->createObject(list.path().string()));
			std::cout << "Added to list: " << filename.string() << std::endl;
		}
		else
		{
			std::cout << "Error located here: " << list.path().filename() << std::endl;
			MessageBoxA(0, "Invalid file extension for prefabs detected. Please only use .json", "Error: Prefabs", MB_OK);
			exit(EXIT_FAILURE);
		}
	}
}

void AssetManager::loadsounds()
{
	for (const auto& list : std::filesystem::directory_iterator(pathaudio))
	{
		std::cout << "Folders: " << list.path().filename() << std::endl;
		FMOD_MODE audio_mode;

		if (list.is_directory()) // Only loads audio files within the sub-directory (e.g. looping)
		{
			if (list.path().filename() == "looping")
				audio_mode = FMOD_LOOP_NORMAL;
			else
				audio_mode = FMOD_DEFAULT;

			auto inneraudio = std::filesystem::directory_entry(list);
			for (const auto& innerlist : std::filesystem::directory_iterator(inneraudio))
			{
				if (innerlist.path().extension() == ".wav")
				{
					std::filesystem::path filename = innerlist.path().filename();
					FMOD::Sound* sound;
					audio->createSound(innerlist.path().string(), audio_mode, &sound);
					sounds.emplace(filename.string(), sound);
					std::cout << "Added to list: " << filename.string() << std::endl;
				}
				else
				{
					// Popup for invalid audio file
					std::cout << "Error located here: " << innerlist.path().filename() << std::endl;
					MessageBoxA(0, "Invalid file extension for audio detected. Please only use .wav", "Error: Audio", MB_OK);
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	audio->setupSound();
}

void AssetManager::clearsounds()
{
	for (const auto& s : sounds)
		s.second->release();
	sounds.clear();
}

bool AssetManager::texturecheckexist(std::string str)
{
	std::map<std::string, GLuint>::iterator it = textures.find(str);
	if (it == textures.end())
		return false;
	else
		return true;
}

GLuint AssetManager::textureval(std::string str)
{
	try {
		return textures.at(str);
	}
	catch (std::out_of_range) {
		return missing_texture;
	}
}

bool AssetManager::animationcheckexist(std::string str)
{
	std::map<std::string, GLuint>::iterator it = animations.find(str);
	if (it == animations.end())
		return false;
	else
		return true;
}

GLuint AssetManager::animationval(std::string str)
{
	return animations[str];
}

Object* AssetManager::prefabsval(std::string str)
{
	try {
		return prefabs.at(str);
	}
	catch (std::out_of_range) {
		return nullptr;
	}
}

//Returns a game prefab from the map with a specific id
Object* AssetManager::prefabById(long id)
{
	auto it = prefabs.begin();
	std::advance(it, -(id + 1));
	return it->second;
}

void AssetManager::updateprefab(std::string str, Object* o)
{
	prefabs[str] = o;
}

std::string AssetManager::objectprefabsval()
{
	return objectprefabs.string();
}

void AssetManager::cleanprefab()
{
	for (const auto& p : prefabs)
		delete p.second;

	prefabs.clear();
}

FMOD::Sound* AssetManager::soundsval(std::string str)
{
	try {
		return sounds.at(str);
	}
	catch (std::out_of_range) {
		return nullptr;
	}
}

FMOD::Sound* AssetManager::getsoundbyaudiotype(AudioType a, bool random, int seq_num) {
	try {
		auto& sound = soundMapping.at(a);

		if (std::holds_alternative<std::string>(sound)) {
			return soundsval(std::get<std::string>(sound));
		}
		else {
			std::vector<std::string> v = std::get<std::vector<std::string>>(sound);
			if (random) {
				// Rng the audio file to play
				std::random_device rd;
				std::mt19937 gen(rd());

				std::uniform_int_distribution<> dis(0, v.size() - 1);

				return soundsval(v[dis(gen)]);
			}
			else {
				return soundsval(v[seq_num]);
			}
		}
	}
	catch (std::out_of_range) {
		return nullptr;
	}
}

void AssetManager::addtextures(std::string str, GLuint tex)
{
	textures.emplace(str, tex);
}

void AssetManager::updateSoundMap(AudioType a, std::string file)
{
	soundMapping.emplace(a, file);
}

void AssetManager::clearSoundMap()
{
	soundMapping.clear();
}






