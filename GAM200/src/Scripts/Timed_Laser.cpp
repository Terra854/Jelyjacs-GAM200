/* !
@file	Timed_Laser.cpp
@author Luke Goh
@date	5/2/2024

This file contains the script for the Laser
*//*__________________________________________________________________________*/
#include "Scripts/Timed_Laser.h"
#include <PhysicsSystem.h>
#include <components/Physics.h>
#include <components/Transform.h>
#include <components/Body.h>
#include <Factory.h>
#include <Core_Engine.h>
#include <components/Animation.h>

namespace L_Script {
	float count, deltaT;
	bool active;
	Laser::Laser(std::string name) : LogicScript(name)
	{
		std::cout << name << " Created" << std::endl;
		active = false;
	}
	/***************************************************************************/
	// Start method, called when the Laser script is first activated.
	// @param obj: A pointer to the Object that this script is attached to.
	// Performs initial setup and configuration for Laser.
	/***************************************************************************/
	void Laser::Start(Object* obj)
	{
		std::cout << "Laser Script Ready : " << obj->GetName() << std::endl;
		count = 0.f;
		deltaT = engine->GetDt();
		active = true;

	}

	/***************************************************************************/
	// Update method, called on every frame to update Laser's state.
	// @param obj: A pointer to the Object that this script is attached to.
	// Contains logic for Laser's movement and animations.
	/***************************************************************************/
	void Laser::Update(Object* obj) {
		// For some reason, the player is not changing position
		if (obj == nullptr) {
			return;
		}
		//audio->playSfx(AudioType::Sliding_Door_Open);
		Animation* door_animation = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));

		Body* door_body = static_cast<Body*>(obj->GetComponent(ComponentType::Body));
		Behaviour* door_behaviour = static_cast<Behaviour*>(obj->GetComponent(ComponentType::Behaviour));
		if (door_animation == nullptr || door_body == nullptr || door_behaviour == nullptr) {
			//std::cout << "NIL PHYSICS : V_Elevator" << std::endl;
			return;
		}
		
		// if the count >= 5, change direction
		if (active) {
			if (door_behaviour->GetBehaviourIndex() == -1) {
				active = false;
			}
			if (door_behaviour->GetBehaviourSpeed() >= 3.f) {
				std::cout << obj->GetId() <<": change dir\n";
				door_animation->fixed = !door_animation->fixed;
				if (door_animation->current_type == AnimationType::Jump) { door_animation->current_type = AnimationType::Idle; }
				else { door_animation->current_type = AnimationType::Jump; }
				door_body->active = !door_body->active;
				door_behaviour->SetBehaviourSpeed(0);
			}
			else {
				door_behaviour->SetBehaviourSpeed(door_behaviour->GetBehaviourSpeed() + deltaT);
			}
		}
		else {
			if (door_behaviour->GetBehaviourIndex() == 0) {
				active = true;
			}
			//keep the door open
			door_animation->current_type = AnimationType::Jump;
			door_animation->fixed = true;
			door_body->active = false;
			std::cout << obj->GetId() << "Laser is off\n";
		}

			
			
	}

	/***************************************************************************/
	// Shutdown method, called when the Laser script is stopped.
	// @param obj: A pointer to the Object that this script is attached to.
	// Contains cleanup logic for Laser.
	/***************************************************************************/
	void Laser::Shutdown(Object* obj) {
		std::cout << "Laser Script Shutdown : " << obj->GetName() << std::endl;
	}

	Laser laser("Timed Laser");
}