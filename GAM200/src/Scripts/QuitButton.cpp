/* !
@file	.cpp
@author 
@date	26/2/2023

This file contains the script for the in-game clickable buttons
*//*__________________________________________________________________________*/
#include "Scripts/QuitButton.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
#include <Core_Engine.h>
#include <SceneManager.h>

// @param name: The name of the portal.
QuitButton::QuitButton(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the quitButton script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void QuitButton::Start(Object* obj) {
    executeOnPause = true;
    std::cout << "QuitButton Script Ready : " << obj->GetName() << std::endl;
}

/*********************************************************************/
// Update method called every frame to update the QuitButton's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void QuitButton::Update(Object* obj) {
    if (obj == nullptr || !input::IsPressed(KEY::mouseL) || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
        //std::cout << "NIL OBJ : QuitButton" << std::endl;
        return;
    }

    if (isObjectClicked((Transform*) obj->GetComponent(ComponentType::Transform), Vec2(input::GetMouseX(), input::GetMouseY()))) {
		std::cout << "Button Clicked" << std::endl;
		audio->playSfx("button_click");

        //quit the game
        /*
        Message_Handler msg(MessageID::Event_Type::Quit);
        engine->Broadcast(&msg);

        objectFactory->GetLayer("PauseMenu")->second.first.isVisible = false;
        objectFactory->GetLayer("GameMenu")->second.first.isVisible = true;
        */

        sceneManager->LoadScene("main_menu.json");
        sceneManager->PlayScene();
	}
}
/*********************************************************************/
// Shutdown method called when the QuitButton script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void QuitButton::Shutdown(Object* obj) {
    std::cout << "QuitButton Script Shutdown : " << obj->GetName() << std::endl;
}

QuitButton quitButton("QuitButton");


