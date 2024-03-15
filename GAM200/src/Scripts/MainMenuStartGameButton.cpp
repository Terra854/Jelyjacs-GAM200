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
}

/*********************************************************************/
// Update method called every frame to update the ButtonBase's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void MainMenuStartGameButton::Update(Object* obj) {
	static float accum_time = 0.0f;
	static int frame_dt_count = 0;

	frame_dt_count--;
	if (obj == nullptr || !input::IsPressed(KEY::mouseL) || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
		//std::cout << "NIL OBJ : ButtonBase" << std::endl;
		return;
	}

	if (isObjectClicked((Transform*)obj->GetComponent(ComponentType::Transform), Vec2(input::GetMouseX(), input::GetMouseY()))) {
		std::cout << "Button Clicked" << std::endl;
		audio->playSfx("button_click");

		// shift to center of screen
		app->video_start = true;

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