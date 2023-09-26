#include "Audio.h"

Audio* audio;

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

    system->createSound("../Asset/Sounds/Jump.wav", FMOD_DEFAULT, 0, &jump);
    system->createSound("../Asset/Sounds/Footsteps.wav", FMOD_LOOP_NORMAL, 0, &walking);

    system->playSound(walking, 0, false, &channel);

    audio = this;
}

void Audio::Update(float time){
    system->update();
}

void Audio::startWalking() {
    channel->setVolume(1.0);
}

void Audio::stopWalking() {
    channel->setVolume(0.0);
}

void Audio::playJump() {
    system->playSound(jump, 0, false, &channel2);
    channel2->setVolume(0.2f);
}
