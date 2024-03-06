/* !
@file	Pplate_H_Elev.cpp
@author Luke Goh
@date	27/11/2023

This file contains the script for piston to trigger the horizontal elevator
*//*__________________________________________________________________________*/
#include "Scripts/Pplate_H_Elev.h"
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
	Pplate_H_Elev::Pplate_H_Elev(std::string name) : LogicScript(name)
	{
		std::cout << name << " Created" << std::endl;
		moving_platform_direction = false;
		activated = false;
		H_Elev = nullptr;
	}
	/***************************************************************************/
	// Start method, called when the Pplate_H_Elev script is first activated.
	// @param obj: A pointer to the Object that this script is attached to.
	// Performs initial setup and configuration for Pplate_H_Elev.
	/***************************************************************************/
	void Pplate_H_Elev::Start(Object* obj) {
		std::cout << "Pplate_H_Elev Script Ready : " << obj->GetName() << std::endl;
		count = 0.f;
		deltaT = engine->GetDt();
		activated = true;
	}

	/***************************************************************************/
	// Update method, called on every frame to update Pplate_H_Elev's state.
	// @param obj: A pointer to the Object that this script is attached to.
	// Contains logic for Pplate_H_Elev's interactions with H_Elev, and animations.
	/***************************************************************************/
	void Pplate_H_Elev::Update(Object* obj) {

		// For some reason, the player is not changing position
		if (obj == nullptr) {
			//std::cout << "NIL OBJ : Pplate_H_Elev" << std::endl;
			return;
		}
		Rectangular* piston_b = static_cast<Rectangular*>(obj->GetComponent(ComponentType::Body));
		//Transform* player_t = static_cast<Transform*>(playerObj->GetComponent(ComponentType::Transform));
		if (piston_b == nullptr) {
			//std::cout << "NIL BODY : Pplate_H_Elev" << std::endl;
			return;
		};

		Event* piston_event = static_cast<Event*>(obj->GetComponent(ComponentType::Event));

		// if piston collides with player, change the animation of piston
		if (piston_b->collision_flag & COLLISION_TOP) {
			Animation* piston_animation = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));
			if (piston_animation == nullptr) {
				//std::cout << "NIL ANIMATION : Pplate_H_Elev" << std::endl;
				return;
			}
			piston_animation->fixed = true;
			if (piston_animation->current_type != AnimationType::Jump) {
				audio->playSfx("piston_plate_press");
				piston_animation->current_type = AnimationType::Jump;
				activated = false;
			}

		}
		else {
		Animation* piston_animation = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));
		if (piston_animation == nullptr) {
			//std::cout << "NIL ANIMATION : Pplate_H_Elev" << std::endl;
			return;
		}
			piston_animation->fixed = false;
			if (piston_animation->current_type != AnimationType::Idle)
			{
				audio->playSfx("piston_plate_press");
				piston_animation->current_type = AnimationType::Idle;

				activated = true;
			}
		}

		for (size_t j = 0; j < objectFactory->NumberOfObjects(); j++) {
			Object* obj2 = objectFactory->getObjectWithID((long)j);
			if (obj2->GetComponent(ComponentType::Event) != nullptr) {
				Event* H_Elev_event = static_cast<Event*>(obj2->GetComponent(ComponentType::Event));
				if (piston_event->linked_event == H_Elev_event->linked_event) {
					H_Elev = obj2;

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
						float moving_platform_speed{};
						Behaviour* moving_platform_b = static_cast<Behaviour*>(H_Elev->GetComponent(ComponentType::Behaviour));

						// Moving right
						if (moving_platform_t->Position.x - moving_platform_b->GetBehaviourCounter() >= moving_platform_b->GetBehaviourDistance() && moving_platform_direction == false)
						{
							moving_platform_direction = true;
							moving_platform_b->SetBehaviourCounter(moving_platform_t->Position.x);
							std::cout << "change dir to left\n";
						}
						// Moving left
						else if (moving_platform_b->GetBehaviourCounter() - moving_platform_t->Position.x >= moving_platform_b->GetBehaviourDistance() && moving_platform_direction == true)
						{
							moving_platform_direction = false;
							moving_platform_b->SetBehaviourCounter(moving_platform_t->Position.x);
							std::cout << "change dir to right\n";
						}
						// First move
						else if (moving_platform_b->GetBehaviourCounter() == 0)
						{
							moving_platform_b->SetBehaviourCounter(moving_platform_t->Position.x);
							std::cout << "ONLY SEE THIS ONCE PER OBJECT OF THIS SCRIPT\n";
						}

						moving_platform_speed = moving_platform_direction ? -moving_platform_b->GetBehaviourSpeed() : moving_platform_b->GetBehaviourSpeed();
						moving_platform_physics->Velocity.x = moving_platform_speed;
					}
					else
					{
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
					}

				}
			}
		}

		//// if the piston is not activated, move the horizontal elevator
		//if (activated) {

		//	for (size_t j = 0; j < objectFactory->NumberOfObjects(); j++) {
		//		Object* obj2 = objectFactory->getObjectWithID((long)j);
		//		if (obj2->GetComponent(ComponentType::Event) != nullptr) {
		//			Event* H_Elev_event = static_cast<Event*>(obj2->GetComponent(ComponentType::Event));
		//			if (piston_event->linked_event == H_Elev_event->linked_event) {
		//				H_Elev = obj2;
		//			}
		//		}
		//	}

		//	if (H_Elev == nullptr) {
		//		return;
		//	}

		//	Physics* moving_platform_physics = static_cast<Physics*>(H_Elev->GetComponent(ComponentType::Physics));
		//	Transform* moving_platform_t = static_cast<Transform*>(H_Elev->GetComponent(ComponentType::Transform));
		//	if (moving_platform_physics == nullptr || moving_platform_t == nullptr) {
		//		//std::cout << "NIL PHYSICS : V_Elevator" << std::endl;
		//		return;
		//	};
		//	moving_platform_physics->Velocity.x = 0.0f;
		//	float moving_platform_speed{};

		//	// if the count >= 5, change direction
		//	if (count >= 420.f) {
		//		std::cout << "change dir\n";
		//		moving_platform_direction = !moving_platform_direction;
		//		count = 0;
		//	}
		//	else {
		//		//count += deltaT;
		//		if (moving_platform_speed < 0)
		//			count += -moving_platform_speed;
		//		else
		//			count += moving_platform_speed;
		//	}
		//	
		//	Behaviour* moving_platform_b = static_cast<Behaviour*>(H_Elev->GetComponent(ComponentType::Behaviour));

		//	moving_platform_speed = moving_platform_direction ? -moving_platform_b->GetBehaviourIndex() : moving_platform_b->GetBehaviourIndex();
		//	moving_platform_physics->Velocity.x = moving_platform_speed;
		//}
		//else
		//{
		//	for (size_t j = 0; j < objectFactory->NumberOfObjects(); j++) {
		//		Object* obj2 = objectFactory->getObjectWithID((long)j);
		//		if (obj2->GetComponent(ComponentType::Event) != nullptr) {
		//			Event* H_Elev_event = static_cast<Event*>(obj2->GetComponent(ComponentType::Event));
		//			if (piston_event->linked_event == H_Elev_event->linked_event) {
		//				H_Elev = obj2;
		//			}
		//		}
		//	}

		//	if (H_Elev == nullptr) {
		//		return;
		//	}

		//	Physics* moving_platform_physics = static_cast<Physics*>(H_Elev->GetComponent(ComponentType::Physics));
		//	Transform* moving_platform_t = static_cast<Transform*>(H_Elev->GetComponent(ComponentType::Transform));
		//	if (moving_platform_physics == nullptr || moving_platform_t == nullptr) {
		//		//std::cout << "NIL PHYSICS : V_Elevator" << std::endl;
		//		return;
		//	};
		//	moving_platform_physics->Velocity.x = 0.0f;
		//}
	}

	/***************************************************************************/
	// Shutdown method, called when the Pplate_H_Elev script is stopped.
	// @param obj: A pointer to the Object that this script is attached to.
	/***************************************************************************/

	void Pplate_H_Elev::Shutdown(Object* obj) {
		std::cout << "Pplate_H_Elev Script Shutdown : " << obj->GetName() << std::endl;
	}

	Pplate_H_Elev pplate_H_Elev("Pplate_H_Elev"); 
}