/* !
@file	.cpp
@author Tan Yee Ann (t.yeeann@digipen.edu)
@date	26/2/2023

This file contains the script for the in-game clickable buttons
*//*__________________________________________________________________________*/
#include "Scripts/HowToPlayButton.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
#include <Core_Engine.h>
#include <SceneManager.h>
#include <../components/Texture.h>
#include <Factory.h>

// Constructor for the PauseButton class.
// @param name: The name of the portal.
HowToPlayButton::HowToPlayButton(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the PauseButton script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void HowToPlayButton::Start(Object* obj) {
    executeOnPause = true;
    std::cout << "HowToPlay Script Ready : " << obj->GetName() << std::endl;
}

/*********************************************************************/
// Update method called every frame to update the PauseButton's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void HowToPlayButton::Update(Object* obj) {
    if (obj == nullptr || !objectFactory->FindLayerThatHasThisObject(obj) || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
        //std::cout << "NIL OBJ : QuitButton" << std::endl;
        return;
    }


    Texture* tex = static_cast<Texture*>(obj->GetComponent(ComponentType::Texture));

    
    if(!isObjectClicked((Transform*)obj->GetComponent(ComponentType::Transform), Vec2(input::GetMouseX(), input::GetMouseY())))
    {
        tex->textureName = "How to play.png";
        return;
    }
    

    //tex->textureName = "How to play glow.png";
    

    if (input::IsPressed(KEY::mouseL))
    {
        audio->playSfx("button_click");
        objectFactory->GetLayer("PauseMenu")->second.first.isVisible = false;
        objectFactory->GetLayer("HowToPlayMenu")->second.first.isVisible = true;
    }
}
/*********************************************************************/
// Shutdown method called when the PauseButton script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void HowToPlayButton::Shutdown(Object* obj) {
    std::cout << "PauseButton Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of PauseButton.
HowToPlayButton howtoplayButton("HowToPlayButton");


