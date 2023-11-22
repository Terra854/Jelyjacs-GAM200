/* !
@file	Audio.cpp
@author	Tan Yee Ann
@date	28/9/2023

This file contains the definitions of the functions that are part of the Audio system
*//*__________________________________________________________________________*/
#include "Audio.h"
#include "Assets Manager/asset_manager.h"
#include <Core_Engine.h>

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
    //system->createSound("Asset/Sounds/Game_Background.wav", FMOD_LOOP_NORMAL, 0, &game_background);
    system->playSound(AssetManager::soundsval("Game_Background.wav"), 0, false, &background);
    background->setVolume(0.2f);

    //system->createSound("Asset/Sounds/Footsteps.wav", FMOD_LOOP_NORMAL, 0, &walking);
    system->playSound(AssetManager::soundsval("Footsteps.wav"), 0, false, &channel);
    channel->setVolume(0.0);


    //system->createSound("Asset/Sounds/Jump.wav", FMOD_DEFAULT, 0, &jump);
    //system->createSound("Asset/Sounds/Sliding_Door_Open.wav", FMOD_DEFAULT, 0, &sliding_door_open);
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

// Legacy functions, will be nuked

void Audio::playJump() {
    system->playSound(AssetManager::soundsval("Jump.wav"), 0, false, &sfx);
    sfx->setVolume(0.2f);
}

void Audio::playSlidingDoor() {
    system->playSound(AssetManager::soundsval("Sliding_Door_Open.wav"), 0, false, &sfx);
    sfx->setVolume(0.2f);
}