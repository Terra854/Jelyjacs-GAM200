/* !
@file	Audio.cpp
@author	Tan Yee Ann
@date	28/9/2023

This file contains the definitions of the functions that are part of the Audio system
*//*__________________________________________________________________________*/
#include "Audio.h"

Audio* audio = nullptr;

Audio::~Audio(){
    system->close();
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

    // Load sounds
    system->createSound("Asset/Sounds/Game_Background.wav", FMOD_LOOP_NORMAL, 0, &game_background);
    system->playSound(game_background, 0, false, &background);
    background->setVolume(0.2f);

    system->createSound("Asset/Sounds/Footsteps.wav", FMOD_LOOP_NORMAL, 0, &walking);
    system->playSound(walking, 0, false, &channel);
    channel->setVolume(0.0);


    system->createSound("Asset/Sounds/Jump.wav", FMOD_DEFAULT, 0, &jump);
    system->createSound("Asset/Sounds/Sliding_Door_Open.wav", FMOD_DEFAULT, 0, &sliding_door_open);

    // Let other systems have access to this system
    audio = this;
}

void Audio::Update(){
    system->update();
}

void Audio::startWalking() {
    channel->setVolume(1.0);
}

void Audio::stopWalking() {
    channel->setVolume(0.0);
}

void Audio::playJump() {
    system->playSound(jump, 0, false, &sfx);
    sfx->setVolume(0.2f);
}

void Audio::playSlidingDoor() {
    system->playSound(sliding_door_open, 0, false, &sfx);
    sfx->setVolume(0.2f);
}