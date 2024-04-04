/* !
@file CatPortal.cpp
@author Luke Goh
@date	27/11/2023

This file contains the script for the portal for the cat character to teleport
*//*__________________________________________________________________________*/
#include "Scripts/CatPortal.h"
#include <components/Body.h>
#include <components/Animation.h>
#include <components/Event.h>
#include <Audio.h>
#include <Scripts/Spark.h>

namespace CatPortal_Script {
    /*********************************************************************/
    // Definition of a static variable to track the teleportation status.
    /*********************************************************************/
    bool CatPortal::justTeleported = false;

    // Constructor for the CatPortal class.
    // @param name: The name of the portal.
    CatPortal::CatPortal(std::string name) : LogicScript(name)
    {
        std::cout << name << " Created" << std::endl;
    }
    /*********************************************************************/
    // Start method called when the portal script is ready.
    // @param obj: The object to which this script is attached.
    /*********************************************************************/
    void CatPortal::Start(Object* obj) {
        std::cout << "CatPortal Script Ready : " << obj->GetName() << std::endl;
    }

    /*********************************************************************/
    // Update method called every frame to update the portal's logic.
    // @param obj: The object to which this script is attached.
    /*********************************************************************/
    void CatPortal::Update(Object* obj) {
        if (obj == nullptr) {
            //std::cout << "NIL OBJ : CatPortal" << std::endl;
            return;
        }

        // Get the Animation component of the CatPortal.
        Animation* catPortal_animation = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));
        //catPortal_animation->fixed = true;
        //catPortal_animation->current_type = AnimationType::Jump;


        // Find the player object in the game.
        Object* Spark = objectFactory->FindObject("Spark");
        if (GameLogic::playerObj != nullptr && GameLogic::playerObj == Spark)
        {
            // Get the Transform components of the player and the CatPortal.
            Transform* player_t = static_cast<Transform*>(GameLogic::playerObj->GetComponent(ComponentType::Transform));
            Transform* catPortal_t = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));

            Physics* player_p = static_cast<Physics*>(GameLogic::playerObj->GetComponent(ComponentType::Physics));
            Animation* player_a = static_cast<Animation*>(GameLogic::playerObj->GetComponent(ComponentType::Animation));

            // Initialize variables to hold the other CatPortal and its Transform.
            Object* otherCatPortal = nullptr;
            Transform* otherCatPortal_t = nullptr;

            // Search for another CatPortal object.
            for (int i = 0; i < objectFactory->NumberOfObjects(); i++)
            {
                Object* temp_obj = objectFactory->getObjectWithID((long)i);
                if (temp_obj == nullptr)
                    return;
                if (static_cast<Behaviour*>(temp_obj->GetComponent(ComponentType::Behaviour)) != nullptr) {
                    if (static_cast<Behaviour*>(temp_obj->GetComponent(ComponentType::Behaviour))->GetBehaviourName() == "CatPortal" && temp_obj != obj)
                    {
                        otherCatPortal = temp_obj;
                        otherCatPortal_t = static_cast<Transform*>(otherCatPortal->GetComponent(ComponentType::Transform));
                    }
                }
            }

            if (player_t == nullptr || catPortal_t == nullptr) {
                std::cout << "NIL COMPONENT : CatPortal" << std::endl;
                return;
            }
            if (otherCatPortal == nullptr || otherCatPortal_t == nullptr) {
                std::cout << "NIL OTHER CAT PORTAL : CatPortal" << std::endl;
                return;
            }

            // Check if the player is within the portal's range.
            if (player_t->Position.x > catPortal_t->Position.x - 50 && player_t->Position.x < catPortal_t->Position.x + 50
                && player_t->Position.y > catPortal_t->Position.y - 50 && player_t->Position.y < catPortal_t->Position.y + 50) {
                if (justTeleported == true)
                    return;
                else {
                    //std::cout << "Player is in the cat portal" << std::endl;
                    //Find the other cat portal
                        // Teleport the player to the other CatPortal's position.
                    audio->playSfx("cat_teleport", 2.f);

                    if (player_a != nullptr) {
						player_a->current_type = AnimationType::Push;
                        player_a->fixed = true;
					}
                    Spark::teleporting = true;
                    Spark::teleporting_state = Disappearing;
                    Spark::next_position = otherCatPortal_t->Position;
                    justTeleported = true;
                    
                    if (player_p != nullptr) {
                        player_p->Velocity.x = 0;
                        player_p->Velocity.y = 0;
                    }

                }
            }
            else if ((player_t->Position.x > otherCatPortal_t->Position.x - 50 && player_t->Position.x < otherCatPortal_t->Position.x + 50)
                && (player_t->Position.y > otherCatPortal_t->Position.y - 50 && player_t->Position.y < otherCatPortal_t->Position.y + 50))
            {}
            else {
                // Reset teleportation status and animation when the player is not near the portal.
                if (catPortal_animation != nullptr) {
                    catPortal_animation->fixed = false;
                    catPortal_animation->current_type = AnimationType::Idle;
                }
                justTeleported = false;
            }
        }
    }
    /*********************************************************************/
    // Shutdown method called when the portal script is being shut down.
    // @param obj: The object to which this script is attached.
    /*********************************************************************/
    void CatPortal::Shutdown(Object* obj) {
        std::cout << "CatPortal Script Shutdown : " << obj->GetName() << std::endl;
    }

    // Creating an instance of CatPortal.
    CatPortal catPortal("CatPortal");

} // namespace CatPortal_Script

