/* !
@file	Piston_H_Elev.cpp
@author Luke Goh
@date	27/11/2023

This file contains the script for piston to trigger the horizontal elevator
*//*__________________________________________________________________________*/
#include "Scripts/Piston_H_Elev.h"
#include <Factory.h>
#include <Object.h>
#include <GameLogic.h>
#include <components/Body.h>
#include <components/Transform.h>
#include <components/Animation.h>
#include <components/Event.h>
#include <iostream>
#include <Audio.h>


namespace PHE_Script
{
	float count, deltaT;
	Piston_H_Elev::Piston_H_Elev(std::string name) : LogicScript(name)
	{
		std::cout << name << " Created" << std::endl;
		moving_platform_direction = false;
		activated = false;
		H_Elev = nullptr;
	}
	/***************************************************************************/
	// Start method, called when the Piston_H_Elev script is first activated.
	// @param obj: A pointer to the Object that this script is attached to.
	// Performs initial setup and configuration for Piston_H_Elev.
	/***************************************************************************/
	void Piston_H_Elev::Start(Object* obj) {
		std::cout << "Piston_H_Elev Script Ready : " << obj->GetName() << std::endl;
		count = 0.f;
		deltaT = engine->GetDt();
		activated = static_cast<Behaviour*>(obj->GetComponent(ComponentType::Behaviour))->GetBehaviourIndex();
	}

	/***************************************************************************/
	// Update method, called on every frame to update Piston_H_Elev's state.
	// @param obj: A pointer to the Object that this script is attached to.
	// Contains logic for Piston_H_Elev's interactions with H_Elev, and animations.
	/***************************************************************************/
	void Piston_H_Elev::Update(Object* obj) {

		// For some reason, the player is not changing position
		if (obj == nullptr) {
			//std::cout << "NIL OBJ : Piston_H_Elev" << std::endl;
			return;
		}
		Rectangular* piston_b = static_cast<Rectangular*>(obj->GetComponent(ComponentType::Body));
		//Transform* player_t = static_cast<Transform*>(playerObj->GetComponent(ComponentType::Transform));
		if (piston_b == nullptr) {
			//std::cout << "NIL BODY : Piston_H_Elev" << std::endl;
			return;
		};

		// if piston collides with player, change the animation of piston
		if (piston_b->collision_flag & COLLISION_TOP) {
			Animation* piston_animation = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));
			if (piston_animation == nullptr) {
				//std::cout << "NIL ANIMATION : Piston_H_Elev" << std::endl;
				return;
			}
			piston_animation->fixed = true;
			if (piston_animation->current_type == AnimationType::Jump) {}
			else {
				piston_animation->current_type = AnimationType::Jump;
				Event* piston_event = static_cast<Event*>(obj->GetComponent(ComponentType::Event));
				std::cout << "piston event linked event:";
				std::cout << piston_event->linked_event << std::endl;
				
				//  Change the animation and disable the body of the horizontal elevator
				for (size_t j = 0; j < objectFactory->NumberOfObjects(); j++) {
					Object* obj2 = objectFactory->getObjectWithID((long)j);
					if (obj2->GetComponent(ComponentType::Event) != nullptr) {
						Event* H_Elev_event = static_cast<Event*>(obj2->GetComponent(ComponentType::Event));
						if (piston_event->linked_event == H_Elev_event->linked_event) {
							H_Elev = obj2;
							activated = !activated;
						}
					}
				}
			}
		}

		// if the piston is activated, move the horizontal elevator
		if (activated) {
			if (H_Elev == nullptr) {
				return;
			}
			Physics* moving_platform_physics = static_cast<Physics*>(H_Elev->GetComponent(ComponentType::Physics));
			Transform* moving_platform_t = static_cast<Transform*>(H_Elev->GetComponent(ComponentType::Transform));
			if (moving_platform_physics == nullptr || moving_platform_t == nullptr) {
				//std::cout << "NIL PHYSICS : V_Elevator" << std::endl;
				return;
			};
			moving_platform_physics->Velocity.x = 0.0f;
			float moving_platform_speed;

			// if the count >= 5, change direction
			if (count >= 5.f) {
				std::cout << "change dir\n";
				moving_platform_direction = !moving_platform_direction;
				count = 0;
			}
			else {
				count += deltaT;
			}
			moving_platform_speed = moving_platform_direction ? -70.0f : 70.0f;
			moving_platform_physics->Velocity.x = moving_platform_speed;
		}
	}

	/***************************************************************************/
	// Shutdown method, called when the Piston_H_Elev script is stopped.
	// @param obj: A pointer to the Object that this script is attached to.
	/***************************************************************************/

	void Piston_H_Elev::Shutdown(Object* obj) {
		std::cout << "Piston_H_Elev Script Shutdown : " << obj->GetName() << std::endl;
	}

	Piston_H_Elev piston_H_Elev("Piston_H_Elev"); }