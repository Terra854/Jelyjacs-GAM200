/* !
@file	Door.cpp
@author Tan Yee Ann
@date	8/2/2023

This file contains the script for the doors
*//*__________________________________________________________________________*/
#include "Scripts/Door.h"
#include <components/Body.h>
#include <components/Animation.h>
#include <components/Event.h>
#include <Audio.h>

static float DoorSfxCooldown = 0.f;

// Constructor for the Door class.
// @param name: The name of the portal.
Door::Door(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the door script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void Door::Start(Object* obj) {
    std::cout << "Door Script Ready : " << obj->GetName() << std::endl;
}

/*********************************************************************/
// Update method called every frame to update the door's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void Door::Update(Object* obj) {
    DoorSfxCooldown -= engine->GetDt();
    DoorSfxCooldown = DoorSfxCooldown < 0.f ? 0.f : DoorSfxCooldown;

    if (obj == nullptr) {
        //std::cout << "NIL OBJ : Door" << std::endl;
        return;
    }

    Animation* door_a = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));
    Body* door_b = static_cast<Body*>(obj->GetComponent(ComponentType::Body));

    // Make sure the components are not nullptr
    if (door_a == nullptr || door_b == nullptr)
        return;

    if (((!door_a->reverse && door_a->frame_num == 22) || (door_a->reverse && door_a->frame_num == 27)) && !DoorSfxCooldown) {
        audio->playSfx("sliding_door_open");
        DoorSfxCooldown = 1.5f; // Make sure the audio only plays once
    }

    // Disable collision only when the door is fully opened
    door_b->active = (door_a->frame_num < 28);
    
}
/*********************************************************************/
// Shutdown method called when the door script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void Door::Shutdown(Object* obj) {
    std::cout << "Door Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of Door.
Door door("Door");


