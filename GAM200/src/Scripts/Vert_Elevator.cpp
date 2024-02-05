/* !
@file	Vert_Elevator.cpp
@author Luke Goh
@date	27/11/2023

This file contains the script for the vertical elevator
*//*__________________________________________________________________________*/
#include "Scripts/Vert_Elevator.h"
#include <PhysicsSystem.h>
#include <components/Physics.h>
#include <components/Transform.h>
#include <components/Body.h>
#include <Factory.h>
#include <Core_Engine.h>

float counter, dt;
Vert_Elevator::Vert_Elevator(std::string name) : LogicScript(name)
{
	std::cout << name << " Created" << std::endl;
	moving_platform_direction = false;
}
/***************************************************************************/
// Start method, called when the Vert_Elevator script is first activated.
// @param obj: A pointer to the Object that this script is attached to.
// Performs initial setup and configuration for Vert_Elevator.
/***************************************************************************/
void Vert_Elevator::Start(Object* obj)
{
	std::cout << "Vert_Elevator Script Ready : " << obj->GetName() << std::endl;
	counter = 0.f;
	dt = engine->GetDt();
	
}

/***************************************************************************/
// Update method, called on every frame to update Vert_Elevator's state.
// @param obj: A pointer to the Object that this script is attached to.
// Contains logic for Vert_Elevator's movement and animations.
/***************************************************************************/
void Vert_Elevator::Update(Object* obj) {
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
	if (counter >= 5.f) { // 160 is the max height of the platform
		std::cout << "change dir\n";
		moving_platform_direction = !moving_platform_direction;
		counter = 0;
	}
	else {
		counter += dt;
	}
	moving_platform_speed = moving_platform_direction ? -70.0f : 70.0f;
	moving_platform_physics->Velocity.y = moving_platform_speed;
}

/***************************************************************************/
// Shutdown method, called when the Vert_Elevator script is stopped.
// @param obj: A pointer to the Object that this script is attached to.
// Contains cleanup logic for Vert_Elevator.
/***************************************************************************/
void Vert_Elevator::Shutdown(Object* obj) {
	std::cout << "Vert_Elevator Script Shutdown : " << obj->GetName() << std::endl;
}

Vert_Elevator vert_elevator("Vert_Elevator");