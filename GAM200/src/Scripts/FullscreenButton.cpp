/* !
@file	.cpp
@author
@date	26/2/2023

This file contains the script for the in-game clickable buttons
*//*__________________________________________________________________________*/
#include "Scripts/FullscreenButton.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
#include <Core_Engine.h>
#include <SceneManager.h>
#include <../components/Texture.h>
#include <Object.h>

// @param name: The name of the portal.
FullscreenButton::FullscreenButton(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the quitButton script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void FullscreenButton::Start(Object* obj) {
    executeOnPause = true;
    std::cout << "FullscreenButton Script Ready : " << obj->GetName() << std::endl;
}

/*********************************************************************/
// Update method called every frame to update the QuitButton's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void FullscreenButton::Update(Object* obj) {





    if (!input::IsPressed(KEY::mouseL) || obj == nullptr || !objectFactory->FindLayerThatHasThisObject(obj) || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
        //std::cout << "NIL OBJ : QuitButton" << std::endl;
        return;
    }

    Transform* trans = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));
    Texture* tex = static_cast<Texture*>(obj->GetComponent(ComponentType::Texture));
    
    Vec2 pos1{ trans->Position.x - trans->Scale.x / 2.0f  , trans->Position.y - trans->Scale.y / 2.0f };
    Vec2 pos2{ trans->Position.x + trans->Scale.x / 2.0f  , trans->Position.y + trans->Scale.y / 2.0f };
    if (input::GetMouseX() < pos1.x || input::GetMouseX() > pos2.x || input::GetMouseY() < pos1.y || input::GetMouseY() > pos2.y)
    {
        tex->textureName = "Fullscreen.png";
        return;
    }
    

    /*
    if(!isObjectClicked((Transform*)obj->GetComponent(ComponentType::Transform), Vec2(input::GetMouseX(), input::GetMouseY())))
    {
        tex->textureName = "Fullscreen.png";
        return;
    }
    */

    tex->textureName = "Fullscreen glow.png";
    

    if (input::IsPressed(KEY::mouseL))
    {
		std::cout << "Button Clicked" << std::endl;
		audio->playSfx("button_click");
        if (window->window_size == Window_size::fullscreen)
        {
            window->change_window_size(Window_size::high);
            window->window_size = Window_size::high;
        }
        else
        {
            window->change_window_size_fullscreen();
            window->window_size = Window_size::fullscreen;
        }
        input::update_resolution();
	}
}
/*********************************************************************/
// Shutdown method called when the QuitButton script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void FullscreenButton::Shutdown(Object* obj) {
    std::cout << "QuitButton Script Shutdown : " << obj->GetName() << std::endl;
}

FullscreenButton fullscreenButton("FullscreenButton");


