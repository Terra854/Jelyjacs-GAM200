/* !
@file	OpeningCutscene.cpp
@author Tan Yee Ann
@date	13/3/2024

This file contains the script for OpeningCutscene
*//*__________________________________________________________________________*/
#include "Scripts/OpeningCutscene.h"
#include <iostream>
#include <components/Transform.h>
#include <components/Animation.h>
#include <components/Physics.h>
#include <components/Body.h>
#include <Audio.h>
#include <components/Text.h>
#include <SceneManager.h>

float OpeningCutsceneTime;
Texture *opening_cutscene_1, *opening_cutscene_2, *opening_cutscene_3, *opening_cutscene_4, *opening_cutscene_5, *opening_cutscene_6;
Text *opening_cutscene_text;

OpeningCutscene::OpeningCutscene(std::string name) : LogicScript(name)
{
	std::cout << name << " Created" << std::endl;
}
/***************************************************************************/
// Start method, called when the OpeningCutscene script is first activated.
// @param obj: A pointer to the Object that this script is attached to.
// Performs initial setup and configuration for OpeningCutscene.
/***************************************************************************/
void OpeningCutscene::Start(Object* obj) {
	if (obj == nullptr || engine->loaded_level != "opening_cutscene") {
		//std::cout << "NIL OBJ : Player" << std::endl;
		return;
	}

	audio->setBackgroundAudio("opening_cutscene_bg");

	opening_cutscene_1 = static_cast<Texture*>(objectFactory->FindObject("opening_cutscene_1")->GetComponent(ComponentType::Texture));
	opening_cutscene_2 = static_cast<Texture*>(objectFactory->FindObject("opening_cutscene_2")->GetComponent(ComponentType::Texture));
	opening_cutscene_3 = static_cast<Texture*>(objectFactory->FindObject("opening_cutscene_3")->GetComponent(ComponentType::Texture));
	opening_cutscene_4 = static_cast<Texture*>(objectFactory->FindObject("opening_cutscene_4")->GetComponent(ComponentType::Texture));
	opening_cutscene_5 = static_cast<Texture*>(objectFactory->FindObject("opening_cutscene_5")->GetComponent(ComponentType::Texture));
	opening_cutscene_6 = static_cast<Texture*>(objectFactory->FindObject("opening_cutscene_6")->GetComponent(ComponentType::Texture));

	OpeningCutsceneTime = 0.f;

	std::cout << "OpeningCutscene Script Ready : " << obj->GetName() << std::endl;
}

/***************************************************************************/
// Update method, called on every frame to update OpeningCutscene's state.
// @param obj: A pointer to the Object that this script is attached to.
// Contains logic for OpeningCutscene's movement and animations.
/***************************************************************************/
void OpeningCutscene::Update(Object* obj) {
	if (obj == nullptr || engine->loaded_level != "opening_cutscene") {
		//std::cout << "NIL OBJ : Player" << std::endl;
		return;
	}
	
	OpeningCutsceneTime += engine->GetDt();

	opening_cutscene_1->opacity = opening_cutscene_2->opacity = opening_cutscene_3->opacity = opening_cutscene_4->opacity = opening_cutscene_5->opacity = opening_cutscene_6->opacity = 1.f;


	if (OpeningCutsceneTime <= 2.f) {
		opening_cutscene_1->opacity = OpeningCutsceneTime;
		opening_cutscene_2->opacity = opening_cutscene_3->opacity = opening_cutscene_4->opacity = opening_cutscene_5->opacity = opening_cutscene_6->opacity = OpeningCutsceneTime - 1.f;
	}
	else {
		if (5.f <= OpeningCutsceneTime) {
			opening_cutscene_1->opacity = 6.f - OpeningCutsceneTime;
		}
		if (10.f <= OpeningCutsceneTime) {
			opening_cutscene_2->opacity = 11.f - OpeningCutsceneTime;
		}
		if (15.f <= OpeningCutsceneTime) {
			opening_cutscene_3->opacity = 16.f - OpeningCutsceneTime;
		}
		if (20.f <= OpeningCutsceneTime) {
			opening_cutscene_4->opacity = 21.f - OpeningCutsceneTime;
		}
		if (25.f <= OpeningCutsceneTime) {
			opening_cutscene_5->opacity = 26.f - OpeningCutsceneTime;
		}
		if (30.f <= OpeningCutsceneTime) {
			opening_cutscene_6->opacity = 31.f - OpeningCutsceneTime;
			audio->setBackgroundVolume(31.f - OpeningCutsceneTime);
		}
		if (31.f < OpeningCutsceneTime) {
			audio->setBackgroundAudio("background");
			SceneManager::LoadScene("tutorial_level.json");
		}

	}

}

/***************************************************************************/
// Shutdown method, called when the OpeningCutscene script is stopped.
// @param obj: A pointer to the Object that this script is attached to.
// Performs cleanup tasks for OpeningCutscene.
/***************************************************************************/
void OpeningCutscene::Shutdown(Object* obj) {
	std::cout << "OpeningCutscene Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of the OpeningCutscene class.
OpeningCutscene OpeningCutscene("OpeningCutscene");