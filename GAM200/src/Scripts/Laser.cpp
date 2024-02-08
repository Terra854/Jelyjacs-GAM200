/* !
@file	Laser.cpp
@author Luke Goh
@date	5/2/2024

This file contains the script for the Laser
*//*__________________________________________________________________________*/
#include "Scripts/Laser.h"
#include <PhysicsSystem.h>
#include <components/Physics.h>
#include <components/Transform.h>
#include <components/Body.h>
#include <Factory.h>
#include <Core_Engine.h>
#include <components/Animation.h>

namespace L_Script {
	float count, deltaT;
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
		if (door_animation == nullptr || door_body == nullptr) {
			//std::cout << "NIL PHYSICS : V_Elevator" << std::endl;
			return;
		}
		// if the count >= 5, change direction
		if (count >= 5.f) {
			std::cout << "change dir\n";
			door_animation->fixed = !door_animation->fixed;
			if (door_animation->current_type == AnimationType::Jump) { door_animation->current_type = AnimationType::Idle; }
			else { door_animation->current_type = AnimationType::Jump;}
			door_body->active = !door_body->active;
			count = 0;
		}
		else {
			count += deltaT;
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

	Laser laser("Laser");
}