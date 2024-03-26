/* !
@file	.cpp
@author Tan Yee Ann (t.yeeann@digipen.edu)
@date	26/2/2023

This file contains the script for the in-game clickable buttons
*//*__________________________________________________________________________*/
#include "Scripts/WinNextButton.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
#include <Core_Engine.h>
#include <SceneManager.h>
#include <../components/Texture.h>
#include <Factory.h>

// Constructor for the PauseButton class.
// @param name: The name of the portal.
WinNextButton::WinNextButton(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the PauseButton script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void WinNextButton::Start(Object* obj) {
    executeOnPause = true;
    std::cout << "WinBackButton Script Ready : " << obj->GetName() << std::endl;
}

/*********************************************************************/
// Update method called every frame to update the PauseButton's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void WinNextButton::Update(Object* obj) {
    if (!input::IsPressed(KEY::mouseL) || obj == nullptr || !objectFactory->FindLayerThatHasThisObject(obj) || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
        //std::cout << "NIL OBJ : WinBackButton" << std::endl;
        return;
    }

    
    if(!isObjectClicked((Transform*)obj->GetComponent(ComponentType::Transform), Vec2(input::GetMouseX(), input::GetMouseY())))
    {
        return;
    }
    

    

    if (input::IsPressed(KEY::mouseL))
    {
        audio->playSfx("button_click");
        objectFactory->GetLayer("WinMenu")->second.first.isVisible = false;

        audio->restartBackgroundAudio();

        if (engine->loaded_level == "tutorial_level")
            sceneManager->LoadScene("level_1.json");
        else if (engine->loaded_level == "level_1")
            sceneManager->LoadScene("level_2.json");
        else if (engine->loaded_level == "level_2") {
            audio->setBackgroundAudio("main_menu_bg");
            sceneManager->LoadScene("main_menu.json");
        }
        sceneManager->PlayScene();
    }
}
/*********************************************************************/
// Shutdown method called when the PauseButton script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void WinNextButton::Shutdown(Object* obj) {
    std::cout << "WinNextButton Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of PauseButton.
WinNextButton winnextButton ("WinNextButton");


