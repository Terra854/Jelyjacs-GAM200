/* !
@file	.cpp
@author 
@date	26/2/2023

This file contains the script for the in-game clickable buttons
*//*__________________________________________________________________________*/
#include "Scripts/ChatBox.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
#include <Core_Engine.h>
#include <SceneManager.h>
#include <../components/Texture.h>
#include <Factory.h>
#include <Font.h>

// Constructor for the PauseButton class.
// @param name: The name of the portal.
ChatBox::ChatBox(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the PauseButton script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void ChatBox::Start(Object* obj) {
    executeOnPause = true;
    std::cout << "HowToPlay Script Ready : " << obj->GetName() << std::endl;
}

/*********************************************************************/
// Update method called every frame to update the PauseButton's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void ChatBox::Update(Object* obj) {
    if (obj == nullptr || !objectFactory->FindLayerThatHasThisObject(obj) || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
        return;
    }
    //std::cout << "chatbox!!!!!!" << std::endl;
    
    Object* finn_obj = objectFactory->FindObject("Finn");
    Transform* finn_pos = (Transform*)finn_obj->GetComponent(ComponentType::Transform);
    Transform hidden_hitbox{};
    hidden_hitbox.Position = { 0,0 };
    hidden_hitbox.Scale = { 50,50};

    //Transform* hidden_hitbox{ };

    if(isObjectClicked(finn_pos, Vec2(finn_pos->Position.x, finn_pos->Position.x)))
    {
        std::cout << "in hitbox!!!!" << std::endl;
        
    }
    else
    {
        std::cout << "not in hitbox" << std::endl;

    }
    
}
/*********************************************************************/
// Shutdown method called when the PauseButton script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void ChatBox::Shutdown(Object* obj) {
    std::cout << "ChatBox Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of PauseButton.
ChatBox chatbox ("ChatBox");


