#pragma once
/* !
@file	Audio.h
@author	Tan Yee Ann (t.yeeann@digipen.edu)
@date	28/9/2023

This file contains the declaration for the Audio system class
*//*__________________________________________________________________________*/
#include <Interface_System.h>
#include <fmod.hpp>
#pragma warning(disable : 4505)
#include <fmod_errors.h> // Supress the "unreferenced function with internal linkage has been removed" warnings in Fmod
#pragma warning(default : 4505)


class Audio : public ISystems {
public:
	~Audio();
	virtual void Initialize();
	virtual void Update() override;

	void playBackground();
	void stopBackground();

	// Returns name of the component
	virtual std::string SystemName() { return "Audio"; }

	void setupSound();
	void createSound(std::string str, FMOD_MODE mode, FMOD::Sound** sound);

	void deleteSound(FMOD::Sound* sound);

	void playSfx(std::string audioName, float volume_multiplier = 1.0f);
	void playSfx(std::string audioName, FMOD::ChannelGroup *&sfxChannelGroup, float volume_multiplier = 1.0f);
	void setBackgroundAudio(std::string bg);
	void setBackgroundVolume(float bg_volume_multiplier);
	void setMasterVolume(float volume);
	void restartBackgroundAudio();
	void stopSfx(FMOD::ChannelGroup*& c);
	void createChannelGroup(std::string name, FMOD::ChannelGroup*& c);

	friend class LevelEditor;
private:
	FMOD::System* system;
	FMOD::Channel *background, *channel, *sfx;
	FMOD_RESULT result;

	float master_volume = 1.f, bg_volume_multiplier = 1.f;
	std::string current_background_audio;
};

extern Audio* audio;