/* !
@file	LaserDoor.cpp
@author Tan Yee Ann
@date	8/2/2023

This file contains the script for the laser "doors"
*//*__________________________________________________________________________*/
#include "Scripts/LaserDoor.h"
#include <components/Body.h>
#include <components/Animation.h>
#include <components/Event.h>
#include <Audio.h>

static float LaserOffSfxCooldown = 0.f;

// Constructor for the LaserDoor class.
// @param name: The name of the portal.
LaserDoor::LaserDoor(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the LaserDoor script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void LaserDoor::Start(Object* obj) {
    std::cout << "LaserDoor Script Ready : " << obj->GetName() << std::endl;
}

/*********************************************************************/
// Update method called every frame to update the LaserDoor's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void LaserDoor::Update(Object* obj) {
    LaserOffSfxCooldown -= engine->GetDt();
    LaserOffSfxCooldown = LaserOffSfxCooldown < 0.f ? 0.f : LaserOffSfxCooldown;

    if (obj == nullptr) {
        //std::cout << "NIL OBJ : LaserDoor" << std::endl;
        return;
    }

    Animation* LaserDoor_a = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));
    Body* LaserDoor_b = static_cast<Body*>(obj->GetComponent(ComponentType::Body));

    // Make sure the components are not nullptr
    if (LaserDoor_a == nullptr || LaserDoor_b == nullptr)
        return;

    // Disable collision only when the laser is off
    LaserDoor_b->active = (LaserDoor_a->frame_num != 2);

    if (LaserDoor_a->frame_num == 1 && !LaserOffSfxCooldown) {
        audio->playSfx("laser_off");
        LaserOffSfxCooldown = 1.5f;
    }
    
}
/*********************************************************************/
// Shutdown method called when the LaserDoor script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void LaserDoor::Shutdown(Object* obj) {
    std::cout << "LaserDoor Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of LaserDoor.
LaserDoor laserDoor("LaserDoor");


