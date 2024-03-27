/* !
@file	Dialogue.cpp
@author Tan Yee Ann (t.yeeann@digipen.edu)
@date	26/2/2023

This file contains the script for the in-game clickable buttons
*//*__________________________________________________________________________*/
#include "Scripts/DialogueSequence.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>


enum sequence
{
    opening,
    elevator,
    win
};

sequence dialogue = opening;

// Constructor for the ButtonBase class.
// @param name: The name of the portal.
DialogueSequence::DialogueSequence(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
    sequence dialogue = opening;

}
/*********************************************************************/
// Start method called when the ButtonBase script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void DialogueSequence::Start(Object* obj) {
    executeOnPause = true;
    std::cout << "DialogueSequence Script Ready : " << obj->GetName() << std::endl;
}

/*********************************************************************/
// Update method called every frame to update the ButtonBase's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void DialogueSequence::Update(Object* obj) {
    if (obj == nullptr)  {
        return;
    }

  

    switch (dialogue)
    {
    case opening:
    {
            Object* finn_obj = objectFactory->FindObject("Finn");
            Transform* finn_trans = (Transform*)finn_obj->GetComponent(ComponentType::Transform);
            if (isObjectClicked((Transform*)obj->GetComponent(ComponentType::Transform), finn_trans->Position)) 
            {
                objectFactory->GetLayer("ChatBoxMenu")->second.first.isVisible = true;
            }
            dialogue = elevator;
    }
    break;
    }
    
}
/*********************************************************************/
// Shutdown method called when the ButtonBase script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void DialogueSequence::Shutdown(Object* obj) {
    std::cout << "DialogueSequence Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of ButtonBase.
DialogueSequence dialogueSequence("DialogueSequence");


