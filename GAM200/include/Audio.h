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
	Background_Music,
	Button_Click,
	Cat_Teleport,
	Finn_Jumping,
	Finn_Walking,
	Spark_Jumping,
	Spark_Walking,
	Sliding_Door_Open,
	END
};

class Audio : public ISystems {
public:
	~Audio();
	virtual void Initialize();
	virtual void Update() override;

	// Returns name of the component
	virtual std::string SystemName() { return "Audio"; }

	void setupSound();
	void createSound(std::string str, FMOD_MODE mode, FMOD::Sound** sound);

	void playSfx(AudioType a);
	void restartBackgroundAudio();
private:
	FMOD::System* system;
	FMOD::Channel *background, *channel, *sfx;
	FMOD_RESULT result;
};

extern Audio* audio;