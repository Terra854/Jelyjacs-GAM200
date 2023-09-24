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
private:
	FMOD::System* system;
	FMOD::System* sound;
	FMOD::Channel* channel = 0;
	FMOD_RESULT result;
};