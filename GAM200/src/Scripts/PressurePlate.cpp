/* !
@file	PPlate.cpp
@author Luke Goh
@date	27/11/2023

This file contains the script for PPlate trigger to open door
*//*__________________________________________________________________________*/
#include "Scripts/PressurePlate.h"
#include <Factory.h>
#include <Object.h>
#include <GameLogic.h>
#include <components/Body.h>
#include <components/Transform.h>
#include <components/Animation.h>
#include <components/Event.h>
#include <iostream>
#include <Audio.h>

namespace PressurePlate {
	float accumulator = 0.f;
	float fixed_dt = 1.f / 60.f;
	int num_of_steps = 0;

	PPlate::PPlate(std::string name) : LogicScript(name)
	{
		std::cout << name << " Created" << std::endl;
	}
	/***************************************************************************/
	// Start method, called when the PPlate script is first activated.
	// @param obj: A pointer to the Object that this script is attached to.
	// Performs initial setup and configuration for PPlate.
	/***************************************************************************/
	void PPlate::Start(Object* obj) {
		std::cout << "PPlate Script Ready : " << obj->GetName() << std::endl;
	}

	/***************************************************************************/
	// Update method, called on every frame to update PPlate's state.
	// @param obj: A pointer to the Object that this script is attached to.
	// Contains logic for PPlate's interactions with door, and animations.
	/***************************************************************************/
	void PPlate::Update(Object* obj) {
		// For some reason, the player is not changing position
		if (obj == nullptr) {
			//std::cout << "NIL OBJ : PPlate" << std::endl;
			return;
		}
		Rectangular* plate_b = static_cast<Rectangular*>(obj->GetComponent(ComponentType::Body));
		Animation* plate_animation = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));
		Event* plate_event = static_cast<Event*>(obj->GetComponent(ComponentType::Event));

		if (plate_animation == nullptr) {
			//std::cout << "NIL ANIMATION : PPlate" << std::endl;
			return;
		}

		//Transform* player_t = static_cast<Transform*>(playerObj->GetComponent(ComponentType::Transform));
		if (plate_b == nullptr) {
			//std::cout << "NIL BODY : PPlate" << std::endl;
			return;
		};

		accumulator += engine->GetDt();

		// Only run the Pressure Plate script if fixed_dt has passed 
		if (accumulator < fixed_dt) {
			return;
		}

		// Check and see how many loops the script needs to update
		while (accumulator > fixed_dt) {
			num_of_steps++;
			accumulator -= fixed_dt;
		}

		for (; num_of_steps; num_of_steps--) {
			// if piston collides with player, change the animation of piston
			if (plate_b->collision_flag & COLLISION_TOP) {
				std::cout << "PP Open" << std::endl;
				plate_animation->fixed = true;
				if (plate_animation->current_type == AnimationType::Jump) {}
				else {
					plate_animation->current_type = AnimationType::Jump;
					std::cout << "Pressure Plate event linked event:";
					std::cout << plate_event->linked_event << std::endl;

					//  Change the animation of door and disable the body of door
					for (size_t j = 0; j < objectFactory->NumberOfObjects(); j++) {
						Object* obj2 = objectFactory->getObjectWithID((long)j);
						if (obj2->GetComponent(ComponentType::Event) != nullptr) {
							Event* door_event = static_cast<Event*>(obj2->GetComponent(ComponentType::Event));
							if (plate_event->linked_event == door_event->linked_event) {
								audio->playSfx(AudioType::Sliding_Door_Open);
								Animation* door_animation = static_cast<Animation*>(obj2->GetComponent(ComponentType::Animation));
								door_animation->fixed = true;
								door_animation->current_type = AnimationType::Jump;
								Body* door_body = static_cast<Body*>(obj2->GetComponent(ComponentType::Body));
								door_body->active = false;
							}
						}
					}
				}
			}
			else if(!(plate_b->collision_flag & COLLISION_TOP) && plate_animation->fixed) {
				//std::cout << "PP Closed" << std::endl;
				/*plate_animation->fixed = false;
				if (plate_animation->current_type == AnimationType::Idle) {}
				else {
					plate_animation->current_type = AnimationType::Idle;
					for (size_t j = 0; j < objectFactory->NumberOfObjects(); j++) {
						Object* obj2 = objectFactory->getObjectWithID((long)j);
						if (obj2->GetComponent(ComponentType::Event) != nullptr) {
							Event* door_event = static_cast<Event*>(obj2->GetComponent(ComponentType::Event));
							if (plate_event->linked_event == door_event->linked_event) {
								audio->playSfx(AudioType::Sliding_Door_Open); // Should be Closing sound
								Animation* door_animation = static_cast<Animation*>(obj2->GetComponent(ComponentType::Animation));
								door_animation->fixed = false;
								door_animation->current_type = AnimationType::Idle;
								Body* door_body = static_cast<Body*>(obj2->GetComponent(ComponentType::Body));
								door_body->active = true;
							}
						}
					}
				}*/

			}
		}
	}

	/***************************************************************************/
	// Shutdown method, called when the PPlate script is stopped.
	// @param obj: A pointer to the Object that this script is attached to.
	/***************************************************************************/

	void PPlate::Shutdown(Object* obj) {
		std::cout << "PPlate Script Shutdown : " << obj->GetName() << std::endl;
	}

	PPlate piston("PPlate");
}