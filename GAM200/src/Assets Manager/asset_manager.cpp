/* !
@file
@author
@date	28/9/2023


*//*__________________________________________________________________________*/
#include "asset_manager.h"
#include <iostream>

std::map<std::string, GLuint> textures;

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
		loadassets(); // Will either be moved or turned into texture specific loader
	}
	else
		std::cout << pathtexture << " does not exist!" << std::endl;


}

void AssetManager::Free()
{

}

std::string AssetManager::SystemName()
{
	return "AssetManager";
}

void AssetManager::Update(float dt)
{

}

// Load all assets in the different paths (might be changed to specific type of assets)
void AssetManager::loadassets()
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












