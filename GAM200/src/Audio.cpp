/* !
@file	Audio.cpp
@author	Tan Yee Ann
@date	28/9/2023

This file contains the definitions of the functions that are part of the Audio system
*//*__________________________________________________________________________*/
#include "Audio.h"
#include "Assets Manager/asset_manager.h"
#include <Core_Engine.h>
#include "Assets Manager/json_serialization.h"

Audio* audio = nullptr;

Audio::~Audio(){
    AssetManager::clearsounds();
    system->release();
}

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
void Audio::setupSound()
{
    // Load sounds
    std::string soundmap = "Asset/Sounds/sounds.json";
    
	// Check if the given file exists
	JsonSerialization jsonobj;
	jsonobj.openFileRead(soundmap);

	// Loop through AudioType
	for (int i = 0; i < AudioType::END; i++)
	{
		std::string audio_type;
		switch (i) // Set audio value based on current AudioType
		{
		case AudioType::Background:
			audio_type = "background";
			break;
		case AudioType::Button_Click:
			audio_type = "button_click";
			break;
		case AudioType::Cat_Teleport:
			audio_type = "cat_teleport";
			break;
		case AudioType::Walking:
			audio_type = "walking";
			break;
		case AudioType::Finn_Jumping:
			audio_type = "finn_jumping";
			break;
		case AudioType::Spark_Jumping:
			audio_type = "spark_jumping";
			break;
		case AudioType::Sliding_Door_Open:
			audio_type = "sliding_door_open";
			break;
		default:
			audio_type = "END";
			break;
		}

		AudioType a = static_cast<AudioType>(i);

		// Check if AudioType exist in audiomap json and then update it
		if (jsonobj.isMember(audio_type))
		{
			if (jsonobj.isArray(audio_type))
			{
				size_t pos = jsonobj.size(audio_type);
				std::vector<std::string> audioarr;
				for (size_t j = 0; j < pos; j++)
				{
					std::string audioval;
					jsonobj.readString(audioval, audio_type, (int)j);
					audioarr.push_back(audioval);
					std::cout << "AudioMap arrvalue: " << audioval << std::endl;
				}
				AssetManager::updateSoundMap(a, audioarr);
			}
			else
			{
				std::string audioval;
				jsonobj.readString(audioval, audio_type);
				AssetManager::updateSoundMap(a, audioval);
				std::cout << "AudioMap value: " << audioval << std::endl;
			}
		}
	}

	jsonobj.closeFile();

    // Set the background music
    system->playSound(AssetManager::getsoundbyaudiotype(AudioType::Background), 0, false, &background);
    background->setVolume(0.2f);

    system->playSound(AssetManager::getsoundbyaudiotype(AudioType::Walking), 0, false, &channel);
    channel->setVolume(0.0);
}

void Audio::createSound(std::string str, FMOD_MODE mode, FMOD::Sound** sound)
{
    system->createSound(str.c_str(), mode, 0, sound);
}

void Audio::Update(){
    system->update();

    background->setPaused(engine->isPaused());
}

void Audio::startWalking() {
    channel->setVolume(1.0);
}

void Audio::stopWalking() {
    channel->setVolume(0.0);
}

void Audio::playSfx(AudioType a) {
    system->playSound(AssetManager::getsoundbyaudiotype(a), 0, false, &sfx);
    sfx->setVolume(0.2f);
}

void Audio::setBackgroundAudio() {
    system->playSound(AssetManager::getsoundbyaudiotype(AudioType::Background), 0, false, &background);
    sfx->setVolume(0.2f);
}

void Audio::setWalkingAudio() {
    system->playSound(AssetManager::getsoundbyaudiotype(AudioType::Walking), 0, false, &channel);
    channel->setVolume(0.0);
}