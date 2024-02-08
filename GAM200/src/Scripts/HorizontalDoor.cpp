/* !
@file	HorizontalDoor.cpp
@author Tan Yee Ann
@date	8/2/2023

This file contains the script for the horizontal doors
*//*__________________________________________________________________________*/
#include "Scripts/HorizontalDoor.h"
#include <components/Body.h>
#include <components/Animation.h>
#include <components/Event.h>
#include <Audio.h>

// Constructor for the HorizontalDoor class.
// @param name: The name of the portal.
HorizontalDoor::HorizontalDoor(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the HorizontalDoor script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void HorizontalDoor::Start(Object* obj) {
    std::cout << "HorizontalDoor Script Ready : " << obj->GetName() << std::endl;
}

/*********************************************************************/
// Update method called every frame to update the HorizontalDoor's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void HorizontalDoor::Update(Object* obj) {
    if (obj == nullptr) {
        //std::cout << "NIL OBJ : HorizontalDoor" << std::endl;
        return;
    }

    Animation* HorizontalDoor_a = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));
    Body* HorizontalDoor_b = static_cast<Body*>(obj->GetComponent(ComponentType::Body));

    // Make sure the components are not nullptr
    if (HorizontalDoor_a == nullptr || HorizontalDoor_b == nullptr)
        return;

    // Disable collision only when the horizontal doors are fully retracted
    HorizontalDoor_b->active = (HorizontalDoor_a->frame_num != 4);
    
}
/*********************************************************************/
// Shutdown method called when the HorizontalDoor script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void HorizontalDoor::Shutdown(Object* obj) {
    std::cout << "HorizontalDoor Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of HorizontalDoor.
HorizontalDoor horizontalDoor("HorizontalDoor");


