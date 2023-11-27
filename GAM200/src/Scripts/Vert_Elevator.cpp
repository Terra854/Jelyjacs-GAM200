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

Vert_Elevator::Vert_Elevator(std::string name) : LogicScript(name)
{
	std::cout << name << " Created" << std::endl;
	moving_platform_direction = false;
}

void Vert_Elevator::Start(Object* obj)
{
	std::cout << "Vert_Elevator Script Ready : " << obj->GetName() << std::endl;
	
}

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
	if (moving_platform_t->Position.y >= 160.0f) { // 160 is the max height of the platform
		moving_platform_direction = true;
	}
	if (moving_platform_t->Position.y <= -160.0f) { // -160 is the min height of the platform
		moving_platform_direction = false;
	}
	moving_platform_speed = moving_platform_direction ? -70.0f : 70.0f;
	moving_platform_physics->Velocity.y = moving_platform_speed;
}

void Vert_Elevator::Shutdown(Object* obj) {
	std::cout << "Vert_Elevator Script Shutdown : " << obj->GetName() << std::endl;
}

Vert_Elevator vert_elevator("Vert_Elevator");