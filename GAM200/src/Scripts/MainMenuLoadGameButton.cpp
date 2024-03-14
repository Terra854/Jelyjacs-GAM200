/* !
@file	MainMenuLoadGameButton.cpp
@author
@date	26/2/2023

This file contains the script for the in-game clickable buttons
*//*__________________________________________________________________________*/
#include "Scripts/MainMenuLoadGameButton.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
#include <Core_Engine.h>
#include <SceneManager.h>
#include <../components/Texture.h>
#include <Object.h>

// Constructor for the ButtonBase class.
// @param name: The name of the portal.
MainMenuLoadGameButton::MainMenuLoadGameButton(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the ButtonBase script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void MainMenuLoadGameButton::Start(Object* obj) {
    executeOnPause = true;
    std::cout << "ButtonBase Script Ready : " << obj->GetName() << std::endl;
}

/*********************************************************************/
// Update method called every frame to update the ButtonBase's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void MainMenuLoadGameButton::Update(Object* obj) {
    if (obj == nullptr || !input::IsPressed(KEY::mouseL) || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
        //std::cout << "NIL OBJ : ButtonBase" << std::endl;
        return;
    }

    if (isObjectClicked((Transform*)obj->GetComponent(ComponentType::Transform), Vec2(input::GetMouseX(), input::GetMouseY()))) {
        std::cout << "Button Clicked" << std::endl;
        audio->playSfx("button_click");
    }
}
/*********************************************************************/
// Shutdown method called when the ButtonBase script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void MainMenuLoadGameButton::Shutdown(Object* obj) {
    std::cout << "ButtonBase Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of ButtonBase.
MainMenuLoadGameButton mainMenuLoadGameButton("MainMenuLoadGameButton");