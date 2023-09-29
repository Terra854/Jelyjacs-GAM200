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

class Audio : public ISystems {
public:
	~Audio();
	virtual void Initialize();
	virtual void Update() override;
	virtual std::string SystemName() { return "Audio"; }

	void startWalking();
	void stopWalking();
	void playJump();
	//void stopJump();
private:
	FMOD::System* system;
	FMOD::System* sound;
	FMOD::Channel *channel, *channel2;
	FMOD_RESULT result;

	FMOD::Sound *walking, *jump;

	bool isPlayingWalking = false;
};

extern Audio* audio;