/* !
@file	ResumeButton.cpp
@author Tan Yee Ann (t.yeeann@digipen.edu)
@date	26/2/2023

This file contains the script for the in-game clickable buttons
*//*__________________________________________________________________________*/
#include "Scripts/ResumeButton.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
#include <Core_Engine.h>
#include <SceneManager.h>
#include <../components/Texture.h>

// Constructor for the ResumeButton class.
// @param name: The name of the portal.
ResumeButton::ResumeButton(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the ResumeButton script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void ResumeButton::Start(Object* obj) {
    executeOnPause = true;
    std::cout << "ResumeButton Script Ready : " << obj->GetName() << std::endl;
}

/*********************************************************************/
// Update method called every frame to update the ResumeButton's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void ResumeButton::Update(Object* obj) {
    if (obj == nullptr || objectFactory->FindLayerThatHasThisObject(obj) == nullptr ||!objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
        //std::cout << "NIL OBJ : ResumeButton" << std::endl;
        return;
    }
    Texture* tex = static_cast<Texture*>(obj->GetComponent(ComponentType::Texture));
    if (!isObjectClicked((Transform*) obj->GetComponent(ComponentType::Transform), Vec2(input::GetMouseX(), input::GetMouseY()))) {
        tex->textureName = "Resume.png";
        return;
	}

    tex->textureName = "Resume glow.png";
    Logic->button_hover = true;

    if (input::IsPressed(KEY::mouseL))
    {
        std::cout << "Button Clicked" << std::endl;
        audio->playSfx("button_click");

        // Pause the game.
        sceneManager->PlayScene();

        objectFactory->GetLayer("PauseMenu")->second.first.isVisible = false;
        objectFactory->GetLayer("GameMenu")->second.first.isVisible = true;
    }
}
/*********************************************************************/
// Shutdown method called when the ResumeButton script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void ResumeButton::Shutdown(Object* obj) {
    std::cout << "ResumeButton Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of ResumeButton.
ResumeButton ResumeButton("ResumeButton");


