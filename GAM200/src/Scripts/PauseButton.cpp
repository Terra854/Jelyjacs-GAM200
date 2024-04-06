/* !
@file	PauseButton.cpp
@author Tan Yee Ann (t.yeeann@digipen.edu)
@date	26/2/2023

This file contains the script for the in-game clickable buttons
*//*__________________________________________________________________________*/
#include "Scripts/PauseButton.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
#include <Core_Engine.h>
#include <SceneManager.h>

// Constructor for the PauseButton class.
// @param name: The name of the portal.
PauseButton::PauseButton(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the PauseButton script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void PauseButton::Start(Object* obj) {
    executeOnPause = true;
    std::cout << "PauseButton Script Ready : " << obj->GetName() << std::endl;
}

/*********************************************************************/
// Update method called every frame to update the PauseButton's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void PauseButton::Update(Object* obj) {
    if (obj == nullptr || objectFactory->FindLayerThatHasThisObject(obj) == nullptr || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
        //std::cout << "NIL OBJ : PauseButton" << std::endl;
        return;
    }

    if (isObjectClicked((Transform*) obj->GetComponent(ComponentType::Transform), Vec2(input::GetMouseX(), input::GetMouseY()))) {
		Logic->button_hover = true;

        if (input::MouseClickedOnce()) {

            std::cout << "Button Clicked" << std::endl;
            audio->playSfx("button_click");

            // Pause the game.
            sceneManager->PauseScene();

            objectFactory->GetLayer("PauseMenu")->second.first.isVisible = true;
            objectFactory->GetLayer("GameMenu")->second.first.isVisible = false;
        }
	}
}
/*********************************************************************/
// Shutdown method called when the PauseButton script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void PauseButton::Shutdown(Object* obj) {
    std::cout << "PauseButton Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of PauseButton.
PauseButton PauseButton("PauseButton");


