/* !
@file	.cpp
@author Tan Yee Ann (t.yeeann@digipen.edu)
@date	26/2/2023

This file contains the script for the in-game clickable buttons
*//*__________________________________________________________________________*/
#include "Scripts/ChatBoxline1.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
#include <Core_Engine.h>
#include <SceneManager.h>
#include <../components/Texture.h>
#include <Factory.h>


// @param name: The name of the portal.
ChatBoxline1::ChatBoxline1(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the PauseButton script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void ChatBoxline1::Start(Object* obj) {
    executeOnPause = true;
    std::cout << "ChatBoxline1 Script Ready : " << obj->GetName() << std::endl;
}

/*********************************************************************/
// Update method called every frame to update the PauseButton's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void ChatBoxline1::Update(Object* obj) {
    if (obj == nullptr || !objectFactory->FindLayerThatHasThisObject(obj) || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
        return;
    }



}
/*********************************************************************/
// Shutdown method called when the PauseButton script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void ChatBoxline1::Shutdown(Object* obj) {
    std::cout << "ChatBoxline1 Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of PauseButton.
ChatBoxline1 chatboxline1 ("ChatBoxLine1");


