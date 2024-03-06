/* !
@file	Hori_Elevator.cpp
@author Luke Goh
@date	5/2/2024

This file contains the script for the Horizontal elevator
*//*__________________________________________________________________________*/
#include "Scripts/Hori_Elevator.h"
#include <PhysicsSystem.h>
#include <components/Physics.h>
#include <components/Transform.h>
#include <components/Body.h>
#include <Factory.h>
#include <Core_Engine.h>

namespace HE_Script {
	float count, deltaT;
	bool active;
	Hori_Elevator::Hori_Elevator(std::string name) : LogicScript(name)
	{
		std::cout << name << " Created" << std::endl;
		moving_platform_direction = false;
		active = true;
	}
	/***************************************************************************/
	// Start method, called when the Hori_Elevator script is first activated.
	// @param obj: A pointer to the Object that this script is attached to.
	// Performs initial setup and configuration for Hori_Elevator.
	/***************************************************************************/
	void Hori_Elevator::Start(Object* obj)
	{
		std::cout << "Hori_Elevator Script Ready : " << obj->GetName() << std::endl;
		count = 0.f;
		deltaT = engine->GetDt();
		active = true;

	}

	/***************************************************************************/
	// Update method, called on every frame to update Hori_Elevator's state.
	// @param obj: A pointer to the Object that this script is attached to.
	// Contains logic for Hori_Elevator's movement and animations.
	/***************************************************************************/
	void Hori_Elevator::Update(Object* obj) {
		// For some reason, the player is not changing position
		if (obj == nullptr) {
			return;
		}
		// Ignore object if index is not 0
		if (static_cast<Behaviour*>(obj->GetComponent(ComponentType::Behaviour))->GetBehaviourIndex() != 0)
			return;

		//std::cout << obj->GetName() << std::endl;
		Physics* moving_platform_physics = static_cast<Physics*>(obj->GetComponent(ComponentType::Physics));
		Transform* moving_platform_t = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));
		if (moving_platform_physics == nullptr || moving_platform_t == nullptr) {
			//std::cout << "NIL PHYSICS : V_Elevator" << std::endl;
			return;
		};
		moving_platform_physics->Velocity.x = 0.0f;
		float moving_platform_speed;
		if (active) {
			if (static_cast<Behaviour*>(obj->GetComponent(ComponentType::Behaviour))->GetBehaviourIndex() == -1) {
				active = false;
				return;
			}
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
		else {
			if (static_cast<Behaviour*>(obj->GetComponent(ComponentType::Behaviour))->GetBehaviourIndex() == 0) {
				active = true;
			}
		}
	}

	/***************************************************************************/
	// Shutdown method, called when the Hori_Elevator script is stopped.
	// @param obj: A pointer to the Object that this script is attached to.
	// Contains cleanup logic for Hori_Elevator.
	/***************************************************************************/
	void Hori_Elevator::Shutdown(Object* obj) {
		std::cout << "Hori_Elevator Script Shutdown : " << obj->GetName() << std::endl;
	}

	Hori_Elevator hori_elevator("Hori_Elevator");
}