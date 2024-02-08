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


Vert_Elevator::Vert_Elevator(std::string name) : LogicScript(name)
{
	std::cout << name << " Created" << std::endl;
	moving_platform_direction = false;
	moving_platform_speed = -70.0f;
	stateManager.SetState(UP);
}
/***************************************************************************/
// Start method, called when the Vert_Elevator script is first activated.
// @param obj: A pointer to the Object that this script is attached to.
// Performs initial setup and configuration for Vert_Elevator.
/***************************************************************************/
void Vert_Elevator::Start(Object* obj)
{
	std::cout << "Vert_Elevator Script Ready : " << obj->GetName() << std::endl;
	Physics* moving_platform_physics = static_cast<Physics*>(obj->GetComponent(ComponentType::Physics));
	moving_platform_physics->Velocity.y = moving_platform_speed;
	
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
	stateManager.ChangeDirection(obj);
	Physics* moving_platform_physics = static_cast<Physics*>(obj->GetComponent(ComponentType::Physics));
	moving_platform_physics->Velocity.y = stateManager.ChangeDirection(obj) ? moving_platform_speed : -moving_platform_speed;
	//std::cout << "Speed of platform : " << moving_platform_physics->Velocity.y << std::endl;
}

/***************************************************************************/
// Shutdown method, called when the Vert_Elevator script is stopped.
// @param obj: A pointer to the Object that this script is attached to.
// Contains cleanup logic for Vert_Elevator.
/***************************************************************************/
void Vert_Elevator::Shutdown(Object* obj) {
	std::cout << "Vert_Elevator Script Shutdown : " << obj->GetName() << std::endl;
}

Vert_Elevator::StateManager::StateManager() {
	currentstate = UP;
	counter = 0;
	timing = 6.0f;
}

bool Vert_Elevator::StateManager::ChangeDirection(Object* obj) {

	float dt = engine->Get_Fixed_DT();
	if(counter >= timing) {
		currentstate = currentstate ? UP : DOWN;
		counter = 0;
	}
	else {
		counter += dt;
	}

	std::cout << "Counter Check" << counter << std::endl;
	return static_cast<bool>(currentstate);
}

void Vert_Elevator::StateManager::SetState(states state) {
	currentstate = state;
}

Vert_Elevator::states Vert_Elevator::StateManager::GetState() {
	return currentstate;
}


Vert_Elevator vert_elevator("Vert_Elevator");