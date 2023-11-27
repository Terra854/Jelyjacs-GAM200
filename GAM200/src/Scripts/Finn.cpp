/* !
@file	Finn.cpp
@author Luke Goh
@date	27/11/2023

This file contains the script for Finn, the player character (Human)
*//*__________________________________________________________________________*/
#include "Scripts/Finn.h"
#include <iostream>
#include <components/Transform.h>
#include <components/Animation.h>
#include <components/Physics.h>
#include <components/Body.h>
#include <Audio.h>

float finn_move_time = 0.f;

Finn::Finn(std::string name) : LogicScript(name)
{
	std::cout << name << " Created" << std::endl;
}
/***************************************************************************/
// Start method, called when the Finn script is first activated.
// @param obj: A pointer to the Object that this script is attached to.
// Performs initial setup and configuration for Finn.
/***************************************************************************/
void Finn::Start(Object* obj) {
	std::cout << "Finn Script Ready : " << obj->GetName() << std::endl;
}

/***************************************************************************/
// Update method, called on every frame to update Finn's state.
// @param obj: A pointer to the Object that this script is attached to.
// Contains logic for Finn's movement and animations.
/***************************************************************************/
void Finn::Update(Object* obj) {
	if (obj == nullptr) {
		return;
	}
	Physics* player_physics = static_cast<Physics*>(obj->GetComponent(ComponentType::Physics));
	Animation* player_animation = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));
	if (GameLogic::playerObj->GetName() == "Finn")
	{
		if (player_physics == nullptr || player_animation == nullptr) {
			return;
		};
		player_physics->Velocity.x = 0.0f;

		if (player_animation->face_right) {
			player_animation->current_type = AnimationType::Idle;
		}
		else {
			player_animation->current_type = AnimationType::Idle_left;
		}

		bool moving = false;
		if (input::IsPressed(KEY::w)) {
			MovementKey msg(up);
			engine->Broadcast(&msg);
			if (player_physics->Velocity.y == 0.0f) {
				player_physics->Force = 85000.0f;
				std::cout << "PlayJump " << player_physics->GetOwner()->GetName() << std::endl;
				audio->playSfx(AudioType::Finn_Jumping);
				
			}
		}
		if (input::IsPressedRepeatedly(KEY::a)) {
			MovementKey msg(left);
			engine->Broadcast(&msg);
			player_physics->Velocity.x -= 500.0f;
			moving = true;
			player_animation->face_right = false;
			player_animation->current_type = AnimationType::Run_left;
		}
		if (input::IsPressedRepeatedly(KEY::d)) {
			MovementKey msg(right);
			engine->Broadcast(&msg);
			player_physics->Velocity.x += 500.0f;
			moving = true;
			player_animation->face_right = true;
			player_animation->current_type = AnimationType::Run;
		}

		if (player_physics->Velocity.y != 0.0f) {
			player_animation->jump_fixed = true;

			if (player_animation->face_right)player_animation->current_type = AnimationType::Jump;
			else player_animation->current_type = AnimationType::Jump_left;
		}
		else player_animation->jump_fixed = false;



		// Audio for Character Movement
		if ((player_physics->Velocity.y == 0.f) && moving) {
			finn_move_time += engine->GetDt();

			if (finn_move_time > 0.4f) {
				audio->playSfx(AudioType::Finn_Walking);
				finn_move_time -= 0.4f;
			}
		}
		else {
			finn_move_time = 0.f;
			moving = false;
		}
	}
	else {
		player_physics->Velocity.x = 0.0f;
		player_animation->current_type = AnimationType::Idle;
		player_animation->face_right = true;
		player_animation->jump_fixed = false;
	}
}

/***************************************************************************/
// Shutdown method, called when the Finn script is stopped.
// @param obj: A pointer to the Object that this script is attached to.
// Performs cleanup tasks for Finn.
/***************************************************************************/
void Finn::Shutdown(Object* obj) {
	std::cout << "Finn Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of the Finn class.
Finn finn("Finn");