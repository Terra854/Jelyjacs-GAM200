/* !
@file	MainMenuQuitButton.cpp
@author Jonathan Woo(w.jiahowjonathan@digipen.edu)
@date	15/3/2023

This file contains the script for the in-game clickable buttons
*//*__________________________________________________________________________*/
#include "Scripts/MainMenuQuitButton.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
#include <Core_Engine.h>
#include <components/Texture.h>

// Constructor for the ButtonBase class.
// @param name: The name of the portal.
MainMenuQuitButton::MainMenuQuitButton(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the ButtonBase script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void MainMenuQuitButton::Start(Object* obj) {
    executeOnPause = true;
    std::cout << "ButtonBase Script Ready : " << obj->GetName() << std::endl;
}

/*********************************************************************/
// Update method called every frame to update the ButtonBase's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void MainMenuQuitButton::Update(Object* obj) {
    if (!input::IsPressed(KEY::mouseL) || obj == nullptr || !objectFactory->FindLayerThatHasThisObject(obj) || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
        //std::cout << "NIL OBJ : ButtonBase" << std::endl;
        return;
    }

    if (isObjectClicked((Transform*)obj->GetComponent(ComponentType::Transform), Vec2(input::GetMouseX(), input::GetMouseY()))) {
        std::cout << "Button Clicked" << std::endl;
        audio->playSfx("button_click");
        Message_Handler msg(MessageID::Event_Type::Quit);
        engine->Broadcast(&msg);
    }
}
/*********************************************************************/
// Shutdown method called when the ButtonBase script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void MainMenuQuitButton::Shutdown(Object* obj) {
    std::cout << "ButtonBase Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of ButtonBase.
MainMenuQuitButton mainMenuQuitButton("MainMenuQuitButton");


