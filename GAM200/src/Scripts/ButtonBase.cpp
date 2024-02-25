/* !
@file	ButtonBase.cpp
@author Tan Yee Ann (t.yeeann@digipen.edu)
@date	26/2/2023

This file contains the script for the in-game clickable buttons
*//*__________________________________________________________________________*/
#include "Scripts/ButtonBase.h"
#include <Utils.h>
#include <Audio.h>

// Constructor for the ButtonBase class.
// @param name: The name of the portal.
ButtonBase::ButtonBase(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the ButtonBase script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void ButtonBase::Start(Object* obj) {
    std::cout << "ButtonBase Script Ready : " << obj->GetName() << std::endl;
}

/*********************************************************************/
// Update method called every frame to update the ButtonBase's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void ButtonBase::Update(Object* obj) {
    if (obj == nullptr || !input::IsPressed(KEY::mouseL)) {
        //std::cout << "NIL OBJ : ButtonBase" << std::endl;
        return;
    }

    if (isObjectClicked((Transform*) obj->GetComponent(ComponentType::Transform), Vec2(input::GetMouseX(), input::GetMouseY()))) {
		std::cout << "Button Clicked" << std::endl;
		audio->playSfx("button_click");
	}
}
/*********************************************************************/
// Shutdown method called when the ButtonBase script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void ButtonBase::Shutdown(Object* obj) {
    std::cout << "ButtonBase Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of ButtonBase.
ButtonBase ButtonBase("ButtonBase");


