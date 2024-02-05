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
    std::cout << "Box Script Ready : " << obj->GetName() << std::endl;
}

/*********************************************************************/
// Update method called every frame to update the box's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void Box::Update(Object* obj) {
    if (obj == nullptr) {
        //std::cout << "NIL OBJ : Box" << std::endl;
        return;
    }

    // Get the Animation component of the Box.
    Animation* box_animation = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));
    Physics* box_phy = (Physics*)obj->GetComponent(ComponentType::Physics);
    Rectangular* box_b = static_cast<Rectangular*>(obj->GetComponent(ComponentType::Body));

    // Find the player object in the game.
    Object* Checkplayer = objectFactory->FindObject("Finn");
    if (GameLogic::playerObj != nullptr && GameLogic::playerObj == Checkplayer)
    {
        // Get the Transform components of the player and the Box.
        Transform* player_t = static_cast<Transform*>(GameLogic::playerObj->GetComponent(ComponentType::Transform));
        Physics* finn_phy = (Physics*)Checkplayer->GetComponent(ComponentType::Physics);
        Transform* box_t = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));


        if (player_t == nullptr || box_t == nullptr) {
            std::cout << "NIL COMPONENT : Box" << std::endl;
            return;
        }

        // Check if the player is within the box's range.
        if (player_t->Position.x > box_t->Position.x - 50 && player_t->Position.x < box_t->Position.x + 50
            && player_t->Position.y > box_t->Position.y - 50 && player_t->Position.y < box_t->Position.y + 50) {
            if ((box_b->collision_flag & COLLISION_RIGHT) || (box_b->collision_flag & COLLISION_LEFT)) {
                box_phy->Velocity.x = finn_phy->Velocity.x * 0.2f;
                if(box_phy->Velocity.x != 0.0f)
                    std::cout << "Box is moving" << std::endl;
            }
            
        }
        // Else, stop the box.
        else {
            box_phy->Velocity.x = 0.0f;
        }
    }
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


