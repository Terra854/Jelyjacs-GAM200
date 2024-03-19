/* !
@file	MainMenuStartGameButton.cpp
@author Jonathan Woo(w.jiahowjonathan@digipen.edu)
@date	15/3/2023

This file contains the script for the in-game clickable buttons
*//*__________________________________________________________________________*/
#include "Scripts/MainMenuStartGameButton.h"
#include "glapp.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
#include <string>
#include <SceneManager.h>
#include <components/Animation.h>

std::vector<Animation*> FinnCrashingVideo;
bool HaveFinnCrash;
float videoTimer;
float videoFPS = 1.f / 25.f;
float sfxCooldown = 0.f;
int videoFrame;

static float EaseOutBounce(float start, float end, float value)
{
	value /= 1.f;
	end -= start;
	if (value < (1 / 2.75f))
	{
		return end * (7.5625f * value * value) + start;
	}
	else if (value < (2 / 2.75f))
	{
		value -= (1.5f / 2.75f);
		return end * (7.5625f * (value)*value + .75f) + start;
	}
	else if (value < (2.5 / 2.75))
	{
		value -= (2.25f / 2.75f);
		return end * (7.5625f * (value)*value + .9375f) + start;
	}
	else
	{
		value -= (2.625f / 2.75f);
		return end * (7.5625f * (value)*value + .984375f) + start;
	}
}

// Constructor for the ButtonBase class.
// @param name: The name of the portal.
MainMenuStartGameButton::MainMenuStartGameButton(std::string name) : LogicScript(name)
{
	std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the ButtonBase script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void MainMenuStartGameButton::Start(Object* obj) {
	executeOnPause = true;
	std::cout << "MainMenuStartGameButton Script Ready : " << obj->GetName() << std::endl;

	FinnCrashingVideo.clear();

	Object* o1, * o2, * o3, * o4, * o5, * o6, * o7;

	o1 = objectFactory->FindObject("Video1");
	o2 = objectFactory->FindObject("Video2");
	o3 = objectFactory->FindObject("Video3");
	o4 = objectFactory->FindObject("Video4");
	o5 = objectFactory->FindObject("Video5");
	o6 = objectFactory->FindObject("Video6");
	o7 = objectFactory->FindObject("Video7");

	if (o1 && o2 && o3 && o4 && o5 && o6 && o7) {

		FinnCrashingVideo.push_back(static_cast<Animation*>(o1->GetComponent(ComponentType::Animation)));
		FinnCrashingVideo.push_back(static_cast<Animation*>(o2->GetComponent(ComponentType::Animation)));
		FinnCrashingVideo.push_back(static_cast<Animation*>(o3->GetComponent(ComponentType::Animation)));
		FinnCrashingVideo.push_back(static_cast<Animation*>(o4->GetComponent(ComponentType::Animation)));
		FinnCrashingVideo.push_back(static_cast<Animation*>(o5->GetComponent(ComponentType::Animation)));
		FinnCrashingVideo.push_back(static_cast<Animation*>(o6->GetComponent(ComponentType::Animation)));
		FinnCrashingVideo.push_back(static_cast<Animation*>(o7->GetComponent(ComponentType::Animation)));

		for (auto a : FinnCrashingVideo) {
			a->opacity = 0.f;
			a->fixed = true;
		}
	}

	HaveFinnCrash = false;
	videoTimer = 0.f;
	videoFrame = 0;

}

/*********************************************************************/
// Update method called every frame to update the ButtonBase's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void MainMenuStartGameButton::Update(Object* obj) {
	for (auto a : FinnCrashingVideo) {
		a->opacity = 0.f;
	}

	if (HaveFinnCrash) {
		videoTimer += engine->GetDt();
				
		videoFrame = videoTimer > 1.f ? (int)((videoTimer - 1.f) / videoFPS) : 0;

		auto MainMenu = objectFactory->GetLayer("MainMenu");

		for (Object* o : MainMenu->second.second) {
			static_cast<Texture*>(o->GetComponent(ComponentType::Texture))->opacity = EaseOutBounce(1.f, 0.f, videoTimer);
		}

		if (videoFrame < 51) {
			FinnCrashingVideo[videoFrame / 8]->opacity = 1.f;
			FinnCrashingVideo[videoFrame / 8]->frame_num = videoFrame % 8;
		}
		else {
			HaveFinnCrash = false;
			audio->setBackgroundAudio("background");
			SceneManager::LoadScene("tutorial_level.json");
		}

		sfxCooldown -= engine->GetDt();
		sfxCooldown = sfxCooldown < 0.f ? 0.f : sfxCooldown;

		if (!sfxCooldown) {
			switch (videoFrame) {
			case 4:
				audio->playSfx("start_game_finn_crashing");
				sfxCooldown = videoFPS;
				break;
			case 32:
				audio->playSfx("spark_jumping");
				sfxCooldown = videoFPS;
				break;
			case 39:
				audio->playSfx("spark_walking");
				sfxCooldown = videoFPS;
				break;
			}
		}

	}

	if (obj == nullptr || !input::IsPressed(KEY::mouseL) || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible || static_cast<Texture*>(obj->GetComponent(ComponentType::Texture))->opacity < 1.f) {
		//std::cout << "NIL OBJ : ButtonBase" << std::endl;
		return;
	}

	if (isObjectClicked((Transform*)obj->GetComponent(ComponentType::Transform), Vec2(input::GetMouseX(), input::GetMouseY()))) {
		std::cout << "Button Clicked" << std::endl;
		audio->playSfx("button_click");
		audio->playSfx("title_losing_power");

		// shift to center of screen
		//app->video_start = true;

		HaveFinnCrash = true;

	}
}
/*********************************************************************/
// Shutdown method called when the ButtonBase script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void MainMenuStartGameButton::Shutdown(Object* obj) {
	std::cout << "MainMenuStartGameButton Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of ButtonBase.
MainMenuStartGameButton mainMenuStartGameButton("MainMenuStartGameButton");