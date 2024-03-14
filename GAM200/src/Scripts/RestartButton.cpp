/* !
@file	RestartButton.cpp
@author Tan Yee Ann (t.yeeann@digipen.edu)
@date	26/2/2023

This file contains the script for the in-game clickable buttons
*//*__________________________________________________________________________*/
#include "Scripts/RestartButton.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
#include <Core_Engine.h>
#include <SceneManager.h>

// Constructor for the RestartButton class.
// @param name: The name of the portal.
RestartButton::RestartButton(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the RestartButton script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void RestartButton::Start(Object* obj) {
    executeOnPause = true;
    std::cout << "RestartButton Script Ready : " << obj->GetName() << std::endl;
}

/*********************************************************************/
// Update method called every frame to update the RestartButton's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void RestartButton::Update(Object* obj) {
    if (obj == nullptr || !input::IsPressed(KEY::mouseL) || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
        //std::cout << "NIL OBJ : RestartButton" << std::endl;
        return;
    }

    if (isObjectClicked((Transform*) obj->GetComponent(ComponentType::Transform), Vec2(input::GetMouseX(), input::GetMouseY()))) {
		std::cout << "Button Clicked" << std::endl;
		audio->playSfx("button_click");

        // Restart the level.
        GameLogic::restarting = true;

	}
}
/*********************************************************************/
// Shutdown method called when the RestartButton script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void RestartButton::Shutdown(Object* obj) {
    std::cout << "RestartButton Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of RestartButton.
RestartButton RestartButton("RestartButton");


