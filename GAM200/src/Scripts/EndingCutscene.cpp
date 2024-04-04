/* !
@file	EndingCutscene.cpp
@author Tan Yee Ann
@date	13/3/2024

This file contains the script for EndingCutscene
*//*__________________________________________________________________________*/
#include "Scripts/EndingCutscene.h"
#include <iostream>
#include <components/Transform.h>
#include <components/Animation.h>
#include <components/Physics.h>
#include <components/Body.h>
#include <Audio.h>
#include <components/Text.h>
#include <SceneManager.h>
#include "camera.h"

float EndingCutsceneTime;
Texture* ending_cutscene, *end_text;
std::pair<std::string, std::pair<LayerSettings, std::vector<Object*>>>* thankyou_text;
FMOD::ChannelGroup* ending_bg;

EndingCutscene::EndingCutscene(std::string name) : LogicScript(name)
{
	std::cout << name << " Created" << std::endl;
}
/***************************************************************************/
// Start method, called when the EndingCutscene script is first activated.
// @param obj: A pointer to the Object that this script is attached to.
// Performs initial setup and configuration for EndingCutscene.
/***************************************************************************/
void EndingCutscene::Start(Object* obj) {
	if (obj == nullptr || engine->loaded_level != "ending_cutscene") {
		//std::cout << "NIL OBJ : Player" << std::endl;
		return;
	}
	camera2D->scale = { 1.f,1.f };
	audio->createChannelGroup("ending_bg", ending_bg);

	ending_cutscene = static_cast<Texture*>(objectFactory->FindObject("ending_cutscene")->GetComponent(ComponentType::Texture));
	end_text = static_cast<Texture*>(objectFactory->FindObject("ending_cutscene_end_text")->GetComponent(ComponentType::Texture));

	thankyou_text = objectFactory->FindLayerThatHasThisObject(objectFactory->FindObject("ending_cutscene_thankyou_text"));

	EndingCutsceneTime = 0.f;

	std::cout << "EndingCutscene Script Ready : " << obj->GetName() << std::endl;
}

/***************************************************************************/
// Update method, called on every frame to update EndingCutscene's state.
// @param obj: A pointer to the Object that this script is attached to.
// Contains logic for EndingCutscene's movement and animations.
/***************************************************************************/
void EndingCutscene::Update(Object* obj) {
	if (obj == nullptr || engine->loaded_level != "ending_cutscene") {
		//std::cout << "NIL OBJ : Player" << std::endl;
		return;
	}

	if (EndingCutsceneTime == 0.f) {
		audio->stopBackground();
		audio->playSfx("running_away", ending_bg);
		audio->playSfx("police_chase", ending_bg);
	}
	EndingCutsceneTime += engine->GetDt();

	ending_cutscene->opacity = end_text->opacity = 1.f;

	ending_cutscene->opacity = EndingCutsceneTime;
	end_text->opacity = EndingCutsceneTime - 2.f;

	thankyou_text->second.first.isVisible = EndingCutsceneTime > 5.f;

	if (EndingCutsceneTime > 5.f) {
		if (input::IsPressed(KEY::mouseL)) {
			std::cout << "Button Clicked" << std::endl;
			audio->playSfx("button_click");

			audio->stopSfx(ending_bg);
			audio->setBackgroundAudio("main_menu_bg");
			SceneManager::LoadScene("main_menu.json");
		}
	}
}

/***************************************************************************/
// Shutdown method, called when the EndingCutscene script is stopped.
// @param obj: A pointer to the Object that this script is attached to.
// Performs cleanup tasks for EndingCutscene.
/***************************************************************************/
void EndingCutscene::Shutdown(Object* obj) {
	std::cout << "EndingCutscene Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of the EndingCutscene class.
EndingCutscene EndingCutscene("EndingCutscene");