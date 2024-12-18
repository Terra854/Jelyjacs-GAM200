/* !
@file	Piston.cpp
@author Luke Goh
@date	27/11/2023

This file contains the script for Piston trigger to open door
*//*__________________________________________________________________________*/
#include "Scripts/Piston.h"
#include <Factory.h>
#include <Object.h>
#include <GameLogic.h>
#include <components/Body.h>
#include <components/Transform.h>
#include <components/Animation.h>
#include <components/Event.h>
#include <iostream>
#include <Audio.h>

namespace Piston_Script {
	Piston::Piston(std::string name) : LogicScript(name)
	{
		std::cout << name << " Created" << std::endl;
	}
	/***************************************************************************/
	// Start method, called when the Piston script is first activated.
	// @param obj: A pointer to the Object that this script is attached to.
	// Performs initial setup and configuration for Piston.
	/***************************************************************************/
	void Piston::Start(Object* obj) {
		std::cout << "Piston Script Ready : " << obj->GetName() << std::endl;
	}

	/***************************************************************************/
	// Update method, called on every frame to update Piston's state.
	// @param obj: A pointer to the Object that this script is attached to.
	// Contains logic for Piston's interactions with door, and animations.
	/***************************************************************************/
	void Piston::Update(Object* obj) {

		// For some reason, the player is not changing position
		if (obj == nullptr) {
			//std::cout << "NIL OBJ : Piston" << std::endl;
			return;
		}
		Rectangular* piston_b = static_cast<Rectangular*>(obj->GetComponent(ComponentType::Body));
		//Transform* player_t = static_cast<Transform*>(playerObj->GetComponent(ComponentType::Transform));
		if (piston_b == nullptr) {
			//std::cout << "NIL BODY : Piston" << std::endl;
			return;
		};

		// if piston collides with player, change the animation of piston
		if (piston_b->collision_flag & COLLISION_TOP) {
			Animation* piston_animation = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));
			if (piston_animation == nullptr) {
				//std::cout << "NIL ANIMATION : Piston" << std::endl;
				return;
			}
			piston_animation->fixed = true;
			if (piston_animation->current_type == AnimationType::Jump) {}
			else {
				piston_animation->current_type = AnimationType::Jump;
				audio->playSfx("piston_plate_press");
				Event* piston_event = static_cast<Event*>(obj->GetComponent(ComponentType::Event));
				std::cout << "piston event linked event:";
				std::cout << piston_event->linked_event << std::endl;

				//  Change the animation of door and disable the body of door
				for (size_t j = 0; j < objectFactory->NumberOfObjects(); j++) {
					Object* obj2 = objectFactory->getObjectWithID((long)j);
					if (obj2->GetComponent(ComponentType::Event) != nullptr) {
						Event* door_event = static_cast<Event*>(obj2->GetComponent(ComponentType::Event));
						if (piston_event->linked_event == door_event->linked_event) {
							Animation* door_animation = static_cast<Animation*>(obj2->GetComponent(ComponentType::Animation));
							//door_animation->reverse = false;
							static_cast<Behaviour*>(obj2->GetComponent(ComponentType::Behaviour))->SetBehaviourIndex(-1);
							door_animation->fixed = true;
							door_animation->current_type = AnimationType::Jump;
						}
					}
				}
			}
		}
	}

	/***************************************************************************/
	// Shutdown method, called when the Piston script is stopped.
	// @param obj: A pointer to the Object that this script is attached to.
	/***************************************************************************/

	void Piston::Shutdown(Object* obj) {
		std::cout << "Piston Script Shutdown : " << obj->GetName() << std::endl;
	}

	Piston piston("Piston");
}