/* !
@file P_liquid.cpp
@author Luke Goh
@date	27/11/2023

This file contains the script for the p_liquid for the human to move
*//*__________________________________________________________________________*/
#include "Scripts/PowerLiquid.h"
#include <components/Body.h>
#include <components/Animation.h>
#include <components/Event.h>
#include <Audio.h>
#include <Core_Engine.h>
#include <GameLogic.h>


// Constructor for the P_liquid class.
// @param name: The name of the p_liquid.

float counter;
float dt;

P_liquid::P_liquid(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the p_liquid script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void P_liquid::Start(Object* obj) {
    std::cout << "P_liquid Script Ready : " << obj->GetName() << std::endl;
    counter = 0.f;
    dt = engine->Get_Fixed_DT();
}

/*********************************************************************/
// Update method called every frame to update the p_liquid's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void P_liquid::Update(Object* obj) {
    if (obj == nullptr) {
        //std::cout << "NIL OBJ : P_liquid" << std::endl;
        return;
    }

    // Get the Animation component of the P_liquid.
    Animation* p_liquid_animation = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));
    Physics* p_liquid_phy = (Physics*)obj->GetComponent(ComponentType::Physics);
    Rectangular* p_liquid_b = static_cast<Rectangular*>(obj->GetComponent(ComponentType::Body));

    // Get the Transform components of the player and the P_liquid.
    Transform* player_t = static_cast<Transform*>(GameLogic::playerObj->GetComponent(ComponentType::Transform));
    Transform* p_liquid_t = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));


    if (player_t == nullptr || p_liquid_t == nullptr) {
        std::cout << "NIL COMPONENT : P_liquid" << std::endl;
        return;
    }

    // Check if the player is within the p_liquid's range.
    if (player_t->Position.x > p_liquid_t->Position.x - 64.f && player_t->Position.x < p_liquid_t->Position.x + 64.f
        && player_t->Position.y > p_liquid_t->Position.y - 64.f && player_t->Position.y < p_liquid_t->Position.y + 64.f) {
        if (((p_liquid_b->collision_flag & COLLISION_RIGHT) || (p_liquid_b->collision_flag & COLLISION_LEFT)) || (p_liquid_b->collision_flag & COLLISION_TOP)) {
            objectFactory->destroyObject(obj);
            CatPower++;
            return;
        }
        
    }
   


}
/*********************************************************************/
// Shutdown method called when the p_liquid script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void P_liquid::Shutdown(Object* obj) {
    std::cout << "P_liquid Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of P_liquid.
P_liquid p_liquid("Power_Liquid");


