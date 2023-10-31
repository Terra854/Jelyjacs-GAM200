/* !
@file	asset_manager.cpp
@author	Tay Sen Chuan (t.senchuan@digipen.edu)
@date	28/9/2023

This file contains the definition of class AssetManager. It will look through all the folder
where the assets are stored and load them, before storing them into a std::map container
to be referenced from when needed.

*//*__________________________________________________________________________*/
#include "asset_manager.h"
#include <iostream>

// Creating of static data members
std::filesystem::path AssetManager::pathtexture = "Asset/Picture";
std::filesystem::path AssetManager::pathanimations = "Asset/Animation";
std::filesystem::path AssetManager::objectprefabs = "Asset/Objects";

std::map<std::string, GLuint> AssetManager::textures;
std::map<std::string, GLuint> AssetManager::animations;
std::map<std::string, Object*> AssetManager::prefabs;

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
		loadtextures();
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
		std::cout << pathtexture << " does not exist!" << std::endl;

	// Load the placeholder for missing textures
	missing_texture = GLApp::setup_texobj("Asset/missing_texture.png");

	// Create a list of object prefabs, that will be used for scene loading
	std::cout << "Prefab object list: " << std::endl;
	if (std::filesystem::exists(objectprefabs))
	{
		createprefablist();
	}
	else
		std::cout << objectprefabs << " does not exist!" << std::endl;


}

void AssetManager::Free()
{
	for (const std::pair<std::string, Object*> &p : prefabs)
		delete p.second;
}

std::string AssetManager::SystemName()
{
	return "AssetManager";
}

void AssetManager::Update()
{
	
}

// Load all textures
void AssetManager::loadtextures()
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
		std::filesystem::path filename = list.path().filename();
		prefabs.emplace(filename.string(), objectFactory->createObject(list.path().string()));
		std::cout << "Added to list: " << filename.string() <<  std::endl;
	}
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
	prefabs.clear();
}

void AssetManager::addtextures(std::string str, GLuint tex)
{
	textures.emplace(str, tex);
}










