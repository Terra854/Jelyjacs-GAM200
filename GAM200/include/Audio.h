#pragma once
/* !
@file	Audio.h
@author	Tan Yee Ann
@date	28/9/2023

This file contains the declaration for the Audio system class
*//*__________________________________________________________________________*/
#include <Interface_System.h>
#include <fmod.hpp>
#include <fmod_errors.h>

enum AudioType {
	Background,
	Walking,
	Finn_Jumping,
	Spark_Jumping,
	Sliding_Door_Open,
	END
};

class Audio : public ISystems {
public:
	~Audio();
	virtual void Initialize();
	virtual void Update() override;
	virtual std::string SystemName() { return "Audio"; }

	void setupSound();
	void createSound(std::string str, FMOD_MODE mode, FMOD::Sound** sound);

	void startWalking();
	void stopWalking();
	void playSfx(AudioType a);
	void setBackgroundAudio();
	void setWalkingAudio();
private:
	FMOD::System* system;
	FMOD::Channel *background, *channel, *sfx;
	FMOD_RESULT result;

	//FMOD::Sound *walking, *jump, *game_background, *sliding_door_open;

	bool isPlayingWalking = false;
};

extern Audio* audio;