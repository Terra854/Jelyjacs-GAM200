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
#include "json_serialization.h"

// Creating of static data members
std::filesystem::path AssetManager::pathtexture = "Asset/Picture";
std::filesystem::path AssetManager::pathanimations = "Asset/Animation";
std::filesystem::path AssetManager::objectprefabs = "Asset/Objects";
std::filesystem::path AssetManager::pathaudio = "Asset/Sounds";
std::filesystem::path AssetManager::pathfonts = "Asset/Fonts";
std::filesystem::path AssetManager::pathshaders = "Asset/shaders";
std::filesystem::path AssetManager::pathmodels = "Asset/meshes";
std::filesystem::path AssetManager::pathcutscene = "Asset/cutscenes";

std::map<std::string, GLuint> AssetManager::textures;
std::map<std::string, GLuint> AssetManager::animations;
std::map<std::string, Object*> AssetManager::prefabs;
std::map<std::string, FMOD::Sound*> AssetManager::sounds;
std::map<std::string, std::variant<std::string, std::vector<std::string>>> AssetManager::soundMapping;
std::map<std::string, GLSLShader> AssetManager::shaders;
std::map<std::string, GLApp::GLModel> AssetManager::models;
std::map<std::string, std::vector<std::pair<GLuint, std::string>>> AssetManager::cutscenes;


GLuint AssetManager::missing_texture;

/******************************************************************************
Initialize
-	Looked through the asset folder and load all assets
*******************************************************************************/
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

	// Create a placeholder for missing textures
	glGenTextures(1, &AssetManager::missing_texture);
	glBindTexture(GL_TEXTURE_2D, AssetManager::missing_texture);

	int width = 128;  // Texture width
	int height = 128; // Texture height
	GLubyte* blackData = new GLubyte[width * height * 3]; // 3 for RGB
	memset(blackData, 0, width * height * 3 * sizeof(GLubyte));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, blackData);

	delete[] blackData;

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
	
	if (std::filesystem::exists(pathshaders))
	{
		loadshaders();
	}
	else
		std::cout << pathshaders << " does not exsit!" << std::endl;
		

	if (std::filesystem::exists(pathmodels))
	{
		loadmodels();
	}
	else
		std::cout << pathmodels << " does not exsit!" << std::endl;

	if (std::filesystem::exists(pathcutscene))
	{
		loadcutscenes();
	}
	else
		std::cout << pathcutscene << " does not exsit!" << std::endl;

}

/******************************************************************************
Free
-	Free the assets stored in cleanprefab as necessary
*******************************************************************************/
void AssetManager::Free()
{
	// No freeing needed for textures and animations
	cleanprefab();

	freeshader();
	freemodel();
	// Freeing of sound is called in audio.cpp when audio system is destroyed
}

/******************************************************************************
SystemName
-	Return the system name
*******************************************************************************/
std::string AssetManager::SystemName()
{
	return "AssetManager";
}

void AssetManager::Update()
{
	
}

/******************************************************************************
loadalltextures
-	The function looks through the directory for textures to load and store
them in the assetmanager
*******************************************************************************/
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

/******************************************************************************
unloadalltextures
-	Unload all the loaded textures stored
*******************************************************************************/
void AssetManager::unloadalltextures()
{
	for (const auto& t : textures) {
		glDeleteTextures(1, &t.second);
	}
	textures.clear();
}

/******************************************************************************
loadanimations
-	The function looks through the directory for animations to load and store
them in the assetmanager
*******************************************************************************/
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

/******************************************************************************
createprefablist
-	The function looks through the directory for prefabs to load and store
them in the assetmanager
*******************************************************************************/
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

/******************************************************************************
loadsounds
-	The function looks through the directory for sounds to load and store
them in the assetmanager
*******************************************************************************/
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
				if (innerlist.path().extension() == ".wav" || innerlist.path().extension() == ".ogg")
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
}

/******************************************************************************
loadshaders
-	The function looks through the directory for shaders to load and store
them in the assetmanager
*******************************************************************************/
void AssetManager::loadshaders()
{
	
	for (const auto& list : std::filesystem::directory_iterator(pathshaders))
	{
		
		std::filesystem::path filename = list.path().filename();
		std::string name = filename.stem().string();

		std::cout << "SHADER NAME TESTING!! == " << name << std::endl;

		auto it = shaders.find(name);
		if (it != shaders.end())
		{
			continue;
		}
		else
		{
			// Filename should always have both vert and frag extensions
			GLApp::insert_shdrpgm(name, pathshaders.string() + "/" + name + ".vert", pathshaders.string() + "/" + name + ".frag");
		}

	}
}

/******************************************************************************
loadmodels
-	The function looks through the directory for models/mesh to load and store
them in the assetmanager
*******************************************************************************/
void AssetManager::loadmodels()
{
	GLApp::GLModel model;
	for (const auto& list : std::filesystem::directory_iterator(pathmodels))
	{
		std::filesystem::path filename = list.path().filename();
		std::string name = filename.stem().string();

		// Ensure only .msh
		if (filename.extension() == ".msh")
		{
			GLApp::insert_models(name);
		}
		else
			continue;
	}
}

/******************************************************************************
loadcutscene
-	The function looks through the directory for cutscenes to load and store
them in the assetmanager
*******************************************************************************/
void AssetManager::loadcutscenes()
{
	GLApp::GLModel model;
	for (const auto& list : std::filesystem::directory_iterator(pathcutscene))
	{
		if (list.is_directory()) // Only loads files within the sub-directory
		{
			std::string cutscenename = list.path().filename().string();
			std::vector <std::pair<GLuint, std::string>> cutscenedata;

			auto innerscene = std::filesystem::directory_entry(list);

			std::vector<std::filesystem::directory_entry> entries;
			std::vector<std::string> description;

			for (const auto& innerlist : std::filesystem::directory_iterator(innerscene))
			{
				// Retrieve the names of the cutscenes first and store them in a vector
				if (innerlist.path().extension() == ".png" || innerlist.path().extension() == ".jpg")
				{
					entries.push_back(innerlist);
				}
				else if (innerlist.path().extension() == ".json") // Access the jsonfile containing the cutscene txt
				{
					JsonSerialization jsonobj;
					jsonobj.openFileRead(innerlist.path().string());

					for (auto& frame : jsonobj.read("Frame"))
					{
						JsonSerialization jsonloop;
						jsonloop.jsonObject = &frame;

						std::string temptext = "";
						jsonloop.readString(temptext, "Text");
						description.push_back(temptext);
					}

					jsonobj.closeFile();
				}
			}

			// Now sort the entries so that we can create an ordered vector for the cutscenes
			std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) 
			{
				return std::stoi(a.path().filename().stem().string()) < std::stoi(b.path().filename().stem().string());
			});

			// Access the sorted vectors to store the ordered textures and text in assetmanager
			int temp{};
			for (const auto& entry : entries) 
			{
				GLuint textureuint;
				textureuint = GLApp::setup_texobj(entry.path().string().c_str()); 

				cutscenedata.push_back(std::make_pair(textureuint, description[temp]));

				temp++;
			}
			// Create the final map
			cutscenes.emplace(cutscenename, cutscenedata);


			// DUMP into console for testing
			for (const auto& frame : cutscenedata)
			{
				std::cout << cutscenename << " | " << frame.first << " | " << frame.second << std::endl;
			}

		}
	}
}

/******************************************************************************
clearsounds
-	The function frees and clear the stored sounds from the assetmanager
*******************************************************************************/
void AssetManager::clearsounds()
{
	for (const auto& s : sounds)
		s.second->release();
	sounds.clear();
}

/******************************************************************************
texturecheckexist
-	Checks if a texture exist in the assetmanager when given the texture name

@param str - the name of the texture

@return - true / false
*******************************************************************************/
bool AssetManager::texturecheckexist(std::string str)
{
	std::map<std::string, GLuint>::iterator it = textures.find(str);
	if (it == textures.end())
		return false;
	else
		return true;
}

/******************************************************************************
textureval
-	Returns the ID for the textures

@param str - the name of the texture

@return - GLuint value of the texture
*******************************************************************************/
GLuint AssetManager::textureval(std::string str)
{
	try {
		return textures.at(str);
	}
	catch (std::out_of_range) {
		return missing_texture;
	}
}

/******************************************************************************
animationcheckexist
-	The function checks whether an animation spirte sheet exist

@param str - the name of the sprite sheet

@return - true / false
*******************************************************************************/
bool AssetManager::animationcheckexist(std::string str)
{
	std::map<std::string, GLuint>::iterator it = animations.find(str);
	if (it == animations.end())
		return false;
	else
		return true;
}

/******************************************************************************
animationval
-	Returns the GLuint for an animation texture

@param str - the name of the animation

@return - the GLuint value of the texture
*******************************************************************************/
GLuint AssetManager::animationval(std::string str)
{
	return animations[str];
}

/******************************************************************************
prefabsval
-	The function return an object* based on the name of the object

@param str - the prefab name of the object

@return - an Object item
*******************************************************************************/
Object* AssetManager::prefabsval(std::string str)
{
	try {
		return prefabs.at(str);
	}
	catch (std::out_of_range) {
		return nullptr;
	}
}


/******************************************************************************
prefabById
-	The function returns an prefab obect through id instead

@param id - the id for the object

@return - an object item
*******************************************************************************/
Object* AssetManager::prefabById(long id)
{
	auto it = prefabs.begin();
	std::advance(it, -(id + 1));
	return it->second;
}

/******************************************************************************
updateprefab
-	The function update the prefab data

@param str - the name of the object
@param o - the Object item
*******************************************************************************/
void AssetManager::updateprefab(std::string str, Object* o)
{
	prefabs[str] = o;
}

/******************************************************************************
objectprefabsval
-	Returns the path of the directory for prefabs

@return - path of the directory for prefabs
*******************************************************************************/
std::string AssetManager::objectprefabsval()
{
	return objectprefabs.string();
}

/******************************************************************************
cleanprefab
-	Clean the prefab list
*******************************************************************************/
void AssetManager::cleanprefab()
{
	for (const auto& p : prefabs)
		delete p.second;

	prefabs.clear();
}

/******************************************************************************
soundsval
-	Returns the FMOD::Sound based on the name of the audio

@param str - name of the sound

@return - FMOD::Sound
*******************************************************************************/
FMOD::Sound* AssetManager::soundsval(std::string str)
{
	try {
		return sounds.at(str);
	}
	catch (std::out_of_range) {
		return nullptr;
	}
}

/******************************************************************************
getsoundbyaudiotype
-	return the FMOD::Sound based on the type of audio

@param a - AudioType enum
@param random - if audio should be random
@param seq_num - the audio based on the sequence number

@return - FMOD::Sound
*******************************************************************************/
FMOD::Sound* AssetManager::getsoundbyaudiotype(std::string audioType, bool random, int seq_num) {
	try {
		auto& sound = soundMapping.at(audioType);

		if (std::holds_alternative<std::string>(sound)) {
			return soundsval(std::get<std::string>(sound));
		}
		else {
			std::vector<std::string> v = std::get<std::vector<std::string>>(sound);
			if (random) {
				// Rng the audio file to play
				std::random_device rd;
				std::mt19937 gen(rd());

				std::uniform_int_distribution<> dis(0, (int) v.size() - 1);

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

/******************************************************************************
addtextures
-	Add textures to the assetmanager texture list

@param str - name of the texture
@param tex - GLuint of the texture
*******************************************************************************/
void AssetManager::addtextures(std::string str, GLuint tex)
{
	textures.emplace(str, tex);
}

/******************************************************************************
updateSoundMap
-	Add sounds into the soundmap

@param a - AudioType enum
@param file - sound name
*******************************************************************************/
void AssetManager::updateSoundMap(std::string audioType, std::string file)
{
	soundMapping.emplace(audioType, file);
}

/******************************************************************************
updateSoundMap
-	The overload of the function allowing for vectors of sound

@param a - AudioType enum
@param file - std::vector of sound
*******************************************************************************/
void AssetManager::updateSoundMap(std::string audioType, std::vector<std::string> file)
{
	soundMapping.emplace(audioType, file);
}

/******************************************************************************
clearSoundMap
-	The function clears the soundMapping
*******************************************************************************/
void AssetManager::clearSoundMap()
{
	soundMapping.clear();
}

/******************************************************************************
addshader
-	The function adds a shader to the map
*******************************************************************************/
void AssetManager::addshader(std::string str, GLSLShader shd)
{
	shaders[str] = shd;
}

/******************************************************************************
freeshader
-	The function frees all the shaders
*******************************************************************************/
void AssetManager::freeshader()
{
	for (auto& shd : shaders)
	{
		glDeleteProgram(shd.second.GetHandle());
	}
}

/******************************************************************************
shaderval
-	The function returns a value from the shader map
*******************************************************************************/
GLSLShader AssetManager::shaderval(std::string str)
{
	try {
		return shaders.at(str);
	}
	catch (std::out_of_range) {
		std::cout << "MISSING SHADERS IN MAP!" << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

/******************************************************************************
addmodel
-	The function adds a model to the map
*******************************************************************************/
void AssetManager::addmodel(std::string str, GLApp::GLModel model)
{
	models[str] = model;
}

/******************************************************************************
freemodel
-	The function fress all the model
*******************************************************************************/
void AssetManager::freemodel()
{
	for (auto& model : models)
	{
		glDeleteVertexArrays(1, &model.second.vaoid);
	}
}

/******************************************************************************
modelval
-	The function returns the value of a model from the map
*******************************************************************************/
GLApp::GLModel AssetManager::modelval(std::string str)
{
	try {
		return models.at(str);
	}
	catch (std::out_of_range) {
		std::cout << "MISSING MODELS IN MAP!" << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

/******************************************************************************
modelexist
-	The function checks whether the model already exist in the map
*******************************************************************************/
bool AssetManager::modelexist(std::string str)
{
	if (models.find(str) == models.end())
		return true;
	else
		return false;
}

/******************************************************************************
cutsceneval
-	The function returns the vector containing all the cutscene for the scene
*******************************************************************************/
std::vector<std::pair<GLuint, std::string>> AssetManager::cutsceneval(std::string str)
{
	try {
		return cutscenes.at(str);
	}
	catch (std::out_of_range) {
		std::cout << "MISSING CUTSCENES IN MAP!" << std::endl;
		std::exit(EXIT_FAILURE);
	}
}




