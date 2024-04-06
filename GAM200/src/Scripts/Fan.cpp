/* !
@file	Fan.cpp
@author Tan Yee Ann
@date	13/3/2024

This file contains the script for Fan
*//*__________________________________________________________________________*/
#include "Scripts/Fan.h"
#include <iostream>
#include <components/Transform.h>
#include <components/Animation.h>
#include <components/Physics.h>
#include <components/Body.h>
#include <components/Text.h>
#include <SceneManager.h>
#include "camera.h"

FMOD::ChannelGroup* fan_sfx;
bool fan_paused;
std::map<int, float> FanVolume;
float CurrentFanVolume = 0.f, max_distance = 200.f, timer = 0.f;
int prev_fan_id = -1;

Fan::Fan(std::string name) : LogicScript(name)
{
	std::cout << name << " Created" << std::endl;
}
/***************************************************************************/
// Start method, called when the Fan script is first activated.
// @param obj: A pointer to the Object that this script is attached to.
// Performs initial setup and configuration for Fan.
/***************************************************************************/
void Fan::Start(Object* obj) {
	if (obj == nullptr) {
		//std::cout << "NIL OBJ : Player" << std::endl;
		return;
	}

	FanVolume.clear();

	executeOnPause = true;

	audio->createChannelGroup("fan", fan_sfx);
	fan_sfx->setPaused(true);
	fan_paused = true;
	audio->playSfx("fan", fan_sfx, 0.5f);

	std::cout << "Fan Script Ready : " << obj->GetName() << std::endl;
}

/***************************************************************************/
// Update method, called on every frame to update Fan's state.
// @param obj: A pointer to the Object that this script is attached to.
// Contains logic for Fan's movement and animations.
/***************************************************************************/
void Fan::Update(Object* obj) {
	if (obj == nullptr) {
		//std::cout << "NIL OBJ : Player" << std::endl;
		return;
	}

	if (obj->GetId() <= prev_fan_id) {
		fan_sfx->setVolume(CurrentFanVolume / max_distance * 2.f);
		CurrentFanVolume = 0.f;
	}

	prev_fan_id = obj->GetId();

	if (engine->isPaused() && !fan_paused) {
		fan_sfx->setPaused(true);
		fan_paused = true;
	}
	else if (!engine->isPaused() && fan_paused) {
		fan_sfx->setPaused(false);
		fan_paused = false;
	}

	if (!fan_paused)
		timer += engine->GetDt();

	if (timer > 60.f) {
		audio->playSfx("fan", fan_sfx, 1.f);
		timer = 0.f;
	}

	Transform* player = static_cast<Transform*>(GameLogic::playerObj->GetComponent(ComponentType::Transform));
	Transform* fan = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));

	if (!fan || !player)
		return;
	
	float calculated_volume = max_distance - Vec2Distance(player->Position, fan->Position);
	calculated_volume = calculated_volume < 0.f ? 0.f : calculated_volume;

	if (calculated_volume > CurrentFanVolume) {
		CurrentFanVolume = calculated_volume;
	}


}

/***************************************************************************/
// Shutdown method, called when the Fan script is stopped.
// @param obj: A pointer to the Object that this script is attached to.
// Performs cleanup tasks for Fan.
/***************************************************************************/
void Fan::Shutdown(Object* obj) {
	std::cout << "Fan Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of the Fan class.
Fan Fan("Fan");