/* !
@author Yeo Jia Ming
@date	23/3/2024

This file contains the script for the next dialogue
*//*__________________________________________________________________________*/
#include "Scripts/ChatBoxNextButton.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
#include <Core_Engine.h>
#include <SceneManager.h>
#include <../components/Texture.h>
#include <Factory.h>
#include <../components/Dialogue.h>
#include <message.h>
#include <DialogueSystem.h>

// Constructor for the PauseButton class.
// @param name: The name of the portal.
ChatBoxNextButton::ChatBoxNextButton(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the PauseButton script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void ChatBoxNextButton::Start(Object* obj) {
    executeOnPause = true;
    std::cout << "ChatBoxNextButton Script Ready : " << obj->GetName() << std::endl;
}

/*********************************************************************/
// Update method called every frame to update the PauseButton's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void ChatBoxNextButton::Update(Object* obj) {
   
    if (obj == nullptr || !objectFactory->FindLayerThatHasThisObject(obj) || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
        return;
    }

    Texture* t = (Texture*)obj->GetComponent(ComponentType::Texture);
    
    static float opacity_inc = -0.01f;
    t->opacity += opacity_inc;
    if (t->opacity < 0.0f || t->opacity >1.0f)
    {
        opacity_inc = -opacity_inc;
    }

    if(!input::IsPressed(KEY::mouseL))
    {
        return;
    }

   


    if (input::IsPressed(KEY::mouseL))
    {
        audio->playSfx("button_click");
        dialoguesystem->next_dialogue = true;
    }
}
/*********************************************************************/
// Shutdown method called when the PauseButton script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void ChatBoxNextButton::Shutdown(Object* obj) {
    std::cout << "ChatBoxNextButton Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of PauseButton.
ChatBoxNextButton chatboxnextButton("ChatBoxNextButton");


