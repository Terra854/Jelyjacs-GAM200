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



// For fixed physics
float box_script_accumulator = 0.f;
float box_script_fixed_dt = 1.f / 60.f;
int box_script_num_of_steps = 0;

/*********************************************************************/
// Update method called every frame to update the box's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void Box::Update(Object* obj) {
    if (obj == nullptr) {
        //std::cout << "NIL OBJ : Box" << std::endl;
        return;
    }

    box_script_accumulator += engine->GetDt();

    // Only run the physics code if fixed_dt has passed 
    if (box_script_accumulator < box_script_fixed_dt) {
        return;
    }

    // Check and see how many loops the physics needs to update
    while (box_script_accumulator > box_script_fixed_dt) {
        box_script_num_of_steps++;
        box_script_accumulator -= box_script_fixed_dt;
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
            */
            // Check if the player is within the box's range.
                if (((player_b->collision_flag & COLLISION_RIGHT) && static_cast<Rectangular*>(player_b->right_collision->GetComponent(ComponentType::Body)) == box_b) || (player_b->collision_flag & COLLISION_LEFT) && static_cast<Rectangular*>(player_b->left_collision->GetComponent(ComponentType::Body)) == box_b) {
                    for (; box_script_num_of_steps; box_script_num_of_steps--) {
                        box_t->Position.x += finn_phy->Velocity.x * 0.2f * box_script_fixed_dt;
                        if (box_phy->Velocity.x != 0.0f)
                            std::cout << "Box is moving" << std::endl;
                    }
                }
                // Else, stop the box.
                else {
                    if (box_b->collision_flag & COLLISION_BOTTOM)
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


