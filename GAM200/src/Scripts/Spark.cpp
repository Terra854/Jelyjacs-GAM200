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


bool Spark::Just_detached;

float spark_move_time = 0.f;

Spark::Spark(std::string name) : LogicScript(name)
{ 
	std::cout << name << " Created" << std::endl;
	Connected_to_Finn = true;
	Just_detached = false;
}


void Spark::Start(Object* obj) {
	std::cout << "Spark Script Ready : " << obj->GetName() << std::endl;
	Connected_to_Finn = true;
	Just_detached = false;
}

void Spark::Update(Object* obj) {
	if (obj == nullptr || GameLogic::playerObj == nullptr) {
		return;
	}

	// Check if Finn is in the spark
	Object* Finnobj = objectFactory->FindObject("Finn");

	Transform* Finn_t = static_cast<Transform*>(Finnobj->GetComponent(ComponentType::Transform));
	Transform* spark_t = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));
	if (Finn_t == nullptr || spark_t == nullptr) {
		std::cout << "NIL COMPONENT : Spark" << std::endl;
		return;
	};
	/*******************************************************************************************************
	*					Connecting to Finn if Finn is close enough to Spark
	**********************************************************************************************************/
	if (Finn_t->Position.x > spark_t->Position.x - 100 && Finn_t->Position.x < spark_t->Position.x + 100) {
		if (Finn_t->Position.y > spark_t->Position.y - 100 && Finn_t->Position.y < spark_t->Position.y + 100) {
			if (!Just_detached) {
				GameLogic::playerObj = Finnobj;
				Connected_to_Finn = true;
			}
		}
	}
	if (Finn_t->Position.x < spark_t->Position.x - 50 && Finn_t->Position.x > spark_t->Position.x + 50) {
		if (Finn_t->Position.y < spark_t->Position.y - 50 && Finn_t->Position.y > spark_t->Position.y + 50) {
			Just_detached = false;
		}
	}

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


	/********************************************************************************************************
	*
	*	If player is Spark, then control Spark
	*
	********************************************************************************************************/
	Physics* player_physics = static_cast<Physics*>(obj->GetComponent(ComponentType::Physics));
	Animation* player_animation = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));
	if (GameLogic::playerObj->GetName() == "Spark")
	{
		Connected_to_Finn = false;
		
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
			if (player_physics->Velocity.y == 0.0f) {
				player_physics->Force = 85000.0f;
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



		// Audio for Character Movement
		if ((player_physics->Velocity.y == 0.f) && moving) {
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
	}

	
}

void Spark::Shutdown(Object* obj) {
	std::cout << "Spark Script Shutdown : " << obj->GetName() << std::endl;
}

Spark spark("Spark");