/* !
@file	Player.cpp
@author Luke Goh
@date	27/11/2023

This file contains the script the player characters
*//*__________________________________________________________________________*/
#include "Scripts/Player.h"
#include <PhysicsSystem.h>
#include "Core_Engine.h"
#include <components/Physics.h>
#include <components/Animation.h>
#include <components/Transform.h>
#include <components/Body.h>
#include <components/PlayerControllable.h>
#include <components/Event.h>
#include <Audio.h>
#include <input.h>
#include <message.h>

/***********************************************************************************
/
/		NOT IN USE
/
/***********************************************************************************
// Constructor for the Player class.
// @param name: A string representing the name of the Player instance.
Player::Player(std::string name) : LogicScript(name)
{
	std::cout << name << " Created" << std::endl;
}
/***************************************************************************/
// Start method, called when the Player script is first activated.
// @param obj: A pointer to the Object that this script is attached to.
// Performs initial setup and configuration for Player.
/***************************************************************************/
void Player::Start(Object* obj) {
	std::cout << "Player Script Ready : "<< obj->GetName() << std::endl;
}
/***************************************************************************/
// Update method, called on every frame to update the player's state.
// @param obj: A pointer to the Object that this script is attached to.
// Contains logic for player's movement and animations.
/***************************************************************************/
void Player::Update(Object* obj) {
	if (obj == nullptr) { 
		//std::cout << "NIL OBJ : Player" << std::endl;
		return; 
	}

	std::cout << " Player Script Update : " << obj->GetName() << std::endl;
	Physics* player_physics = static_cast<Physics*>(obj->GetComponent(ComponentType::Physics));
	Animation* player_animation = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));
	if (player_physics == nullptr || player_animation == nullptr) {
		//std::cout << "NIL COMPONENT : Player" << std::endl;
		return;
	};
	player_physics->Velocity.x = 0.0f;

	if (player_animation->face_right){
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
			
			if (player_physics->GetOwner()->GetName() == "Finn")
				audio->playSfx(AudioType::Finn_Jumping);
			else if (player_physics->GetOwner()->GetName() == "Spark")
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


	/*
	// Audio for Character Movement
	if ((player_physics->Velocity.y == 0.f) && moving) {
		audio->startWalking();
	}
	else {
		audio->stopWalking();
		moving = false;
	}
	*/

}
/***************************************************************************/
// Shutdown method, called when the Player script is being deactivated or destroyed.
// @param obj: A pointer to the Object that this script is attached to.
// Performs cleanup tasks for the player.
/***************************************************************************/
void Player::Shutdown(Object* obj){

	std::cout << "Player Script Shutdown : " << obj->GetName() << std::endl;

}
// Creating an instance of the Player class.
Player player("Player");

