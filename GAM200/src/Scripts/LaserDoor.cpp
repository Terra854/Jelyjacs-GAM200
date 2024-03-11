/* !
@file	LaserDoor.cpp
@author Tan Yee Ann (t.yeeann@digipen.edu)
@date	8/2/2023

This file contains the script for the laser "doors"
*//*__________________________________________________________________________*/
#include "Scripts/LaserDoor.h"
#include <components/Body.h>
#include <components/Animation.h>
#include <components/Event.h>
#include <Audio.h>
#include <SceneManager.h>

namespace LaserDoor_Script {
    static float LaserOffSfxCooldown = 0.f;
    bool intersecting; // For checking if the player is intersecting with the laser door
    bool doorswitch; // For checking if the door just closed;

    bool Intersects(Object* obj1, Object* obj2) {
        Transform* obj1_t = static_cast<Transform*>(obj1->GetComponent(ComponentType::Transform));
        Transform* obj2_t = static_cast<Transform*>(obj2->GetComponent(ComponentType::Transform));
        Rectangular* obj1_b = static_cast<Rectangular*>(obj1->GetComponent(ComponentType::Body));
        Rectangular* obj2_b = static_cast<Rectangular*>(obj2->GetComponent(ComponentType::Body));

        if (obj1_t == nullptr || obj2_t == nullptr || obj1_b == nullptr || obj2_b == nullptr)
            return false;
        /*
        float obj1_x = obj1_t->Position.x;
        float obj1_y = obj1_t->Position.y;
        float obj1_w = obj1_b->width / 2;
        float obj1_h = obj1_b->height / 2;

        float obj2_x = obj2_t->Position.x;
        float obj2_y = obj2_t->Position.y;
        float obj2_w = obj2_b->width / 2;
        float obj2_h = obj2_b->height / 2;

        Vec2 obj1_bl = { obj1_x - obj1_w, obj1_y - obj1_h };
        Vec2 obj1_tr = { obj1_x + obj1_w, obj1_y + obj1_h };
        Vec2 obj2_bl, obj2_tr;
        */
        
        Vec2 obj1_bl = obj1_b->aabb.min;
        Vec2 obj1_tr = obj1_b->aabb.max;

        Vec2 obj2_bl = obj2_b->aabb.min;
        Vec2 obj2_tr = obj2_b->aabb.max;
        
        if (obj1_bl.x >= obj2_tr.x || obj1_tr.x <= obj2_bl.x)
            return false;

        if (obj1_bl.y >= obj2_tr.y || obj1_tr.y <= obj2_bl.y)
            return false;

        std::cout << "Bottom Left of obj 1 : " << obj1_bl.x << ", " << obj1_bl.y << std::endl;
        std::cout << "Top Right of obj 1 : " << obj1_tr.x << ", " << obj1_tr.y << std::endl;
        std::cout << "Bottom Left of obj 2 : " << obj2_bl.x << ", " << obj2_bl.y << std::endl;
        std::cout << "Top Right of obj 2 : " << obj2_tr.x << ", " << obj2_tr.y << std::endl;

        // Check if the two objects intersect
        return true;
    }

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
        bool intersecting = false;
        bool doorswitch = false;
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

        //std::cout << obj->GetName() << "'s : " << LaserDoor_a->frame_num << std::endl;
        // Disable collision only when the laser is off
        if (LaserDoor_b->active && doorswitch) {
            Object* Finn = objectFactory->FindObject("Finn");
            Object* Spark = objectFactory->FindObject("Spark");
            if (Finn != nullptr && Spark != nullptr) {
                /*
                if (Intersects(Finn, obj)) {
                    intersecting = true;
                }
                
                if (Intersects(Spark, obj)) {
                    intersecting = true;
                }*/
                if (Collision::IsObjectInsideAnotherObject(static_cast<Transform*>(Finn->GetComponent(ComponentType::Transform)), static_cast<Rectangular*>(obj->GetComponent(ComponentType::Body)))) {
					intersecting = true;
				}
                if (Collision::IsObjectInsideAnotherObject(static_cast<Transform*>(Spark->GetComponent(ComponentType::Transform)), static_cast<Rectangular*>(obj->GetComponent(ComponentType::Body)))) {
                    intersecting = true;
                }
            }
            for (auto& temp : objectFactory->FindAllObjectsByName("Box")) {
                /*
                if (Intersects(temp, obj)) {
                    intersecting = true;
                }
                */
                if (Collision::IsObjectInsideAnotherObject(static_cast<Transform*>(temp->GetComponent(ComponentType::Transform)), static_cast<Rectangular*>(obj->GetComponent(ComponentType::Body)))) {
					intersecting = true;
				}
            }
            doorswitch = false;
        }
        else if (!LaserDoor_b->active) {
			doorswitch = true;
		}

        if (intersecting) {
            std::cout << "(Intersecting with " << obj->GetName() << " of obj ID " << obj->GetId() << " )" << std::endl;
            GameLogic::restarting = true;
            intersecting = false;
            return;
        }
        LaserDoor_b->active = static_cast<bool>(LaserDoor_a->frame_num != 2);

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
}// namespace LaserDoor_Script