/* !
@file	MainMenuStartGameButton.cpp
@author Tan Yee Ann (t.yeeann@digipen.edu)
@date	26/2/2023

This file contains the script for the in-game clickable buttons
*//*__________________________________________________________________________*/
#include "Scripts/MainMenuStartGameButton.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
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
    if (obj == nullptr || !input::IsPressed(KEY::mouseL) || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
        //std::cout << "NIL OBJ : ButtonBase" << std::endl;
        return;
    }

    if (isObjectClicked((Transform*)obj->GetComponent(ComponentType::Transform), Vec2(input::GetMouseX(), input::GetMouseY()))) {
        std::cout << "Button Clicked" << std::endl;
        audio->playSfx("button_click");
        SceneManager::LoadScene("level_1.json");
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