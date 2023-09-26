#pragma once

#include <Interface_System.h>
#include <fmod.hpp>
#include <fmod_errors.h>

class Audio : public ISystems {
public:
	~Audio();
	virtual void Initialize();
	virtual void Update(float time) override;
	virtual std::string SystemName() { return "Audio"; }

	void startWalking();
	void stopWalking();
	void playJump();
private:
	FMOD::System* system;
	FMOD::System* sound;
	FMOD::Channel *channel, *channel2;
	FMOD_RESULT result;

	FMOD::Sound *walking, *jump;

	bool isPlayingWalking = false;
};

extern Audio* audio;