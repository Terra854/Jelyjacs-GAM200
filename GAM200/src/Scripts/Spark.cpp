/* !
@file	Spark.cpp
@author Luke Goh
@date	27/11/2023

This file contains the script for Spark, the player character (Cat)
*//*__________________________________________________________________________*/
#include "Scripts/Spark.h"
#include <Factory.h>
#include <components/Transform.h>
#include <components/Physics.h>
#include <components/Animation.h>
#include <Audio.h>
#include <input.h>
#include <message.h>

// Static variable to track if Spark has just detached from Finn.
bool Spark::Just_detached;
bool Spark::Connected_to_Finn;

float spark_move_time = 0.f;
// Constructor for the Spark class.
// @param name: A string representing the name of the Spark instance.
Spark::Spark(std::string name) : LogicScript(name)
{ 
	std::cout << name << " Created" << std::endl;
	Connected_to_Finn = true;
	Just_detached = false;
}
/***************************************************************************/
// Start method, called when the Spark script is first activated.
// @param obj: A pointer to the Object that this script is attached to.
// Performs initial setup and configuration for Spark.
/***************************************************************************/
void Spark::Start(Object* obj) {
	std::cout << "Spark Script Ready : " << obj->GetName() << std::endl;
	Connected_to_Finn = true;
	Just_detached = false;
}
/*******************************************************************************/
// Update method, called on every frame to update Spark's state.
// @param obj: A pointer to the Object that this script is attached to.
// Contains logic for Spark's movement, interactions with Finn, and animations.
/*******************************************************************************/
void Spark::Update(Object* obj) {
	if (obj == nullptr || GameLogic::playerObj == nullptr) {
		//std::cout << "NIL OBJ : Spark" << std::endl;
		return;
	}
	Object* Finnobj = objectFactory->FindObject("Finn");

	Transform* Finn_t = static_cast<Transform*>(Finnobj->GetComponent(ComponentType::Transform));
	Transform* spark_t = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));
	if (Finn_t == nullptr || spark_t == nullptr) {
		std::cout << "NIL COMPONENT : Spark" << std::endl;
		return;
	};
	
	
	/********************************************************************************************************
	*
	*	If player is Spark, then control Spark
	*
	********************************************************************************************************/
	Physics* player_physics = static_cast<Physics*>(obj->GetComponent(ComponentType::Physics));
	Animation* player_animation = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));
	if (GameLogic::playerObj->GetName() == "Spark")
	{

		/*******************************************************************************************************
		*
		*					Connecting to Finn if Finn is close enough to Spark
		*
		**********************************************************************************************************/
		if (Just_detached) {
			if ((Finn_t->Position.x < (spark_t->Position.x - 50) && Finn_t->Position.x > (spark_t->Position.x + 50)) &&
				(Finn_t->Position.y < (spark_t->Position.y - 50) && Finn_t->Position.y > (spark_t->Position.y + 50))) {
				Just_detached = false;
				std::cout << "Ready to Attach" << std::endl;
			}
		}
		else if ((Finn_t->Position.x > spark_t->Position.x - 50 && Finn_t->Position.x < spark_t->Position.x + 50) &&
			(Finn_t->Position.y > spark_t->Position.y - 50 && Finn_t->Position.y < spark_t->Position.y + 50)) {
			if (!Just_detached) {
				GameLogic::playerObj = Finnobj;
				Connected_to_Finn = true;
				std::cout << "Attached to Finn" << std::endl;
				return;
			}
		}

		if (Connected_to_Finn) return;

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
				player_physics->Force = 85000.0f;
				//audio->playJump();
				std::cout << "PlayJump " << player_physics->GetOwner()->GetName() << std::endl;
				audio->playSfx(AudioType::Spark_Jumping);
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

		/****************************************************************************************/
		//			 Teleport the cat to Finn if the cat has the enough energy
		/*****************************************************************************************/
		if (input::IsPressed(KEY::t)) {
			if (CatPower) {
				spark_t->Position.x = Finn_t->Position.x;
				spark_t->Position.y = Finn_t->Position.y;
				CatPower--;
			}
			else {
				std::cout << "Not Enough Energy\n";
			}
		}



		// Audio for Character Movement
		if ((player_physics->Velocity.y == 0.f) && moving) {
			//audio->startWalking();
			spark_move_time += engine->GetDt();

			if (spark_move_time > 0.4f) {
				audio->playSfx(AudioType::Spark_Walking);
				spark_move_time -= 0.4f;
			}
		}
		else {
			//audio->stopWalking();
			spark_move_time = 0.f;
			moving = false;
		}
	}
	else {
		player_physics->Velocity.x = 0.0f;
		player_animation->current_type = AnimationType::Idle;
		player_animation->face_right = true;
		player_animation->jump_fixed = false;
		//audio->stopWalking();

		/*
		if (Connected_to_Finn) {
			static_cast<Body*>(obj->GetComponent(ComponentType::Body))->active = false;
			spark_t->Position.x = Finn_t->Position.x;
			spark_t->Position.y = Finn_t->Position.y;

			// Need to make sure Spark doesn't fall through the map
			static_cast<Physics*>(obj->GetComponent(ComponentType::Physics))->AffectedByGravity = false;
		}
		else {
			static_cast<Body*>(obj->GetComponent(ComponentType::Body))->active = true;
			static_cast<Physics*>(obj->GetComponent(ComponentType::Physics))->AffectedByGravity = true;
		}
		*/

	}

	
}

/***************************************************************************/
// Shutdown method, called when the Spark script is being deactivated or destroyed.
// @param obj: A pointer to the Object that this script is attached to.
// Performs cleanup tasks for Spark.
/***************************************************************************/
void Spark::Shutdown(Object* obj) {
	std::cout << "Spark Script Shutdown : " << obj->GetName() << std::endl;
}
// Creating an instance of the Spark class.
Spark spark("Spark");