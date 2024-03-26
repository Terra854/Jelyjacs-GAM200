/* !
@file	.cpp
@author Tan Yee Ann (t.yeeann@digipen.edu)
@date	26/2/2023

This file contains the script for the in-game clickable buttons
*//*__________________________________________________________________________*/
#include "Scripts/ZoomButton.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
#include <Core_Engine.h>
#include <SceneManager.h>
#include <../components/Texture.h>
#include <Factory.h>

// Constructor for the PauseButton class.
// @param name: The name of the portal.
ZoomButton::ZoomButton(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the PauseButton script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void ZoomButton::Start(Object* obj) {
    executeOnPause = true;
    std::cout << " Script Ready : " << obj->GetName() << std::endl;
}

/*********************************************************************/
// Update method called every frame to update the PauseButton's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void ZoomButton::Update(Object* obj) {
    if (!input::IsPressed(KEY::mouseL) || obj == nullptr || !objectFactory->FindLayerThatHasThisObject(obj) || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
        //std::cout << "NIL OBJ : " << std::endl;
        return;
    }

    if(!isObjectClicked((Transform*)obj->GetComponent(ComponentType::Transform), Vec2(input::GetMouseX(), input::GetMouseY())))
    {
        return;
    }
    

    if (input::IsPressed(KEY::mouseL))
    {
        audio->playSfx("button_click");
        if (camera2D->scale.x == 1.0f || camera2D->scale.y == 1.0f) {
            camera2D->scale = { 2.0f, 2.0f };
        }
        else camera2D->scale = { 1.0f, 1.0f };
    }
}
/*********************************************************************/
// Shutdown method called when the PauseButton script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void ZoomButton::Shutdown(Object* obj) {
    std::cout << " Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of PauseButton.
ZoomButton zoomButton ("ZoomButton");


