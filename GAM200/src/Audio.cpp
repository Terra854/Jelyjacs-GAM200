/* !
@file	Audio.cpp
@author	Tan Yee Ann (t.yeeann@digipen.edu)
@date	28/9/2023

This file contains the definitions of the functions that are part of the Audio system
*//*__________________________________________________________________________*/
#include "Audio.h"
#include "Assets Manager/asset_manager.h"
#include <Core_Engine.h>
#include "Assets Manager/json_serialization.h"

Audio* audio = nullptr;

/******************************************************************************
Descructor for the Audio system
*******************************************************************************/
Audio::~Audio(){
    AssetManager::clearsounds();
    system->release();
}

/******************************************************************************
Initialize
-	This function initalizes the Audio system by setting up Fmod
*******************************************************************************/
void Audio::Initialize(){
    // Create the main system object.
    result = FMOD::System_Create(&system);      
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }

    // Initialize FMOD.
    result = system->init(512, FMOD_INIT_NORMAL, 0);    
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }

    // Let other systems have access to this system
    audio = this;

}

/******************************************************************************
setupSound
-	This function loads the sound mappings and sets the background sound
*******************************************************************************/
void Audio::setupSound()
{
    // Load sounds
    std::string soundmap = "Asset/Sounds/sounds.json";
    
	// Check if the given file exists
	JsonSerialization jsonobj;
	jsonobj.openFileRead(soundmap);

	// Loop through audio types
	for (const auto& key : jsonobj.jsonObject->getMemberNames()) {
		
			if (jsonobj.isArray(key))
			{
				size_t pos = jsonobj.size(key);
				std::vector<std::string> audioarr;
				for (size_t j = 0; j < pos; j++)
				{
					std::string audioval;
					jsonobj.readString(audioval, key, (int)j);
					audioarr.push_back(audioval);
					std::cout << "AudioMap arrvalue: " << audioval << std::endl;
				}
				AssetManager::updateSoundMap(key, audioarr);
			}
			else
			{
				std::string audioval;
				jsonobj.readString(audioval, key);
				AssetManager::updateSoundMap(key, audioval);
				std::cout << "AudioMap value: " << audioval << std::endl;
			}
	}

	jsonobj.closeFile();

    // Set the background music
	system->playSound(AssetManager::getsoundbyaudiotype("background"), 0, true, &background);
    background->setVolume(0.2f);
}

/******************************************************************************
createSound
-	This function calls Fmod to load a given sound file
*******************************************************************************/
void Audio::createSound(std::string str, FMOD_MODE mode, FMOD::Sound** sound)
{
    system->createSound(str.c_str(), mode, 0, sound);
}

/******************************************************************************
Update
-	The update loop for the Audio system
*******************************************************************************/
void Audio::Update(){
    system->update();

    background->setPaused(engine->isPaused());
}

/******************************************************************************
playSfx
-	This function tells Fmod to play the given sfx name

@param a - The sfx name to play
@param sfxChannel - The sfx channel group to play the sfx on. Will use the default sfx channel if left empty
*******************************************************************************/
void Audio::playSfx(std::string audioType, float volume_multiplier) {
	system->playSound(AssetManager::getsoundbyaudiotype(audioType), 0, false, &sfx);
	sfx->setVolume(0.2f * volume_multiplier);
}

void Audio::playSfx(std::string audioType, FMOD::ChannelGroup *&sfxChannelGroup, float volume_multiplier) {
	system->playSound(AssetManager::getsoundbyaudiotype(audioType), sfxChannelGroup, false, &channel);
	channel->setVolume(0.2f * volume_multiplier);
}

/******************************************************************************
restartBackgroundAudio
-	This function restarts the background the audio from the beginning
-	To be called if you are changing or restarting a scene or if the sound
	linked to background changes
*******************************************************************************/
void Audio::restartBackgroundAudio() {
	background->stop();
	system->playSound(AssetManager::getsoundbyaudiotype("background"), 0, true, &background);
	background->setVolume(0.2f);
}

void Audio::stopSfx(FMOD::ChannelGroup* &c) {
	c->stop();
}

void Audio::createChannelGroup(std::string name, FMOD::ChannelGroup* &c) {
	system->createChannelGroup(name.c_str(), &c);
}