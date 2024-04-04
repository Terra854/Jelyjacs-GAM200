/* !
@file Box.cpp
@author Luke Goh
@date	27/11/2023

This file contains the script for the box for the human to move
*//*__________________________________________________________________________*/
#include <Debug.h>
#include "Scripts/Box.h"
#include <components/Body.h>
#include <components/Animation.h>
#include <components/Event.h>
#include <Audio.h>
#include <iostream>

float MovingBoxSfxCooldown = 0.f;

std::map<int, float> LandingBoxSfxCooldown;
std::map<int, float> BoxInTheAir;

FMOD::ChannelGroup* box_sfx;

// Constructor for the Box class.
// @param name: The name of the box.
Box::Box(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the box script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void Box::Start(Object* obj) {
    audio->createChannelGroup("box_sfx", box_sfx);
    std::cout << "Box Script Ready : " << obj->GetName() << std::endl;
}

/*********************************************************************/
// Update method called every frame to update the box's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void Box::Update(Object* obj) {
    MovingBoxSfxCooldown -= engine->GetDt();
    MovingBoxSfxCooldown = MovingBoxSfxCooldown < 0.f ? 0.f : MovingBoxSfxCooldown;

    if (obj == nullptr) {
        //std::cout << "NIL OBJ : Box" << std::endl;
        return;
    }

    Transform* box_t = (Transform*)obj->GetComponent(ComponentType::Transform);
    Rectangular* box_b = static_cast<Rectangular*>(obj->GetComponent(ComponentType::Body));
    Physics* box_phy = (Physics*)obj->GetComponent(ComponentType::Physics);
    Event* box_event = static_cast<Event*>(obj->GetComponent(ComponentType::Event));

    Object* finn = objectFactory->FindObject("Finn");

    // Check for nullptrs
    if (!box_b || !box_t || !box_event)
        return;

    LandingBoxSfxCooldown[box_event->linked_event] -= engine->GetDt();
    LandingBoxSfxCooldown[box_event->linked_event] = LandingBoxSfxCooldown[box_event->linked_event] < 0.f ? 0.f : LandingBoxSfxCooldown[box_event->linked_event];

    // SFX for when the box is moving
    if (!MovingBoxSfxCooldown && box_phy->IsBeingPushed)
    {
        audio->playSfx("box_moving", box_sfx);
        MovingBoxSfxCooldown = 4.0f;
    }
    else if (!static_cast<Physics*>(finn->GetComponent(ComponentType::Physics))->Velocity.x) {
        audio->stopSfx(box_sfx);
        MovingBoxSfxCooldown = 0.f;
    }

    // SFX for when the box lands from a drop
    if (!box_b->bottom_collision) {
        BoxInTheAir[box_event->linked_event] += engine->GetDt();
        LandingBoxSfxCooldown[box_event->linked_event] = 0.0f;
    }
    else if (box_b->bottom_collision && BoxInTheAir[box_event->linked_event] > 0.1f && !LandingBoxSfxCooldown[box_event->linked_event]) {
		BoxInTheAir[box_event->linked_event] = 0.f;
		audio->playSfx("box_landing_from_drop", 2.f);
        LandingBoxSfxCooldown[box_event->linked_event] = 3.0f;
	}
    /*
    
    Pushing/Pulling logic all moved to Physics. Will be deleting this ltr

    Physics* box_phy = (Physics*)obj->GetComponent(ComponentType::Physics);
    Rectangular* box_b = static_cast<Rectangular*>(obj->GetComponent(ComponentType::Body));

    // Find the player object in the game.
    Object* Checkplayer = objectFactory->FindObject("Finn");
    if (GameLogic::playerObj != nullptr && GameLogic::playerObj == Checkplayer)
    {
        // Get the Transform components of the player and the Box.
        Transform* player_t = static_cast<Transform*>(GameLogic::playerObj->GetComponent(ComponentType::Transform));
        Rectangular* player_b = static_cast<Rectangular*>(GameLogic::playerObj->GetComponent(ComponentType::Body));
        Physics* finn_phy = (Physics*)Checkplayer->GetComponent(ComponentType::Physics);
        Transform* box_t = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));


        if (player_t == nullptr || box_t == nullptr) {
            std::cout << "NIL COMPONENT : Box" << std::endl;
            return;
        }
        /*
        // Check if the player is within the box's range.
        if (player_t->Position.x > box_t->Position.x - 50 && player_t->Position.x < box_t->Position.x + 50
            && player_t->Position.y > box_t->Position.y - 50 && player_t->Position.y < box_t->Position.y + 50) {
            if ((box_b->collision_flag & COLLISION_RIGHT) || (box_b->collision_flag & COLLISION_LEFT)) {
                box_phy->Velocity.x = finn_phy->Velocity.x * 0.2f;
                if(box_phy->Velocity.x != 0.0f)
                    std::cout << "Box is moving" << std::endl;
            }
            
        }
        // Check if the player is within the box's range.
        if (((player_b->collision_flag & COLLISION_RIGHT) && static_cast<Rectangular*>(player_b->right_collision->GetComponent(ComponentType::Body)) == box_b) || (player_b->collision_flag & COLLISION_LEFT) && static_cast<Rectangular*>(player_b->left_collision->GetComponent(ComponentType::Body)) == box_b) {
            box_phy->Velocity.x = finn_phy->Velocity.x * 0.2f;
            if (box_phy->Velocity.x)
                std::cout << "Box is moving" << std::endl;

            if (input::IsPressedRepeatedly(KEY::k)) {
                finn_phy->Velocity.x = box_phy->Velocity.x;
                std::cout << "Pulling" << std::endl;
            }
        }
        // Else, stop the box.
        else {
            if (box_b->collision_flag & COLLISION_BOTTOM)
                box_phy->Velocity.x = 0.0f;
        }
    }
    */
}
/*********************************************************************/
// Shutdown method called when the box script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void Box::Shutdown(Object* obj) {
    std::cout << "Box Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of Box.
Box box("Box");


