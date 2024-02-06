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
		//std::cout << "NIL OBJ : Player" << std::endl;
		return;
	}
	Physics* player_physics = static_cast<Physics*>(obj->GetComponent(ComponentType::Physics));
	Animation* player_animation = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));
	if (GameLogic::playerObj->GetName() == "Finn")
	{
		if (player_physics == nullptr || player_animation == nullptr) {
			//std::cout << "NIL COMPONENT : Player" << std::endl;
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
			//if (static_cast<Rectangular*>(obj->GetComponent(ComponentType::Body))->collision_flag & COLLISION_BOTTOM) {
			if (player_physics->Velocity.y == 0.0f) {
				//player_physics->Velocity.y = 1000.0f;
				player_physics->Force = 75000.0f;
				//audio->playJump();
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
			//audio->startWalking();
			finn_move_time += engine->GetDt();

			if (finn_move_time > 0.4f) {
				audio->playSfx(AudioType::Finn_Walking);
				finn_move_time -= 0.4f;
			}
		}
		else {
			//audio->stopWalking();
			finn_move_time = 0.f;
			moving = false;
		}

		/**************************************************************/
		//	Box Collision
		//	- Get find box object
		/**************************************************************/
		/*
		Object* box_obj = objectFactory->FindObject("Box");

		//get aabb
		Rectangular* finn_body = (Rectangular*)obj->GetComponent(ComponentType::Body);
		AABB finn_aabb_extended = { {(finn_body->aabb.min.x - 5) ,finn_body->aabb.min.y} ,  {(finn_body->aabb.max.x + 5) ,finn_body->aabb.max.y} };
		Rectangular* box_body = (Rectangular*)box_obj->GetComponent(ComponentType::Body);


		//get physics
		Physics* finn_phy = (Physics*)obj->GetComponent(ComponentType::Physics);
		Physics* box_phy = (Physics*)box_obj->GetComponent(ComponentType::Physics);
		box_phy->Velocity.x = 0;

		if (input::IsPressedRepeatedly(KEY::k))
		{
			//check if box colliding with extended finn's aabb
			if (Collision::Check_AABB_AABB(finn_aabb_extended, finn_phy->Velocity, box_body->aabb, box_phy->Velocity, 1.f / 60.f))
			{
				if (input::IsPressedRepeatedly(KEY::d))
				{
					box_phy->Velocity.x = 200;
				}
				else if (input::IsPressedRepeatedly(KEY::a))
				{
					box_phy->Velocity.x = -200;
				}
			}
		}
		*/
	}

	else {
		player_physics->Velocity.x = 0.0f;
		player_animation->current_type = AnimationType::Idle;
		player_animation->face_right = true;
		player_animation->jump_fixed = false;
		//audio->stopWalking();
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