/* !
@file	PPlate.cpp
@author Luke Goh
@date	27/11/2023

This file contains the script for PPlate trigger to open door
*//*__________________________________________________________________________*/
#include <Debug.h>
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
	//float accumulator = 0.f;
	//float fixed_dt = 1.f / 60.f;
	//int num_of_steps = 0;

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

		//accumulator += engine->GetDt();

		
			// if pressure plate collides with player, change the animation of pressure plate
			if (plate_b->collision_flag & COLLISION_TOP) {
				//std::cout << obj->GetName() << " PP Open" << std::endl;
				plate_animation->fixed = true;
				if (plate_animation->current_type != AnimationType::Jump) {
					audio->playSfx("piston_plate_press");
					plate_animation->current_type = AnimationType::Jump;
					//std::cout << obj->GetName() << " event linked event:";
					//std::cout << plate_event->linked_event << std::endl;

					//  Change the animation of door and disable the body of door
					for (size_t j = 0; j < objectFactory->NumberOfObjects(); j++) {
						Object* obj2 = objectFactory->getObjectWithID((long)j);
						if (obj != obj2 && obj2->GetComponent(ComponentType::Event) != nullptr) {
							Event* door_event = static_cast<Event*>(obj2->GetComponent(ComponentType::Event));
							if (plate_event->linked_event == door_event->linked_event) {
								//audio->playSfx("sliding_door_open");

								Behaviour* door_behv = static_cast<Behaviour*>(obj2->GetComponent(ComponentType::Behaviour));
								int index{};
								if (door_behv != nullptr)
									index = door_behv->GetBehaviourIndex();

								if (index != 0) // Default Open, close door
								{
									Animation* door_animation = static_cast<Animation*>(obj2->GetComponent(ComponentType::Animation));
									door_animation->reverse = true;
								}
								else // Default close, open door
								{
									Animation* door_animation = static_cast<Animation*>(obj2->GetComponent(ComponentType::Animation));
									door_animation->reverse = false;
									door_animation->fixed = true;
									door_animation->current_type = AnimationType::Jump;
								}
								//Body* door_body = static_cast<Body*>(obj2->GetComponent(ComponentType::Body));
								//door_body->active = false;
							}
						}
					}
				}
				
			}
			else {
				// This is the initial state of the door
				// Change the animation of door and disable the body of door
				//std::cout << obj->GetName() << " PP Closed" << std::endl;
				plate_animation->fixed = false;
				if (plate_animation->current_type != AnimationType::Idle) {
					audio->playSfx("piston_plate_press");
					plate_animation->current_type = AnimationType::Idle;
					for (size_t j = 0; j < objectFactory->NumberOfObjects(); j++) {
						Object* obj2 = objectFactory->getObjectWithID((long)j);
						if (obj != obj2 && obj2->GetComponent(ComponentType::Event) != nullptr) {
							Event* door_event = static_cast<Event*>(obj2->GetComponent(ComponentType::Event));
							if (plate_event->linked_event == door_event->linked_event) {
								//audio->playSfx("sliding_door_open"); // Should be Closing sound

								Behaviour* door_behv = static_cast<Behaviour*>(obj2->GetComponent(ComponentType::Behaviour));
								int index{};
								if (door_behv != nullptr)
									index = door_behv->GetBehaviourIndex();

								// 0 means the door should be closed
								if (index != 0) // Door is opened by default
								{
									Animation* door_animation = static_cast<Animation*>(obj2->GetComponent(ComponentType::Animation));
									door_animation->reverse = false;
									door_animation->fixed = true;
									door_animation->current_type = AnimationType::Jump;
								}
								else // Door is closed by default
								{
									Animation* door_animation = static_cast<Animation*>(obj2->GetComponent(ComponentType::Animation));
									door_animation->reverse = true;
								}
								//door_animation->fixed = false;
								//door_animation->current_type = AnimationType::Idle;
								//Body* door_body = static_cast<Body*>(obj2->GetComponent(ComponentType::Body));
								//door_body->active = true;
							}
						}
					}
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

	PPlate pplate("PPlate");
}