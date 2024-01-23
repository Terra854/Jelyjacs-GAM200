/* !
@file	Hori_Elevator.cpp
@author Luke Goh
@date	27/11/2023

This file contains the script for the horizontal elevator
*//*__________________________________________________________________________*/
#include "Scripts/Hori_Elevator.h"
#include <PhysicsSystem.h>
#include <components/Physics.h>
#include <components/Transform.h>
#include <components/Body.h>
#include <Factory.h>

Hori_Elevator::Hori_Elevator(std::string name) : LogicScript(name)
{
	std::cout << name << " Created" << std::endl;
	moving_platform_direction = false;
}
/***************************************************************************/
// Start method, called when the Hori_Elevator script is first activated.
// @param obj: A pointer to the Object that this script is attached to.
// Performs initial setup and configuration for Hori_Elevator.
/***************************************************************************/
void Hori_Elevator::Start(Object* obj)
{
	std::cout << "Hori_Elevator Script Ready : " << obj->GetName() << std::endl;

}

/***************************************************************************/
// Update method, called on every frame to update Hori_Elevator's state.
// @param obj: A pointer to the Object that this script is attached to.
// Contains logic for Hori_Elevator's movement and animations.
/***************************************************************************/
void Hori_Elevator::Update(Object* obj) {
	// For some reason, the player is not changing position
	if (obj == nullptr) {
		//std::cout << "NIL OBJ : V_Elevator" << std::endl;
		return;
	}
	//std::cout << obj->GetName() << std::endl;
	Physics* moving_platform_physics = static_cast<Physics*>(obj->GetComponent(ComponentType::Physics));
	Transform* moving_platform_t = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));
	if (moving_platform_physics == nullptr || moving_platform_t == nullptr) {
		//std::cout << "NIL PHYSICS : V_Elevator" << std::endl;
		return;
	};
	moving_platform_physics->Velocity.x = 0.0f;
	float moving_platform_speed;

	// if the platform reach the max/min height, change direction
	if (moving_platform_t->Position.y >= 160.0f) { // 160 is the max height of the platform
		moving_platform_direction = true;
	}
	if (moving_platform_t->Position.y <= -160.0f) { // -160 is the min height of the platform
		moving_platform_direction = false;
	}
	moving_platform_speed = moving_platform_direction ? -70.0f : 70.0f;
	moving_platform_physics->Velocity.y = moving_platform_speed;
}

/***************************************************************************/
// Shutdown method, called when the Hori_Elevator script is stopped.
// @param obj: A pointer to the Object that this script is attached to.
// Contains cleanup logic for Hori_Elevator.
/***************************************************************************/
void Hori_Elevator::Shutdown(Object* obj) {
	std::cout << "Hori_Elevator Script Shutdown : " << obj->GetName() << std::endl;
}

Hori_Elevator vert_elevator("Hori_Elevator");