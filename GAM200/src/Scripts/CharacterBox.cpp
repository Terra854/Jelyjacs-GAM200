/* !
@file	.cpp
@author 
@date	26/2/2023

This file contains the script for the in-game clickable buttons
*//*__________________________________________________________________________*/
#include "Scripts/CharacterBox.h"
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
CharacterBox::CharacterBox(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the PauseButton script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void CharacterBox::Start(Object* obj) {
    executeOnPause = true;
}

/*********************************************************************/
// Update method called every frame to update the PauseButton's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void CharacterBox::Update(Object* obj) {
    if (obj == nullptr || !objectFactory->FindLayerThatHasThisObject(obj) || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
        return;
    }
  
    
}
/*********************************************************************/
// Shutdown method called when the PauseButton script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void CharacterBox::Shutdown(Object* obj) {
    std::cout << "CharacterBox Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of PauseButton.
CharacterBox characterBox ("CharacterBox");


