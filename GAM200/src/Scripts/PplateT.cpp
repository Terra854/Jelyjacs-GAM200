/* !
@file	PPlateT.cpp
@author Luke Goh
@date	27/11/2023

This file contains the script for PPlateT trigger to open door
*//*__________________________________________________________________________*/
#include <Debug.h>
#include "Scripts/PplateT.h"
#include <Factory.h>
#include <Object.h>
#include <GameLogic.h>
#include <components/Body.h>
#include <components/Transform.h>
#include <components/Animation.h>
#include <components/Event.h>
#include <iostream>
#include <Audio.h>

namespace PplateT {
	//float accumulator = 0.f;
	//float fixed_dt = 1.f / 60.f;
	//int num_of_steps = 0;

	PPlateT::PPlateT(std::string name) : LogicScript(name)
	{
		std::cout << name << " Created" << std::endl;
	}
	/***************************************************************************/
	// Start method, called when the PPlateT script is first activated.
	// @param obj: A pointer to the Object that this script is attached to.
	// Performs initial setup and configuration for PPlateT.
	/***************************************************************************/
	void PPlateT::Start(Object* obj) {

		std::cout << "PPlateT Script Ready : " << obj->GetName() << std::endl;
	}

	/***************************************************************************/
	// Update method, called on every frame to update PPlateT's state.
	// @param obj: A pointer to the Object that this script is attached to.
	// Contains logic for PPlateT's interactions with door, and animations.
	/***************************************************************************/
	void PPlateT::Update(Object* obj) {
		// For some reason, the player is not changing position
		if (obj == nullptr) {
			//std::cout << "NIL OBJ : PPlateT" << std::endl;
			return;
		}
		Rectangular* plate_b = static_cast<Rectangular*>(obj->GetComponent(ComponentType::Body));
		Animation* plate_animation = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));
		Event* plate_event = static_cast<Event*>(obj->GetComponent(ComponentType::Event));

		if (plate_animation == nullptr) {
			//std::cout << "NIL ANIMATION : PPlateT" << std::endl;
			return;
		}

		//Transform* player_t = static_cast<Transform*>(playerObj->GetComponent(ComponentType::Transform));
		if (plate_b == nullptr) {
			//std::cout << "NIL BODY : PPlateT" << std::endl;
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
							if (door_behv != nullptr)
								door_behv->SetBehaviourIndex(-1);
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
							if (door_behv != nullptr)
								door_behv->SetBehaviourIndex(0);
						}
					}
				}
			}

		}
	}

	/***************************************************************************/
	// Shutdown method, called when the PPlateT script is stopped.
	// @param obj: A pointer to the Object that this script is attached to.
	/***************************************************************************/

	void PPlateT::Shutdown(Object* obj) {
		std::cout << "PPlateT Script Shutdown : " << obj->GetName() << std::endl;
	}

	PPlateT pplate("PPlateT");
}